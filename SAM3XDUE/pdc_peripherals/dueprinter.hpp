#pragma once
#ifndef _SAM3X8E_
#include <sam3x8e.h>
#endif

#include <cstring>
#include "SAM3XDUE.H"
#include "duepdc.hpp"

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

		pio_disable_pio(_pio, (1u << _rx_pin) | (1u << _tx_pin));
		pio_enable_pullup(_pio, (1u << _rx_pin));
		pio_disable_output(_pio, (1u << _rx_pin));

		pio_disable_pullup(_pio,  (1u << _tx_pin));
		pio_enable_output(_pio,  (1u << _tx_pin));

		pio_set_periph_mode_A(_pio,(1u << _rx_pin) | (1u << _tx_pin));
	}

public:
	DueSerialPorts() {}

	DueSerialPorts(volatile uint32_t* base, Pio* pio, uint32_t rx_pin, uint32_t tx_pin, uint32_t id_pmc)
		: _base(base), _pio(pio), _rx_pin(rx_pin), _tx_pin(tx_pin), _id_pmc(id_pmc) {}

};

class DuePrinter : public PDC_streamable<DuePrinter> {
private:
	//volatile uint32_t const* _cr, *_mr, *_ier, *_idr, *_imr, *_sr, *_rhr,*_thr, *_brgr;
	//volatile uint32_t const* _pdc_rpr, *_pdc_rcr, *_pdc_tpr, *_pdc_tcr; 
	//volatile uint32_t const* _pdc_rnpr, *_pdc_rncr, *_pdc_tnpr, *_pdc_tncr;
	//volatile uint32_t const* _pdc_ptcr, * _pdc_ptsr;


	//for literals, which need to be in ram to be DMAd
	char _literal_pool[4][24];
	uint8_t _pool_head = 0;

	uint32_t baud_to_brgr(uint32_t baud, bool oversample = false) {
		uint32_t num = 84000000;
		uint32_t den = oversample ? (8 * baud) : (16 * baud);
		
		uint32_t cd = num / den;
		uint32_t fp = 0;
		
		if (!oversample) {
			uint32_t rem = num % den;
			fp = ((rem * 8) + (den / 2)) / den;
			if (fp == 8) {
				fp = 0;
				cd += 1;
			}
		} else {
			//fractional part is not supported when oversampling
			cd = (num + (den / 2)) / den;
		}

		return (cd & 0xFFFF) | ((fp & 0x7) << 16);
	}

	DuePrinter(DueSerialPorts port, uint32_t baud, bool oversample = false) : _base(port._base) {
		// regs are pointing to uint32_t so offsets are divided by 4
		/*
		_cr = port.__base + 0;
		_mr = port.__base + 1;
		_ier = port.__base + 2;
		_idr = port.__base + 3;
		_imr = port.__base + 4;
		_sr = port.__base + 5;
		_rhr = port.__base + 6;
		_thr = port.__base + 7;
		_brgr = port.__base + 8;
		*/

		//no parity, normal channel mode, for usart: char len = 8 bits
		uint32_t reg_mode = (3 << 6) | (4 << 9);
		if(oversample && _base != Due_UART._base){
			reg_mode |= (1 << 19);
		}
		*(_base + 1) = reg_mode;

		//assumes the clock source for usart is mck and mck is 84MHz
		if(_base == Due_UART._base){
			*(_base + 8) = baud_to_brgr(baud) & 0xffff;
		} else {
			*(_base + 8) = baud_to_brgr(baud, oversample);
		}

		//reset u(s)art rx/tx and enable rx/tx
		*(_base + 0) = (1 << 2) | (1 << 3);
		*(_base + 0) = (1 << 4) | (1 << 6);
		*(_base + 0x48) = 1 | (1 << 8); //pdc rx/tx en
		/*
		_pdc_rpr = port.__base + 0x40;
		_pdc_rcr = port.__base + 0x41;
		_pdc_tpr = port.__base + 0x42;
		_pdc_tcr = port.__base + 0x43;
		_pdc_rnpr = port.__base + 0x44;
		_pdc_rncr = port.__base + 0x45;
		_pdc_tnpr = port.__base + 0x46;
		_pdc_tncr = port.__base + 0x47;
		_pdc_ptcr = port.__base + 0x48;
		_pdc_ptsr = port.__base + 0x49;
		*/

		*(_base + 3) = 0xFFFFFFFF;
		
		auto irq_id = static_cast<IRQn_Type>(port._id_pmc);
		NVIC_ClearPendingIRQ(irq_id);
		NVIC_SetPriority(irq_id, 0);
		NVIC_EnableIRQ(irq_id);
	}

public:
	volatile uint32_t* _base;
	uint32_t _isr_offset = 5;
	uint32_t _ier_offset = 2;
	uint32_t _idr_offset = 3;

	uint32_t _endrx_mask = (1 << 3);
	uint32_t _rxbufe_mask = (1 << 12);
	uint32_t _endtx_mask = (1 << 4);
	uint32_t _txbufe_mask = (1 << 11);


	//DuePrinter overloads queue_write so we need to specify this
	using PDC_streamable<DuePrinter>::queue_write;

	DuePrinter() {}

	static DuePrinter begin(DueSerialPorts port, uint32_t baud = 115200, bool oversample = false) {
		port.init();
		return DuePrinter(port, baud, oversample);
	}

	//blocking read
	void read(void* dst, int cnt = 0, char deliminator = '\n') {
		while (*(_base + 0x41) != 0 || *(_base + 0x45) != 0) {}

		uint8_t* buf = static_cast<uint8_t*>(dst);

		if (cnt > 0) {
			while (cnt-- > 0) {
				while (!(*(_base + 5) & (1 << 0))) {}
				*buf++ = static_cast<uint8_t>(*(_base + 6));
			}
		}
		else {
			while (true) {
				while (!(*(_base + 5) & (1 << 0))) {}
				uint8_t data = static_cast<uint8_t>(*(_base + 6));
				*buf++ = data;
				if (data == static_cast<uint8_t>(deliminator)) break;
			}
			*buf = '\0';
		}
	}

	//string literals must be in RAM to be dma'd
	bool queue_write(const char* str){
		uint32_t len = strlen(str);
		if (len == 0) return false; 
		if (len > 24) len = 24;

		char* ram_buf = _literal_pool[_pool_head];
		
		memcpy(ram_buf, str, len);

		//only advance the pool head if it was queued sucessfully
		if (PDC_streamable<DuePrinter>::queue_write(static_cast<const void*>(ram_buf), len)) {
			_pool_head = (_pool_head + 1) % 4;
			return true;
		}
		
		return false;
	}
	
	//blocking write
	void write(const char* str, int cnt = 0) {
		while (*(_base + 0x43) != 0 || *(_base + 0x47) != 0) {}

		if (cnt > 0) {
			while (cnt-- > 0) {
				while (!(*(_base + 5) & (1 << 1))) {}
				*(_base + 7) = *str++;
			}
		}
		else {
			while (*str) {
				while (!(*(_base + 5) & (1 << 1))) {}
				*(_base + 7) = *str++;
			}
		}

		while (!(*(_base + 5) & (1 << 9))) {}
	}

};

#endif


