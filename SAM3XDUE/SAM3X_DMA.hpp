#pragma once
#ifndef _SAM3X8E_
#include <sam3x8e.h>
#endif

//#include <SAM3X_DMA.cpp>

#ifndef SAM3XDMA
#define SAM3XDMA

//might as well be a struct at this point
struct sam3xdma_lli {

	sam3xdma_lli();
	sam3xdma_lli(uint32_t saddr, uint32_t daddr, uint32_t ctrla=0, uint32_t ctrlb=0, uint32_t dscr=0);
	
	sam3xdma_lli(const sam3xdma_lli&) = delete;
	sam3xdma_lli& operator=(const sam3xdma_lli&) = delete;
	sam3xdma_lli(sam3xdma_lli&&) = delete;
	sam3xdma_lli& operator=(sam3xdma_lli&&) = delete;

	/*
		please do not try to use the << operator on the calling object
		this is supposed to be performant not safe
	*/

	void set_src(void* addr);
	void set_dst(void* addr);
	void set_src(uint32_t addr);
	void set_dst(uint32_t addr);
	sam3xdma_lli& set_dscr(sam3xdma_lli& next);

	void set_src(const void* addr);
	void set_dst(const void* addr);
	void set_ctrla(const uint32_t ctrla);
	void set_ctrlb(const uint32_t ctrlb);

	void terminate();
	uint32_t addr() const;

	enum lli_fields : int {
		SADDR = 0,
		DADDR = 1,
		CTRLA = 2,
		CTRLB = 3,
		DSCR = 4
	};

	sam3xdma_lli& operator<<(sam3xdma_lli& next);

	alignas(4) volatile uint32_t lli_data[5]; //saddr, daddr, ctrla, ctrlb, dscr

};



#endif