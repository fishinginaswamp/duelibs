#pragma once

#ifndef _DUE_DAC
#define _DUE_DAC

#ifndef _SAM3X8E_
#include <sam3x8e.h>
#endif
#include "SAM3XDUE.H"


//#include "duedac.cpp"

inline void init_refresh_timer0(uint32_t refresh_rate = 44100U) {
	pmc_enable_periph_clk(ID_TC0);
	//mclk/128, wave mode
	REG_TC0_CMR0 = 3 | (0b10 << 13) | (1 << 15);
	const uint32_t refresh_count = 656220U / refresh_rate;
	REG_TC0_RC0 = refresh_count;
	REG_TC0_IER0 = (1 << 4);
	REG_TC0_IMR0 = (1 << 4);

	NVIC_ClearPendingIRQ(TC0_IRQn);
	NVIC_SetPriority(TC0_IRQn, 3);
	NVIC_EnableIRQ(TC0_IRQn);
	REG_TC0_CCR0 = 1 | (1 << 2);
}


extern "C" uint32_t pmc_enable_periph_clk(uint32_t);

template <typename T>
class DueSound {
private:
	uint32_t _len;
	T* _data;

public:
	

};


enum class DAC_TC_TRIGGER {
	TC_0 = 0,
	TC_1 = 1,
	TC_2 = 2
};


extern "C" void TC0_Handler() {

}

//due dac only uses one channel DAC0
//it can only use TCO for triggers (ch 0/1/2)
template<DAC_TC_TRIGGER channel>
class DueDAC {
private:
	uint32_t* _tc_base;

	DueDAC() {}


public:

	DueDAC(const DueDAC&) = delete;
	void operator=(const DueDAC&) = delete;


	void begin()
	{
		constexpr uint32_t ch = static_cast<uint32_t>(channel);
		uint32_t* const tc_base = reinterpret_cast<uint32_t*>(0x40080000);
		_tc_base = tc_base + (ch * 0x40);

		pmc_enable_periph_clk(ID_DACC);
		
		if constexpr (channel == DAC_TC_TRIGGER::TC_0) {
			pmc_enable_periph_clk(ID_TC0);
		}
		else if constexpr (channel == DAC_TC_TRIGGER::TC_1) {
			pmc_enable_periph_clk(ID_TC0);
		}
		else if constexpr (channel == DAC_TC_TRIGGER::TC_2) {
			pmc_enable_periph_clk(ID_TC0);
		}
		else {
			static_assert(sizeof(DueDAC) == 0, "invalid channel, how did you do that?");
		}

		// DACC config
	}

	template<typename T>
	void play(DueSound<T>& sound) {

	}


};


#endif