#include "SAM3X_DMA.hpp"


sam3xdma_lli::sam3xdma_lli() {
	this->lli_data[lli_fields::SADDR] = 0;
	this->lli_data[lli_fields::DADDR] = 0;
	this->lli_data[lli_fields::CTRLA] = 0;
	this->lli_data[lli_fields::CTRLB] = 0;
	this->lli_data[lli_fields::DSCR] = 0;
}

sam3xdma_lli::sam3xdma_lli(uint32_t saddr, uint32_t daddr, uint32_t ctrla, uint32_t ctrlb, uint32_t dscr) {
	this->lli_data[lli_fields::SADDR] = saddr;
	this->lli_data[lli_fields::DADDR] = daddr;
	this->lli_data[lli_fields::CTRLA] = ctrla;
	this->lli_data[lli_fields::CTRLB] = ctrlb;
	this->lli_data[lli_fields::DSCR] = dscr;
}

void sam3xdma_lli::set_src(void* addr) {
	this->lli_data[lli_fields::SADDR] = reinterpret_cast<uint32_t>(addr);
}

void sam3xdma_lli::set_dst(void* addr) {
	this->lli_data[lli_fields::DADDR] = reinterpret_cast<uint32_t>(addr);
}


void sam3xdma_lli::set_src(const void* addr) {
	this->lli_data[lli_fields::SADDR] = reinterpret_cast<uint32_t>(addr);
}

void sam3xdma_lli::set_dst(const void* addr) {
	this->lli_data[lli_fields::DADDR] = reinterpret_cast<uint32_t>(addr);
}

void sam3xdma_lli::set_ctrla(const uint32_t ctrla) {
	this->lli_data[lli_fields::CTRLA] = ctrla;
}

void sam3xdma_lli::set_ctrlb(const uint32_t ctrlb) {
	this->lli_data[lli_fields::CTRLB] = ctrlb;
}

void sam3xdma_lli::set_src(uint32_t addr) {
	this->lli_data[lli_fields::SADDR] = addr;
}

void sam3xdma_lli::set_dst(uint32_t addr) {
	this->lli_data[lli_fields::DADDR] = addr;
}

sam3xdma_lli& sam3xdma_lli::set_dscr(sam3xdma_lli& next) {
	this->lli_data[lli_fields::DSCR] = reinterpret_cast<uint32_t>(next.lli_data);
	return next;
}

sam3xdma_lli& sam3xdma_lli::operator<<(sam3xdma_lli& next) {
	this->lli_data[lli_fields::DSCR] = reinterpret_cast<uint32_t>(next.lli_data);
	return next;
}

uint32_t sam3xdma_lli::addr() const {
	return reinterpret_cast<uint32_t>(this->lli_data);
}


void sam3xdma_lli::terminate() {
	this->lli_data[lli_fields::DSCR] = 0;
}
