#pragma once

#ifndef _DUE_DAC
#define _DUE_DAC

#ifndef _SAM3X8E_
#include <sam3x8e.h>
#endif

#include "SAM3XDUE.H"
#include "duepdc.hpp"
//#include "duedac.cpp"

extern "C" uint32_t pmc_enable_periph_clk(uint32_t);

enum class DAC_TC_TRIGGER {
	TC_0 = 0,
	TC_1 = 1,
	TC_2 = 2,
	FREERUNNING = 3
};

enum class DAC_SOUND_MODE {
	MONO = 0,
	STEREO = 1
};

extern "C" void TC0_Handler() {

}

//due dac only uses one channel DAC0
//it can only use TCO for triggers (ch 0/1/2)
template<DAC_TC_TRIGGER tc_channel = DAC_TC_TRIGGER::FREERUNNING, DAC_SOUND_MODE tagmode = DAC_SOUND_MODE::MONO>
class DueDAC : public PDC_tx_streamable<DueDAC<tc_channel, tagmode>> {
private:
	volatile uint32_t* _tc_base;

	void init_timer(uint32_t refresh_rate = 44100U){
		//the tc_base points to uint32_t, so offset needs to be divided by 4

		//set TIOA on RC match, clear on RA match (the signal for the DACC)
		//REG_TC0_CMR0 = 3 | (0b10 << 13) | (1 << 15);
		*(_tc_base + 0x01) = 3 | (0b10 << 13) | (1 << 15) | (1 << 16) | (2 << 18);
		
		
		const uint32_t refresh_count = 656220U / refresh_rate;
		//REG_TC0_RC0 = refresh_count;
		*(_tc_base + 0x05) = refresh_count / 2;
		*(_tc_base + 0x07) = refresh_count;

		//REG_TC0_IER0 = (1 << 4);
		//*(_tc_base + 0x24) = (1 << 4);
		//REG_TC0_IMR0 = (1 << 4);
		//*(_tc_base + 0x2c) = (1 << 4); 
		//REG_TC0_CCR0 = 1 | (1 << 2);
		*(_tc_base + 0x00) = 1 | (1 << 2);
	}
	
	void init_port(){
		pio_disable_pio(PIOB, (1 <<15));
		pio_disable_pullup(PIOB, (1 <<15));
	}

	void init_pdc(){
		REG_DACC_PTCR = (1 << 8); //enable TX via PDC
	
		NVIC_DisableIRQ(DACC_IRQn);
		NVIC_ClearPendingIRQ(DACC_IRQn);
		NVIC_SetPriority(DACC_IRQn,0);
		NVIC_EnableIRQ(DACC_IRQn);
	}

public:
	volatile uint32_t* _base = reinterpret_cast<volatile uint32_t*>(0x400C8000);
	uint32_t _isr_offset = 12;
	uint32_t _ier_offset = 9;
	uint32_t _idr_offset = 10;

	uint32_t _endtx_mask = (1 << 2);
	uint32_t _txbufe_mask = (1 << 3);

	DueDAC(const DueDAC&) = delete;
	void operator=(const DueDAC&) = delete;

	DueDAC() {}

	void begin(uint32_t sample_rate = 44100U, uint32_t refresh = 31)
	{
		uint32_t dac_trigsel = 0;

		//if using tc, enable ext. trigger and set the correct trigger src
		if constexpr (tc_channel == DAC_TC_TRIGGER::TC_0) {
			pmc_enable_periph_clk(ID_TC0);
			dac_trigsel = (0b0011);
		}
		else if constexpr (tc_channel == DAC_TC_TRIGGER::TC_1) {
			pmc_enable_periph_clk(ID_TC1);
			dac_trigsel = (0b0101);
		}
		else if constexpr (tc_channel == DAC_TC_TRIGGER::TC_2) {
			pmc_enable_periph_clk(ID_TC2);
			dac_trigsel = (0b0111);
		}
		else if constexpr (tc_channel == DAC_TC_TRIGGER::FREERUNNING){
			//nothing
		} else {
			static_assert(sizeof(DueDAC) == 0, "invalid channel, how did you do that?");
		}


		if constexpr (tc_channel != DAC_TC_TRIGGER::FREERUNNING){
			constexpr uint32_t ch = static_cast<uint32_t>(tc_channel);
			uint32_t* const tc_base = reinterpret_cast<uint32_t*>(0x40080000);
			_tc_base = tc_base + (ch * 0x40);
			init_timer(sample_rate);
		}

		//dac config
		init_port();
		pmc_enable_periph_clk(ID_DACC);

		refresh = refresh & 0xff;
		//tagmode: using TAG allows writing to both channels, when disabled uses ch 0
		//writing to FIFO in WORD mode
		//usersel = 0 -> DAC0 if freerunning is disabled
		//REG_DACC_MR = dac_trigsel | (1 << 4) | (refresh << 8) | (static_cast<uint32_t>(tagmode) << 20);
		REG_DACC_MR = dac_trigsel | (refresh << 8) | (static_cast<uint32_t>(tagmode) << 20);

		//DAC channel enable
		if constexpr(tagmode == DAC_SOUND_MODE::STEREO){
			//STEREO IS UNFINISHED
			REG_DACC_CHER = 0b11;
		} else {
			REG_DACC_CHER = 1;
		}

		init_pdc();

		//this just gets the DAC output going
		REG_DACC_CDR = 0;
	}

};


#endif