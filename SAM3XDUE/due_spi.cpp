#include <due_spi.hpp>

namespace due_spi {

	bool spi::tx(uint16_t data, bool lastxfer) {
		if (spi::spi_busy()) {
			return false;
		}

		if (lastxfer)
			REG_SPI0_TDR = data | (m_pcs << 16) | (1 << 24);
		else
			REG_SPI0_TDR = data | (m_pcs << 16);

		return true;
	}

	uint32_t spi::rx() {
		return REG_SPI0_RDR;
	}

	void spi::tx_blocking(uint16_t data, bool lastxfer) {
		while (!(REG_SPI0_SR & (1 << 9))) {}

		if(lastxfer)
			REG_SPI0_TDR = data | (m_pcs << 16) | (1 << 24);
		else			
			REG_SPI0_TDR = data | (m_pcs << 16);

		//Pauses while the data in the transmit register has not been sent to serializer
		while (!(REG_SPI0_SR & (1 << 9))) {}
	}

	uint32_t spi::rx_blocking() {
		//Pauses while data has not been received in the RDR since last time it was read
		while (!(REG_SPI0_SR & 1)) {}
		return REG_SPI0_RDR;
	}


	uint32_t spi::transact(uint16_t data, bool lastxfer) {
		this->tx_blocking(data, lastxfer);
		return this->rx_blocking();

	}
}