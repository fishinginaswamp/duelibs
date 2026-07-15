#pragma once
#ifndef _SAM3X8E_
#include <sam3x8e.h>
#endif
#include "SAM3XDUE.H"
#include <cstring>

#ifndef DUEPRINTER
#define DUEPRINTER

extern "C" uint32_t pmc_enable_periph_clk(uint32_t);

class DuePrinter;
class DueSerialPorts;

extern DueSerialPorts Due_USART0;
extern DueSerialPorts Due_USART1;
extern DueSerialPorts Due_USART2;
extern DueSerialPorts Due_USART3;
extern DueSerialPorts Due_UART;

class DueSerialPorts {
	friend class DuePrinter;

private:
	volatile uint32_t* _base;
	Pio* _pio;
	uint32_t _rx_pin;
	uint32_t _tx_pin;
	uint32_t _id_pmc;

	void init() {
		// i have confirmed that the ID_xxxx for the pmc is the same as the NVIC IDs
		pmc_enable_periph_clk(_id_pmc);

		pio_disable_pio(_pio, _rx_pin | _tx_pin);
		pio_enable_pullup(_pio, _rx_pin);
		pio_disable_output(_pio, _rx_pin);

		pio_disable_pullup(_pio, _tx_pin);
		pio_enable_output(_pio, _tx_pin);

		pio_set_periph_mode_A(_pio, _rx_pin | _tx_pin);
	}

public:
	DueSerialPorts() {}

	DueSerialPorts(volatile uint32_t* base, Pio* pio, uint32_t rx_pin, uint32_t tx_pin, uint32_t id_pmc)
		: _base(base), _pio(pio), _rx_pin(rx_pin), _tx_pin(tx_pin), _id_pmc(id_pmc) {}

};

class DuePrinter {
private:

	volatile uint32_t* base;
	//volatile uint32_t const* _cr, *_mr, *_ier, *_idr, *_imr, *_sr, *_rhr,*_thr, *_brgr;
	//volatile uint32_t const* _pdc_rpr, *_pdc_rcr, *_pdc_tpr, *_pdc_tcr; 
	//volatile uint32_t const* _pdc_rnpr, *_pdc_rncr, *_pdc_tnpr, *_pdc_tncr;
	//volatile uint32_t const* _pdc_ptcr, * _pdc_ptsr;

	void (*rx_callback)() = nullptr;
	void (*tx_callback)() = nullptr;

	struct {
		void* volatile ptr = nullptr;
		volatile uint32_t cnt = 0;

		operator bool() const {
			return ptr != nullptr;
		}
	} rx_queue, tx_queue;
	
	uint16_t baud_to_brgr(uint32_t baud) {
		return (84000000 / (16* baud)) & 0xffff;
	}

	void enable_rx_interrupt() {
		*(base + 2) = (1 << 3);
	}

	void enable_tx_interrupt() {
		*(base + 2) = (1 << 4);
	}

	void disable_rx_interrupt() {
		*(base + 3) = (1 << 3);
	}

	void disable_tx_interrupt() {
		*(base + 3) = (1 << 4);
	}

	DuePrinter(DueSerialPorts port, uint32_t baud) : base(port._base) {
		// regs are pointing to uint32_t so offsets are divided by 4
		/*
		_cr = port._base + 0;
		_mr = port._base + 1;
		_ier = port._base + 2;
		_idr = port._base + 3;
		_imr = port._base + 4;
		_sr = port._base + 5;
		_rhr = port._base + 6;
		_thr = port._base + 7;
		_brgr = port._base + 8;
		*/

		//no parity, normal channel mode, for usart: char len = 8 bits
		*(base + 1) = (3 << 6) | (4 << 9);

		auto brgr_val = baud_to_brgr(baud);
		//assumes the clock source for usart is mck and mck is 84MHz
		if (brgr_val == 0) {
			brgr_val = baud_to_brgr(115200);
		}
		*(base + 8) = brgr_val;

		//reset u(s)art rx/tx and enable rx/tx
		*(base + 0) = (1 << 2) | (1 << 3);
		*(base + 0) = (1 << 4) | (1 << 6);

		*(base + 0x48) = 1 | (1 << 8); //pdc rx/tx en
		/*
		_pdc_rpr = port._base + 0x40;
		_pdc_rcr = port._base + 0x41;
		_pdc_tpr = port._base + 0x42;
		_pdc_tcr = port._base + 0x43;
		_pdc_rnpr = port._base + 0x44;
		_pdc_rncr = port._base + 0x45;
		_pdc_tnpr = port._base + 0x46;
		_pdc_tncr = port._base + 0x47;
		_pdc_ptcr = port._base + 0x48;
		_pdc_ptsr = port._base + 0x49;
		*/

		*(base + 3) = 0xFFFFFFFF;
		auto irq_id = static_cast<IRQn_Type>(port._id_pmc);
		NVIC_ClearPendingIRQ(irq_id);
		NVIC_SetPriority(irq_id, 0);
		NVIC_EnableIRQ(irq_id);
	}

public:
	
	DuePrinter() {}

	static DuePrinter begin(DueSerialPorts port, uint32_t baud) {
		port.init();
		return DuePrinter(port, baud);
	}

	void handle(){
		uint32_t sr = *(base + 5);
		handle_tx_interrupt(sr);
		handle_rx_interrupt(sr);
	}

	void handle_rx_interrupt(uint32_t sr) {
		if (!(sr & (1 << 3)))
			return;

		uint32_t callback_ret = -1;
		if (rx_callback) {
			(*rx_callback)();
		}

		if (rx_queue) {
			void* volatile p = rx_queue.ptr;
			uint32_t c = rx_queue.cnt;
			rx_queue.ptr = nullptr;
			queue_read(p, c);
		}
		else {
			disable_rx_interrupt();
		}

	}

	void handle_tx_interrupt(uint32_t sr) {
		if (!(sr & (1 << 4)))
			return;

		if (tx_callback) {
			(*tx_callback)();
		}

		if (tx_queue) {
			void* volatile p = tx_queue.ptr;
			uint32_t c = tx_queue.cnt;
			tx_queue.ptr = nullptr;
			queue_write(p, c);
		}
		else {
			disable_tx_interrupt();
		}
	}

	void set_read_callback(void (*callback)()) {
		rx_callback = callback;
	}

	void set_tx_callback(void (*callback)()) {
		tx_callback = callback;
	}

	//non-blocking read
	void queue_read(void* volatile dst, uint32_t cnt) {
		if (*(base + 0x41) == 0) {
			*(base + 0x40) = reinterpret_cast<uint32_t>(dst);
			*(base + 0x41) = cnt;
			enable_rx_interrupt();
			return;
		}
		else if (*(base + 0x45) == 0) { // if idle, start
			*(base + 0x44) = reinterpret_cast<uint32_t>(dst);
			*(base + 0x45) = cnt;
			enable_rx_interrupt();
		}
		else if(!rx_queue){
			rx_queue.ptr = dst;
			rx_queue.cnt = cnt;
		}
	}

	//non-blocking write
	void queue_write(void* src, uint32_t cnt = 0) {
		if (cnt == 0) {
			const char* c_ptr = static_cast<const char*>(src);
			cnt = strlen(c_ptr);
		}
		if (*(base + 0x43) == 0) { // if idle, start
			*(base + 0x42) = reinterpret_cast<uint32_t>(src);
			*(base + 0x43) = cnt;
			enable_tx_interrupt();
			return;
		}
		else if(*(base + 0x47) == 0){
			*(base + 0x46) = reinterpret_cast<uint32_t>(src);
			*(base + 0x47) = cnt;
			enable_tx_interrupt();
		}
		else if(!tx_queue){
			tx_queue.ptr = src;
			tx_queue.cnt = cnt;
		}
	}

	//blocking read
	void read(void* dst, int cnt = 0, char deliminator = '\n') {
		while (*(base + 0x41) != 0 || *(base + 0x45) != 0) {}

		uint8_t* buf = static_cast<uint8_t*>(dst);

		if (cnt > 0) {
			while (cnt-- > 0) {
				while (!(*(base + 5) & (1 << 0))) {}
				*buf++ = static_cast<uint8_t>(*(base + 6));
			}
		}
		else {
			while (true) {
				while (!(*(base + 5) & (1 << 0))) {}
				uint8_t data = static_cast<uint8_t>(*(base + 6));
				*buf++ = data;
				if (data == static_cast<uint8_t>(deliminator)) break;
			}
			*buf = '\0';
		}
	}

	void write(const char* str, int cnt = 0) {
		while (*(base + 0x43) != 0 || *(base + 0x47) != 0) {}

		if (cnt > 0) {
			while (cnt-- > 0) {
				while (!(*(base + 5) & (1 << 1))) {}
				*(base + 7) = *str++;
			}
		}
		else {
			while (*str) {
				while (!(*(base + 5) & (1 << 1))) {}
				*(base + 7) = *str++;
			}
		}

		while (!(*(base + 5) & (1 << 9))) {}
	}

};

#endif


