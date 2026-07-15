#pragma once
#ifndef _SAM3X8E_
#include <sam3x8e.h>
#endif
//#include <due_spi.cpp>
#include <SAM3XDUE.h>

namespace due_spi {

	//poor due is missing half of its cs pins
	//there are only 2 cases for the pins, either on port A and periph mode A or port B mode B
	enum class spi_cs_pins {
		D10 = (1 << 28), //CS0 - PA28 mode A
		D4 = (1 << 29), //CS1 - PA29 mode A
		//the port/mode can be determined by the value of the pin bit
		A11 = (1 << 20), //CS1 - PB20 mode B
		D52 = (1 << 21) //CS2 - PB21 mode B
	};

	//bits per transfer
	enum class spi_bits {
		BIT8 = 0,
		BIT9,
		BIT10,
		BIT11,
		BIT12,
		BIT13,
		BIT14,
		BIT15,
		BIT16
	};

	/*
	const uint32_t cs0_D10 = (1 << 28);
	const uint32_t cs1_D4 = (1 << 29);
	const uint32_t cs1_A11 = (1 << 20);	//the cs2 pin on port A is not wired on the due (PA30)
	const uint32_t cs2_D52 = (1 << 21);	//cs3 pin on port a not wired on the due (PA31 and PB23)
	*/


	//only valid for 329,411 Hz to 84MHz
	inline constexpr uint8_t hz_to_baud_div(uint32_t hz) {
		//assuming clock source is 84MHz because I don't touch the pmc stuff
		return 84000000/hz;
	}
	
	class spi {
	private:

	public:
		uint8_t m_cs_num;
		uint8_t m_pcs;
		//default to 500khz clock and cs 0 on pin D10
		//NOTE: if one SPI cs has baud div of 1, they all need to have baud div of 1

		spi(uint8_t hz) : spi(spi_cs_pins::D10, hz_to_baud_div(hz)) {}

		spi(const spi_cs_pins _cs_pin = spi_cs_pins::D10, uint8_t _baud_div = 168, 
			spi_bits _bits = spi_bits::BIT8, 
			uint8_t _delaybs = 0, uint8_t _delaybct = 0,
			bool _csaat = true, bool _cpol = false,
			bool _ncpha = true, bool _csnaat = false) {
			pmc_enable_periph_clk(ID_SPI0);
			//constant for all spi instances
			pio_enable_output(PIOA, SPI0_MOSI | SPI0_SCK);
			pio_disable_output(PIOA, SPI0_MISO);
			pio_disable_pullup(PIOA, SPI0_MOSI | SPI0_MISO | SPI0_SCK);
			pio_disable_pio(PIOA, SPI0_MOSI | SPI0_MISO | SPI0_SCK);
			pio_set_periph_mode_A(PIOA, SPI0_MOSI | SPI0_MISO | SPI0_SCK);

			uint32_t pin = static_cast<int>(_cs_pin);
			if (pin & (1 << 28)) {
				m_cs_num = 0;
			}
			else if (pin & (1 << 20) || pin & (1 << 29)) {
				m_cs_num = 1;
			}
			else {
				m_cs_num = 2;
			}
			m_pcs = 0x0f & (~(1 << m_cs_num));
			if (pin < (1 << 23)) {
				pio_enable_output(PIOB, pin);
				pio_disable_pullup(PIOB, pin);
				pio_disable_pio(PIOB, pin);
				pio_set_periph_mode_B(PIOB, pin);
			}
			else {
				pio_enable_output(PIOA, pin);
				pio_disable_pullup(PIOA, pin);
				pio_disable_pio(PIOA, pin);
				pio_set_periph_mode_A(PIOA, pin);
			}

			const uint32_t SPI0_CSR_ADDR = 0x40008030U + 0x04U * m_cs_num;
			uint8_t ncpha = _ncpha ? 1 : 0;
			uint8_t cpol = _cpol ? 1 : 0;
			uint8_t csaat = _csaat ? 1 : 0;
			uint8_t csnaat = _csnaat ? 1 : 0;
			uint32_t csr = (_delaybct << 24 ) | (_delaybs << 16) | (_baud_div << 8) | 
				((static_cast<uint32_t>(_bits) << 4) & 0xF) //bits per transfer
				| (csaat << 3) | (csnaat << 2) | (ncpha << 1) | (cpol);
			
			*(reinterpret_cast<uint32_t*>(SPI0_CSR_ADDR)) = csr;

			//8 gives 10.5MHz clock, 9 gives 9.333MHz clock
			//*(reinterpret_cast<uint32_t*>(SPI0_CSR_ADDR)) = (1 << 1) | (csnaat << 2) | (csaat << 3) | (0 << 4) | (_baud_div << 8);

			REG_SPI0_MR = 1 | (1 << 1) | (1 << 4); //master mode, variable peripheral select, no decode/fault/delay
			REG_SPI0_CR = 1; //enable spi
		}

		inline bool spi_busy() {
			return !(REG_SPI0_SR & (1 << 9));
		}


		bool tx(uint16_t data, bool lastxfer = false);
		void tx_blocking(uint16_t data, bool lastxfer = false);

		uint32_t rx();
		uint32_t rx_blocking();

		uint32_t transact(uint16_t data, bool lastxfer = false);
	};

}