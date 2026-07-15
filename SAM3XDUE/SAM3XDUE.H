#pragma once
#ifndef _SAM3X8E_
extern "C" {
#include <sam3x8e.h>
}
#endif

//***THIS IS FOR PIN STUFF***//
#ifndef SAM3XDUE
#define SAM3XDUE

inline void operator delete(void* ptr, unsigned int size) noexcept {
	::operator delete(ptr);
}

inline void operator delete[](void* ptr, unsigned int size) noexcept {
	::operator delete[](ptr);
}

extern "C" uint32_t pmc_enable_periph_clk(uint32_t);
extern "C" uint32_t pmc_disable_periph_clk(uint32_t);

//***PIN DEFINITIONS***//
// The letter at the end of the definition name is the port the pin is on, as a reminder
//Analog pins
#define PIN_A0A (1<<16)		//PA16
#define PIN_A1A (1<<24)		//PA24	
#define PIN_A2A (1<<23)		//PA23
#define PIN_A3A (1<<22)		//PA22
#define PIN_A4A (1<<6)		//PA6
#define PIN_A5A (1<<4)		//PA4
#define PIN_A6A (1<<3)		//PA3
#define PIN_A7A (1<<2)		//PA2
#define PIN_A8B (1<<17)		//PB17
#define PIN_A9B (1<<18)		//PB18
#define PIN_A10B (1<<19)	//PB19
#define PIN_A11B (1<<20)	//PB20
//Digital pins
#define PIN_1A (1<<9)		//PA9, Arduino TX0 -- UTXD periph. A
#define PIN_0A (1<<8)		//PA8, Arduino RX0 -- PWM periph. B -- URXD periph. A
#define PIN_2B (1<<25)		//PB25, TIOA periph mode B
#define PIN_3C (1<<28)		//PC28
#define PIN_4A (1<<29)		//PA29
#define PIN_5C (1<<25)		//PC25
#define PIN_6C (1<<24)		//PC24
#define PIN_7C (1<<23)		//PC23
#define PIN_8C (1<<22)		//PC22
#define PIN_9C (1<<21)		//PC21
#define PIN_10A (1<<28)		//PA28 - SPI0_NPCS0 (Periph. mode A)
#define PIN_11D (1<<7)		//PD7
#define PIN_12D (1<<8)		//PD8
#define PIN_13B (1<<27)		//PB27, built in LED
#define PIN_14D (1<<4)		//PD4, also TX3
#define PIN_15D (1<<5)		//PD5, also RX3
#define PIN_16A (1<<13)		//PA13, also TX1
#define PIN_17A (1<<12)		//PA12, also RX1
#define PIN_18A (1<<11)		//PA11, also TX0 (Periph. mode A)
#define PIN_19A (1<<10)		//PA10, also RX0 (Periph. mode A)
#define PIN_20B (1<<12)		//PB12, also SDA (near the bank of digital pins)
#define PIN_21B (1<<13)		//PB13, also SCL (near the bank of digital pins)
//note: the SDA/SCL pin pairs (the top and bottom ones) seem to have the same function/digital mapping but different CPU connections
#define PIN_22B (1<<26)		//PB26
#define PIN_23A (1<<14)		//PA14
#define PIN_24A (1<<15)		//PA15
#define PIN_25D (1<<0)		//PD0
#define PIN_26D (1<<1)		//PD1
#define PIN_27D (1<<2)		//PD2
#define PIN_28D (1<<3)		//PD3
#define PIN_29D (1<<6)		//PD6
#define PIN_30D (1<<9)		//PD9
#define PIN_31A (1<<7)		//PA7
#define PIN_32D (1<<10)		//PD10
#define PIN_33C (1<<1)		//PC1
#define PIN_34C (1<<2)		//PC2
#define PIN_35C (1<<3)		//PC3
#define PIN_36C (1<<4)		//PC4
#define PIN_37C (1<<5)		//PC5
#define PIN_38C (1<<6)		//PC6
#define PIN_39C (1<<7)		//PC7
#define PIN_40C (1<<8)		//PC8
#define PIN_41C (1<<9)		//PC9
#define PIN_42A (1<<19)		//PA19
#define PIN_43A (1<<20)		//PA20
#define PIN_44C (1<<19)		//PC19
#define PIN_45C (1<<18)		//PC18
#define PIN_46C (1<<17)		//PC17
#define PIN_47C (1<<16)		//PC16
#define PIN_48C (1<<15)		//PC15
#define PIN_49C (1<<14)		//PC14
#define PIN_50C (1<<13)		//PC13
#define PIN_51C (1<<12)		//PC12
#define PIN_52B (1<<21)		//PB21
#define PIN_53B (1<<14)		//PB14

/*
********SPI HEADER PINS********

 Holding the Due flat, so power jack is on the top left, digital pins closest to you
 The SPI header is a 2x3 (2 rows 3 Cols)


 _______________________________
 (+)	       	(+)
  ^				 ^
 power			reset button

_____________
SAM3X8E (MCU)
______________

   5	3	 1
  (*)  (*)  (*)		SPI
  (*)  (*)  (*)
   6    4    2

 ___________
 DIGITAL PINS
 __________
 ______________________________


 NOTE: COPI == MOSI (Controller out, Peripheral in)
 NOTE: CIPO == MISO (Controller in, Peripheral out)
 PIN 1 - CIPO - SPI0_CIPO - PA25 Periph. mode A
 PIN 2 - 5V OUT, don't use this? (it's part of the main 5v rail)
 PIN 3 - SCK - SPI0_SCK - PA27 Peiph. mode A - WKUP[10]
 PIN 4 - COPI - SPI0_COPI - PA26 Periph. mode A
 PIN 5 - RESET - NRSTb
 PIN 6 - GND
*/

//All port A periph mode A
#define SPI0_MOSI (1<<26)
#define SPI0_MISO (1<<25)
#define SPI0_SCK (1<<27)
//there are npcs1/2/3 on some port B pins but i dont have those right now
//i think that'll take a little bit of work
#define SPI0_NPCS0 (1 << 28)
#define SPI0_NPCS1 (1 << 29)
#define SPI0_NPCS2 (1 << 30)
//#define SPI0_NPCS3 (1 << 31) not wired on the due
#define SPI0_NSS (1 << 28) //NPCS0 becomes NSS when set as input and Due is SPI0 slave
const uint8_t spi0_cs_portA_pins[4] = {28, 29, 30, 31}; //map cs number to pin bit


//Special pins
//Some of these don't have digital or analog pins??
#define DAC0B (1<<15)		//PB15
#define DAC1B (1<<16)		//PB16 - also PWML0
#define PWML0 DAC1B
#define CANRX0A (1<<1)		//PA1
#define CANTX0A (1<<0)		//PA0
#define SCL1A (1<<18)		//PA18, this is the SCL closest to reset button - also TWCLK
#define SDA1A (1<<17)		//PA17, this is the SDA closest to reset button - also TWD
#define TX0A PIN_1A			//PA9, also digital pin 1
#define RX0A PIN_0A			//PA8, also digital pin 0

//PWM pins
#define PWMH0 PIN_0A
#define PWMH1 

//UART pins
#define URXD PIN_0A
#define UTXD PIN_1A

//USART pins
#define TX0 PIN_18A
#define RX0 PIN_19A

//There are outputs for the RNG
inline uint32_t get_RNG() { return REG_TRNG_ODATA; }
inline void init_TRNG() {
	pmc_enable_periph_clk(ID_TRNG);
	REG_TRNG_CR = (0x524e47 << 8) | 1;
}
inline void disable_TRNG() { 
	REG_TRNG_CR = (0x524e47 << 8); 
	pmc_disable_periph_clk(ID_TRNG);
}

//New functions to replace the old ones
//Enable (or disable) peripheral I/O access
inline void pio_enable_pio(Pio* pio, uint32_t pins) { pio->PIO_PER |= pins; }
inline void pio_disable_pio(Pio* pio, uint32_t pins) { pio->PIO_PDR |= pins; }
//Enable (or disable) output to pins
inline void pio_enable_output(Pio* pio, uint32_t pins) { pio->PIO_OER |= pins; }
inline void pio_disable_output(Pio* pio, uint32_t pins) { pio->PIO_ODR |= pins; }
//Enable (or disable) pullup resistors on pin
inline void pio_enable_pullup(Pio* pio, uint32_t pins) { pio->PIO_PUER |= pins; }
inline void pio_disable_pullup(Pio* pio, uint32_t pins) { pio->PIO_PUDR |= pins; }
//Enable (or disable) writing to the PIO_ODSR register
inline void pio_enable_output_write(Pio* pio, uint32_t pins) { pio->PIO_OWER |= pins; }
inline void pio_disable_output_write(Pio* pio, uint32_t pins) { pio->PIO_OWDR |= pins; }
//Write pins HIGH or LOW (not at once) - does not use ODSR
inline void pio_output_write_HIGH(Pio* pio, uint32_t pins) { pio->PIO_SODR |= pins; }
inline void pio_output_write_LOW(Pio* pio, uint32_t pins) { pio->PIO_CODR |= pins; }
//Write to entire bank using PIO_ODSR
inline void pio_output_write(Pio* pio, uint32_t pins) { pio->PIO_ODSR = pins; }
//Read state of PIO register
inline uint32_t pio_read_port(Pio* pio) { return pio->PIO_PDSR; }
//Set peripheral mode
inline void pio_set_periph_mode_A(Pio* pio, uint32_t pins) { pio->PIO_ABSR &= ~pins; }
inline void pio_set_periph_mode_B(Pio* pio, uint32_t pins) { pio->PIO_ABSR |= pins; }

//TRYING TO GET PWM TO WORK
//PWM info is on PDF page 970 in the datasheet
//The PWM clock must be enabled in the Power Management Controller (PWM) before using the PWM
//The PWM clock is the frequency of the master clock divided by a scaler
//Each channel has its own 16-bit counter (8 channels total)

static inline void delayMicros(uint32_t usec) {
	/*
	 * Based on Paul Stoffregen's implementation
	 * for Teensy 3.0 (http://www.pjrc.com/)
	 */
	if (usec == 0) return;
	uint32_t n = usec * (84000000 / 3000000);
	asm volatile(
		"L_%=_delayMicroseconds:"       "\n\t"
		"subs   %0, #1"                 "\n\t"
		"bne    L_%=_delayMicroseconds" "\n"
		: "+r" (n) :
		);
}

//PDF page 1003 has a list of the registers used for the PWM
//(might not be all, too tired to read at the moment of writing this)
//At least 4 registers per channel, 8 channels, lots of registers, this'll take a while

//Functions to Enable/disable PWM in the PWM status register, and read status
//This enables/disables output of the PWM on the channels
//PDF page 1007
inline void enable_pwm_ch0() { REG_PWM_ENA |= (1 << 0); }
inline void enable_pwm_ch1() { REG_PWM_ENA |= (1 << 1); }
inline void enable_pwm_ch2() { REG_PWM_ENA |= (1 << 2); }
inline void enable_pwm_ch3() { REG_PWM_ENA |= (1 << 3); }
inline void enable_pwm_ch4() { REG_PWM_ENA |= (1 << 4); }
inline void enable_pwm_ch5() { REG_PWM_ENA |= (1 << 5); }
inline void enable_pwm_ch6() { REG_PWM_ENA |= (1 << 6); }
inline void enable_pwm_ch7() { REG_PWM_ENA |= (1 << 7); }
inline void disable_pwm_ch0() { REG_PWM_DIS |= (1 << 0); }
inline void disable_pwm_ch1() { REG_PWM_DIS |= (1 << 1); }
inline void disable_pwm_ch2() { REG_PWM_DIS |= (1 << 2); }
inline void disable_pwm_ch3() { REG_PWM_DIS |= (1 << 3); }
inline void disable_pwm_ch4() { REG_PWM_DIS |= (1 << 4); }
inline void disable_pwm_ch5() { REG_PWM_DIS |= (1 << 5); }
inline void disable_pwm_ch6() { REG_PWM_DIS |= (1 << 6); }
inline void disable_pwm_ch7() { REG_PWM_DIS |= (1 << 7); }
inline void set_enable_pwm(uint32_t port) { REG_PWM_ENA = port; }
inline void set_disable_pwm(uint32_t port) { REG_PWM_DIS = port; }
inline uint32_t read_pwm_status() { return REG_PWM_SR; }

//PWM clock register
//Info on pdf page 1006
//DIVA/B in PWM_CLK register determines on/off and divider values
//DIVA/B = 0 - CLKA/B is OFF
//DIVA/B = 1 - CLKA/B determined by PREA/B
//DIVA/B = 2-255 - CLKA determined by PREA/B divided by DIVA/B
//These are read as "PWM prescaler = Masterclock / [factor]"
#define PWM_MCK_1 0b0000
#define PWM_MCK_2 0b0001
#define PWM_MCK_4 0b0010
#define PWM_MCK_8 0b0011
#define PWM_MCK_16 0b0100
#define PWM_MCK_32 0b0101
#define PWM_MCK_64 0b0110
#define PWM_MCK_128 0b0111
#define PWM_MCK_256 0b1000
#define PWM_MCK_512 0b1001
#define PWM_MCK_1024 0b1010
//These are used in the PWM_CMR (channel mode register)
#define PWM_CLKA 0b1011
#define PWM_CLKB 0b1100
//Anything other than these values are reserved
inline void set_pwm_prescalerA(uint8_t prescaler) {
	REG_PWM_CLK &= 0xFFFFF0FF;
	REG_PWM_CLK |= prescaler << 8;
}
inline void set_pwm_prescalerB(uint8_t prescaler) {
	REG_PWM_CLK &= 0xF0FFFFFF;
	REG_PWM_CLK |= prescaler << 24;
}
//div_factor needs to be >0 for clock to be turned ON
//Value needs to be 0-255
inline void set_pwm_div_factorA(uint8_t div_factor) {
	REG_PWM_CLK &= 0xFFFFFF00;
	REG_PWM_CLK |= div_factor;
}
inline void set_pwm_div_factorB(uint8_t div_factor) {
	REG_PWM_CLK &= 0xFF00FFFF;
	REG_PWM_CLK |= div_factor << 16;
}
//These set DIVA/B in the PWM_CLK register to 0
inline void stop_pwm_clkA() { REG_PWM_CLK &= 0xFF00FFFF; }
inline void stop_pwm_clkB() { REG_PWM_CLK &= 0xFFFFFF00; }

//Comparison values info is on pdf page 1041
inline void enable_comp_match_A() { REG_PWM_CMPM0 |= 1; }

//This is on PDF page 1044
inline void pwm_ch2_mode(uint32_t port) {
	REG_PWM_CMR2 = port;
}
//There are update registers as well, that act as buffers
//This is like the upper value, when the counter resets
//PDF page 1048
inline void pwm_ch2_period(uint8_t period) {
	REG_PWM_CPRD2 = period;
}
//This is like the compare match value
//PDF page 1046
inline void pwm_ch2_duty_cycle(uint8_t duty) {
	REG_PWM_CDTY2 = duty;
}

//PWM interrupts
//PDF page 1019
inline void pwm_ier2(uint32_t port) {
	REG_PWM_IER2 = port;
}
inline void pwm_idr2(uint32_t port) {
	REG_PWM_IDR2 = port;
}


/*
UART functions
Starts on page 750 of the datasheet PDF
*/

//Enables RX and TX
inline void uart_enable_rx_tx() { REG_UART_CR |= (1 << 6) | (1 << 4); }
inline void uart_enable_rx() { REG_UART_CR |= (1 << 4); }
inline void uart_disable_rx() { REG_UART_CR &= ~(1 << 5); }
//Baud rate = MCK/(16*CD)
inline void uart_set_clk_div(uint16_t div) { REG_UART_BRGR = div; }


//Different settings for the parity bit
#define DUE_UART_EVEN_PARITY 0
#define DUE_UART_ODD_PARITY 1
#define DUE_UART_SPACE_PARITY 2
#define DUE_UART_MARK_PARITY 3
#define DUE_UART_NO_PARITY 4

//Different settings for the channel mode, specifically for testing
#define DUE_UART_NORMAL 0			//UART runs normally
#define DUE_UART_AUTO 1				//UART auto triggers
#define DUE_UART_LOCAL_LOOPBACK 2	//look it up
#define DUE_UART_REMOTE_LOOPBACK 3	//look it up
inline void uart_set_parity_ch_mode(uint8_t ch_mode = DUE_UART_NORMAL, uint8_t parity = DUE_UART_NO_PARITY) {
	REG_UART_MR = 0;
	REG_UART_MR |= (parity << 9) | (ch_mode << 14);
}

inline uint32_t uart_read_rx() { return REG_UART_RHR; }
inline void uart_write_tx(uint8_t tx_data) { REG_UART_THR = tx_data; }



/*
TWI using SDA and SCL closest to the reset button
TWD0 - TWI data0 PA17   --- periph A
TWCK0 - TWI clock0 PA18   --- periph A
//Note: the arduino calls them "SDA1" and "SCLK1" even though they are "0"
*/


inline void twi0_set_device_addr(uint8_t addr) {
	REG_TWI0_MMR &= ~(0b1111111 << 16);
	REG_TWI0_MMR |= addr << 16;
}

inline void twi0_setup(uint8_t slave_addr) {
	pio_disable_pullup(PIOA, SDA1A | SCL1A);
	pio_enable_output(PIOA, SDA1A | SCL1A);
	pio_set_periph_mode_A(PIOA, SDA1A | SCL1A);
	pio_disable_pio(PIOA, SDA1A | SCL1A);
	twi0_set_device_addr(slave_addr);
	REG_TWI0_CR = (1 << 2) | (1 << 5);
	REG_TWI0_CWGR = 68 | (30 << 8) | (1 << 16);
}

//0 - write, 1 - read
inline void twi0_mread(uint8_t mread) {
	REG_TWI0_MMR &= ~(1 << 12);
	REG_TWI0_MMR |= (mread << 12);
}


/*********/
//External memory bus
/*
I can really only use 8 bits because... arduino
D0-D7 - Pins 34-41 ALL PERIPH MODE A
THE PINOUT DIAGRAM SAYS 33-40 - THIS IS INCORRECT (It has been fixed in the new version don't worry)
The pinout diagram on the Arduino website is incorrect, follow the datasheet
NWE - Write enable Pin 45
NRD - Output (read) enable
NCS[x] - Chip select
NCS0 - Analog pin 4 PA6 - periph mode B

NWE goes to the lcd's WR pin (this is arduino pin 45)
NCS0 goes to the lcd's RS pin (this is PA6 which is analog pin 4)
*/

//Set the RS (D/CX) pin LOW to send command
//Set the RS (D/CX) pin HIGH to send data
//Commands for the TFT
//#define fuckarduino
#ifdef fuckarduino
const uint32_t col_addr_set = 0x2A;
const uint32_t page_addr_set = 0x2B;
const uint32_t mem_write = 0x2C;
const uint32_t set_brightness_cmd = 0x51;
uint8_t* const data = (uint8_t*)0x61000000;	//sets NCS0 HIGH for data
uint8_t* const command = (uint8_t*)0x60000000; //sets NCS0 LOW for cmd
const uint16_t tft_debug_newline_height = 311;	//amount of rows to scroll for a new line when debugging
uint16_t tft_vscroll_val = 0;
char tft_debug_buff[17];
bool two_byte_per_pix_mode = false;


uint32_t spi0_rx_buffer;
uint32_t spi0_tx_buffer;

uint16_t lcd_width = 239;
uint16_t lcd_height = 319;
#else
extern volatile const uint32_t col_addr_set;
extern volatile const uint32_t page_addr_set;
extern volatile const uint32_t mem_write;
extern volatile const uint32_t set_brightness_cmd;
extern volatile uint8_t* const data;	//sets NCS0 HIGH for data
extern volatile uint8_t* const command; //sets NCS0 LOW for cmd
extern volatile uint16_t lcd_height;
extern volatile uint16_t lcd_width;
extern volatile bool two_byte_per_pix_mode;
extern volatile const uint16_t tft_debug_newline_height;	//amount of rows to scroll for a new line when debugging
extern volatile uint16_t tft_vscroll_val;
extern volatile char tft_debug_buff[17];
#endif



/*
IMPORTANT NOTES:
Apparently the DMAC's max BTZISE is 4095 https://arduino.stackexchange.com/questions/19615/due-dmac-btsize-trimmed-modulo-8192
Also, for linked lists to work, either SADDR or DADDR need to be fetched
It makes sense that DADDR is fetched, because it's always constant
*/

inline void smc_tft_lcd_init_pins() {
	pio_disable_pullup(PIOA, PIN_A4A | PIN_4A);	//NCS0 and NRD
	pio_enable_output(PIOA, PIN_A4A | PIN_4A);
	pio_disable_pio(PIOA, PIN_A4A | PIN_4A);
	pio_set_periph_mode_B(PIOA, PIN_A4A | PIN_4A);
	//D[7:0] (pins 34-41) and NWE (pin45) (write enable)
#ifdef SAM_16_BIT_LCD_SMC
	pio_disable_pullup(PIOC, 0x3fffc | PIN_45C);
	pio_enable_output(PIOC, 0x3fffc | PIN_45C);
	pio_disable_pio(PIOC, 0x3fffc | PIN_45C);
	pio_set_periph_mode_A(PIOC, 0x3fffc | PIN_45C);
#else
	pio_disable_pullup(PIOC, PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_41C | PIN_45C);
	pio_enable_output(PIOC, PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_41C | PIN_45C);
	pio_disable_pio(PIOC, PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_41C | PIN_45C);
	pio_set_periph_mode_A(PIOC, PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_41C | PIN_45C);
#endif
}

inline void smc_tft_lcd_setup() {
#ifdef SAM_16_BIT_LCD_SMC
	REG_SMC_MODE0 = 1 | (1 << 1) | (1 << 8) | (1 << 12);
	REG_SMC_MODE1 = 1 | (1 << 1) | (1 << 8) | (1 << 12);

	// SETUP: Put data on the bus, then wait 2 clocks (24ns) for ringing to settle
	REG_SMC_SETUP0 = 2 | (2 << 8) | (2 << 16) | (2 << 24);
	REG_SMC_SETUP1 = 2 | (2 << 8) | (2 << 16) | (2 << 24);

	// PULSE: Drop the WR strobe for 4 clocks (48ns) to lock in the clean data
	REG_SMC_PULSE0 = 4 | (4 << 8) | (4 << 16) | (4 << 24);
	REG_SMC_PULSE1 = 4 | (4 << 8) | (4 << 16) | (4 << 24);

	// CYCLE: Total cycle is 8 clocks. This leaves 2 clocks of HOLD time at the end!
	REG_SMC_CYCLE0 = 8 | (8 << 16);
	REG_SMC_CYCLE1 = 8 | (8 << 16);
#else
	REG_SMC_MODE0 = (1 << 1);
	REG_SMC_PULSE0 = 1 | (1 << 8);
	REG_SMC_CYCLE0 = 2;

	// SMC1 - data, read/write
	REG_SMC_MODE1 = 1 | (1 << 1);
	REG_SMC_PULSE1 = 1 | (1 << 8) | (15 << 16) | (15 << 24);
	REG_SMC_CYCLE1 = 2 | (30 << 16);
#endif
}

inline void lcd_set_columns(uint16_t start, uint16_t end) {
	//This sends 1 byte to specify command and 4 bytes of data (2 for start, 2 for end)
	*command = col_addr_set;
	*data = (start >> 8);
	*data = start;
	*data = (end >> 8);
	*data = end;
}

//The values for pages goes from 0-319, and anything over 255 is over 1 byte, so this needs 2 bytes
inline void lcd_set_pages(uint16_t start, uint16_t end) {
	*command = page_addr_set;
	*data = (start >> 8);
	*data = start;
	*data = (end >> 8);
	*data = end;
}
inline void lcd_clear() {
	*command = mem_write;
	if(two_byte_per_pix_mode) {
		for (uint32_t i = 0; i < 320*240; ++i) {
#ifdef SAM_16_BIT_LCD_SMC
			*reinterpret_cast<uint16_t*>(data) = 0x00;
#else
			*data = 0x00;
			*data = 0x00;
#endif
		}
		return;
	}
	else {
		for (uint32_t i = 0; i < 320 * 240; ++i) {
			*data = 0x00;
			*data = 0x00;
			*data = 0x00;
		}
	}
}

inline void lcd_fill(uint8_t r, uint8_t g, uint8_t b) {
	*command = mem_write;
	if(two_byte_per_pix_mode) {
		for (uint32_t i = 0; i < 76800; ++i) {
			*data = (g & 0b11100000) | (r >> 3);
			*data = b >> 3 | (g << 3);
		}
		return;
	}
	else {
		for (uint32_t i = 0; i < 76800; ++i) {
			*data = b;
			*data = g;
			*data = r;
		}
	}
}

inline void tft_display_off() { *command = 0x28; }
inline void tft_display_on() {
	*command = 0x29;
	delayMicros(5000);
}

inline void init_lcd_smc_dma() {
	//all of the settings are set with the linked lists so this function isnt really necessary i guess
	REG_DMAC_EN = 1;
}

//My TFT's MADCTL B4 = 0
/*
	enables the periph clocks
	sets up the SMC pins
	starts the display and clears it
	should be good to go after this
*/
inline void init_tft(bool horizontal = false, bool two_byte_per_pix = false) {
	pmc_enable_periph_clk(ID_SMC);
	pmc_enable_periph_clk(ID_DMAC);
	smc_tft_lcd_setup();
	smc_tft_lcd_init_pins();
	*command = 0x01;	//software reset, which doesn't really do anything
	delayMicros(5000);
	*command = 0x11;	//sleep out
	delayMicros(5000);
	if (horizontal) {
		*command = 0x36; //madctl access
		*data = (0b101 << 5); //flip x and y, and mirror y
		lcd_set_columns(0, 319);
		lcd_set_pages(0, 239);
	}
	else {
		lcd_set_columns(0, 239);
		lcd_set_pages(0, 319);
	}
	if(two_byte_per_pix) {
		*command = 0x3A;
		*data = 0x55; //16 bits per pixel
		two_byte_per_pix_mode = true;
	}
	delayMicros(1000);
	//refresh rate, default is 70hz
	//*command = 0xb1;
	//*data = 0x00;
	//*data = 0x10;	//~119Hz refresh rate
	//*data = 0x13;	//~100Hz
	//*data = 0x15;	//~90Hz
	//*data = 0x1f; //~61Hz
	init_lcd_smc_dma();
	lcd_clear();
	tft_display_on();
}

inline void dmac_wait_for_done() { while (REG_DMAC_CHSR & 0b11) {} }

inline void tft_vscroll_def(uint16_t tfa = 0, uint16_t vsa = 320, uint16_t bfa = 0) {
	/*
	The default args set the entire screen up for scrolling
	TFA - Top fixed area - number of rows from the top that shouldn't be affected by the scroll
	VSA - Vertical scroll area - how many rows will be scrolled
	BFA - Bottom fixed area - rows at the bottom that won't be affected by the scroll
	NOTE: Behavior is only defined if TFA + VSA + BFA = 320
	*/
	*command = 0x33; //vertical scrolling definition
	*data = tfa >> 8;
	*data = tfa;
	*data = vsa >> 8;
	*data = vsa;
	*data = bfa >> 8;
	*data = bfa;
}

inline void tft_partial_mode() {
	/*Turns on partial mode
	NOTE: set the partial area before turing on partial mode
	*/
	*command = 0x12;
}

inline void tft_partial_area(uint16_t start_row, uint16_t end_row) {
	/*
	start row - what row number to start at
	end row - what row number to end at
	If start row < end row, it'll behave as expected (partial area is subsection of display)
	if start row > end row (end row is above start row) then the partial area will wrap around
	if start row == end row, then the partial area is one row
	*/
	*command = 0x30;
	*data = start_row >> 8;
	*data = start_row;
	*data = end_row >> 8;
	*data = end_row;
}

inline void tft_normal_mode() {
	/*Turns partial mode off
	NOTE: To turn off partial mode:
		1. Turn display OFF
		2. Turn on normal mode (turn off partial mode)
		3. Clear display (part of display RAM could be garbage)
		4. Turn display ON
	*/
	tft_display_off();
	*command = 0x13;
	lcd_clear();
	tft_display_on();
}

inline void tft_vscroll(uint16_t lines) {
	/*
	THE SCREEN SCROLLS UP
	The number of lines to v_scroll is fixed/relative to 0:
		v_scroll(10) then v_scroll(13) only scrolls 3 row, NOT 23
		use v_scroll(0) to return display to normal
	*/
	*command = 0x37;
	*data = lines >> 8;
	*data = lines;
}

inline void tft_debugger_init() {
	init_tft();
	tft_debug_buff[16] = '\n';
	tft_vscroll_def();
	lcd_set_columns(0, 239);
	lcd_set_pages(0, 319);
}

/*
inline void tft_debug_letters_test() {
	uint16_t s_col = 2;
	uint16_t e_col = 8;
	uint16_t s_pages = 2;
	lcd_set_pages(s_pages, 319);
	s_pages = 311;
	for (uint8_t k = 0; k < 4; ++k) {
		for (uint8_t i = 0; i < 26; ++i) {
			const uint8_t* l_ptr = d_letters[i];
			lcd_set_columns(s_col, e_col);
			s_col += 7;
			e_col += 7;
			*command = mem_write;
			for (uint8_t j = 0; j < d_size; ++j) {
				*data = color_palette[l_ptr[j]][0];
				*data = color_palette[l_ptr[j]][1];
				*data = color_palette[l_ptr[j]][2];
			}
		}
		s_col = 2;
		e_col = 8;
		tft_vscroll_val += tft_debug_newline_height;
		tft_vscroll_val %= 320;
		tft_vscroll(tft_vscroll_val);
		s_pages -= 8;
		lcd_set_pages(s_pages, 319);
		delayMicros(0xFFFF);
	}
	while (1) {}
}
*/


/*
Using chip select 0 on digital pin 10
Calculation for SPI0 clk freq is: clk_freq = MCLK/BAUD_DIV
ex: 84MHz / 8 = 10.5MHz SPI clock freq
16 bits per transfer, data on rising edge
*/
extern volatile uint32_t spi0_rx_buffer;
extern volatile uint32_t spi0_tx_buffer;

inline void spi0_cs_rise_after_tx(bool rise) {
	if (rise)
		REG_SPI0_CSR |= (1 << 2) | (1 << 3);
	else
		REG_SPI0_CSR = REG_SPI0_CSR & ~((1 << 2) | (1 << 3));
}

//spi clock is mck/baud div
//remember to enable the periph clock
inline void spi0_init_master(uint8_t baud_div, uint8_t cs_num = 0) {
	uint32_t cs_pin = (1 << spi0_cs_portA_pins[cs_num]); //to do: make sure this works
	
	pio_enable_output(PIOA, SPI0_MOSI | SPI0_SCK | SPI0_NPCS0);
	pio_disable_output(PIOA, SPI0_MISO);
	pio_disable_pullup(PIOA, SPI0_MOSI | SPI0_MISO | SPI0_SCK | SPI0_NPCS0);
	pio_disable_pio(PIOA, SPI0_MOSI | SPI0_MISO | SPI0_SCK | SPI0_NPCS0);
	pio_set_periph_mode_A(PIOA, SPI0_MOSI | SPI0_MISO | SPI0_SCK | SPI0_NPCS0);
	//8 gives 10.5MHz clock, 9 gives 9.333MHz clock
	//chip select does not rise systematically until a new transfer is requested on a different chip select
	REG_SPI0_CSR = (1 << 1) | (1 << 3) | (0 << 4) | (baud_div << 8); //data on rising edge, 8 bits/xfer
	REG_SPI0_MR = 1 | (1 << 4); //master mode, fixed peripheral select (NPCS0), no decode/fault/delay
	REG_SPI0_CR = 1; //enable spi
}

inline void spi0_set_baud_rate_div(uint8_t baud_div) {
	REG_SPI0_CSR = ((REG_SPI0_CSR & ~0xff00) | (baud_div << 8));
}

inline void spi0_init_gbc_display(uint8_t baud_div = 1) {
	pio_enable_output(PIOA, SPI0_MOSI | SPI0_SCK | SPI0_NPCS0);
	pio_disable_output(PIOA, SPI0_MISO);
	pio_disable_pullup(PIOA, SPI0_MOSI | SPI0_MISO | SPI0_SCK | SPI0_NPCS0);
	pio_disable_pio(PIOA, SPI0_MOSI | SPI0_MISO | SPI0_SCK | SPI0_NPCS0);
	pio_set_periph_mode_A(PIOA, SPI0_MOSI | SPI0_MISO | SPI0_SCK | SPI0_NPCS0);
	//max baud rate because our display can ball
	//DC - HIGH = DATA, LOW = COMMAND
	//CS0 for command, CS1 for data
	//Reset - pull low to reset
	//CS - Active low - tied to ground
	REG_SPI0_CSR = (1 << 1) | (1 << 3) | (baud_div << 8); //data on rising edge, 8 bits/xfer
	//I'm writing to SPI0_CSR1 myself
	*((uint32_t*)0x40008034) = (1 << 1) | (1 << 3) | (baud_div << 8);
	REG_SPI0_MR = 1 | (1 << 1) | (1 << 4); //master mode, variable peripheral select (CS0, 1), no decode/fault/delay
	REG_SPI0_CR = 1; //enable spi

}

//Using DMAC channel 0 - 8 byte FIFO
//CTRLB needs to be programmed, and A needs to be tweaked
inline void spi0_init_master_dmac(uint8_t dmac_channel_number = 0) {
	//SPI0 TX interface number is 1
	REG_DMAC_EBCIDR = ~0;
	REG_DMAC_CHDR = ~0;
	REG_DMAC_CTRLA0 = (1 << 25) | (1 << 29);
	REG_DMAC_DADDR0 = 0x4000800CU;
	REG_DMAC_CFG0 = (1 << 4) | (1 << 13);
	//REG_DMAC_EN |= 1;
}

inline void spi0_set_variable_cs(bool enable) {
	uint32_t SPI_MR = REG_SPI0_MR;
	if (enable) {
		SPI_MR |= (1 << 1);
	}
	else {
		SPI_MR &= ~(1 << 1);
	}
	REG_SPI0_MR = SPI_MR;
}

inline void spi0_init_slave(uint8_t baud_div) {
	pio_enable_output(PIOA, SPI0_MISO);
	pio_disable_output(PIOA, SPI0_MOSI | SPI0_NSS);
	pio_disable_pullup(PIOA, SPI0_MOSI | SPI0_MISO | SPI0_SCK | SPI0_NSS);
	pio_disable_pio(PIOA, SPI0_MOSI | SPI0_MISO | SPI0_SCK | SPI0_NSS);
	pio_set_periph_mode_A(PIOA, SPI0_MOSI | SPI0_MISO | SPI0_SCK | SPI0_NSS);
	//8 gives 10.5MHz clock, 9 gives 9.333MHz clock
	REG_SPI0_CSR = (1 << 1) | (1 << 2) | (1 << 3) | (baud_div << 8); //data on rising edge, 16 bits/xfer
	REG_SPI0_MR = (1 << 4); //slave mode, fixed peripheral select (NPCS0), no decode/fault/delay
	REG_SPI0_CR = 1; //enable spi
}

//Using DMAC channel 1 - 8 byte FIFO
//CTRLB needs to be programmed, and A needs to be tweaked
inline void spi0_init_slave_dmac() {
	//SPI0 RX interface number is 2
	REG_DMAC_EBCIDR |= (1 << 1) | (1 << 9) | (1 << 17);
	REG_DMAC_CHDR |= (1 << 1);
	//REG_DMAC_CTRLA1 = 1 | (1 << 25);
	REG_DMAC_SADDR1 = 0x40008008U;
	REG_DMAC_CFG1 = (1 << 1) | (1 << 9);
	REG_DMAC_EN |= 1;
}

inline void spi0_npcs0_enable_manual_control(bool en = true) {
	if (en) {
		pmc_enable_periph_clk(ID_PIOA);
		pio_disable_pullup(PIOA, SPI0_NPCS0);
		pio_enable_output(PIOA, SPI0_NPCS0);
		pio_enable_pio(PIOA, SPI0_NPCS0);
	}
	else {
		pio_disable_pio(PIOA, SPI0_NPCS0);
		pio_set_periph_mode_A(PIOA, SPI0_NPCS0);
		pio_enable_output(PIOA, SPI0_NPCS0);
		pio_disable_pullup(PIOA, SPI0_NPCS0);
	}
}


inline void spi0_npcs0_low() {
	pio_output_write_LOW(PIOA, SPI0_NPCS0);
}

inline void spi0_npcs0_high() {
	pio_output_write_HIGH(PIOA, SPI0_NPCS0);
}

//this can be used to write to SPI1,2... (the different chip selects)
//this used to be for only SPI0 and I don't want to break anything by renaming it
inline void spi0_write_tx_xfer(uint16_t data) {
	REG_SPI0_TDR = data;
}

//same as the function above, but for... variable chip select
inline void spi_variable_cs_write_tx_xfer(uint16_t data, uint8_t pcs) {
	REG_SPI0_TDR = data | ((pcs & 0xf) << 16);
}

//this only works if variable chip select is active, default to CS0 for SPI0
inline void spi0_write_last_tx_xfer(uint16_t data, uint8_t pcs = 0) {
	REG_SPI0_TDR = data | ((pcs & 0xf) << 16) | (1 << 24);
}

inline void spi0_wait_tx_xfer() {
	//Pauses while the data in the transmit register has not been sent to serializer
	while (!(REG_SPI0_SR & (1 << 9))) {}
}

inline void spi0_wait_rx_xfer() {
	//Pauses while data has not been received in the RDR since last time it was read
	while (!(REG_SPI0_SR & 1)) {}
}

extern uint32_t led_matrix_buff0[16];
extern uint8_t crnt_digit;
extern uint8_t matrix_prev_digit;

inline void update_dot(uint32_t current_data) {
	//x has offset 16
	//y has offset 24
	uint8_t x = static_cast<int8_t>(__RBIT((current_data >> 16) & 0xFF)) + 128; //digit
	uint8_t y = static_cast<int8_t>(__RBIT((current_data >> 24) & 0xFF)) + 128; //segment
	x >>= 5; //get number between 0 and 8
	y >>= 5;
	led_matrix_buff0[matrix_prev_digit] &= 0xFF00;
	led_matrix_buff0[x] |= (1 << y);
	matrix_prev_digit = x;
}

inline void scroll_dot() {
	uint8_t dot = led_matrix_buff0[crnt_digit] & 0xFF;
	dot <<= 1;
	if (!dot) {
		led_matrix_buff0[crnt_digit] &= 0xFFFFFF00;
		++crnt_digit;
		if (crnt_digit > 15)
			crnt_digit = 0;
		led_matrix_buff0[crnt_digit] |= 1;
	}
	else
		led_matrix_buff0[crnt_digit] = (led_matrix_buff0[crnt_digit] & 0xFFFFFF00) | dot;
}


inline void led_matrix_spi0_dma_init() {
	//using DMAC channel 2
	//SPI0 TX has DMA channel HW Interface number 1
	REG_DMAC_CHDR |= 1 << 2;
	REG_DMAC_SADDR2 = (uint32_t)led_matrix_buff0;
	REG_DMAC_DADDR2 = 0x4000800CU;
	REG_DMAC_CTRLA2 = 16 | (1 << 25) | (1 << 29);
	REG_DMAC_CTRLB2 = (1 << 16) | (1 << 20) | (1 << 21) | (0b10 << 28);
	REG_DMAC_CFG2 = (1 << 4) | (1 << 13);
	REG_DMAC_EN = 1;
}

inline void led_matrix_start_dma() {
	REG_DMAC_SADDR2 = (uint32_t)led_matrix_buff0;
	REG_DMAC_CTRLA2 = 16 | (1 << 25) | (1 << 29);
	REG_DMAC_CHER |= (1 << 2);
}

inline void led_matrix_wait_dmac() {
	while (REG_DMAC_CHSR & (1 << 2)) {}
}

inline void led_matrix_shutdown_mode(uint8_t mode = 1) {
	//0 - shutdown
	//1 - normal operation
	spi0_write_tx_xfer((0b1100 << 8) | mode);
	delayMicros(2);
	spi0_write_tx_xfer((1 << 16) | (0b1100 << 8) | mode);
	delayMicros(2);

}

inline void led_matrix_decode_mode(uint8_t decode = 0) {
	spi0_write_tx_xfer((0b1001 << 8) | decode);
	delayMicros(2);
	spi0_write_tx_xfer((1 << 16) | (0b1001 << 8) | decode);
	delayMicros(2);
}

inline void led_matrix_intensity(uint8_t intensity) {
	//only the 4 LSB matter
	//at 0, LED duty cycle is 1/32
	//every increase of 1 to intensity raises duty cycle by 2/32
	//(Every odd duty cycle between 1/32 and 31/32 inclusive)
	spi0_write_tx_xfer((0b1010 << 8) | intensity);
	delayMicros(2);
	spi0_write_tx_xfer((1 << 16) | (0b1010 << 8) | intensity);
	delayMicros(2);
}

inline void led_matrix_scan_limit(uint8_t limit = 7) {
	//0b000 - 0b111 (0-7) which segments to light up
	//The matrix needs all of them, preferably 
	spi0_write_tx_xfer((0b1011 << 8) | limit);
	delayMicros(2);
	spi0_write_tx_xfer((1 << 16) | (0b1011 << 8) | limit);
	delayMicros(2);
}

inline void led_matrix_test_mode(uint8_t mode = 0) {
	spi0_write_tx_xfer((0b1111 << 8) | mode);
	delayMicros(2);
	spi0_write_tx_xfer((1 << 16) | (0b1111 << 8) | mode);
	delayMicros(2);
}

inline void led_matrix_spi0_init(uint8_t baud_div, uint8_t intensity, uint8_t scan_limit = 7) {
	spi0_init_master(baud_div);
	led_matrix_test_mode(1);
	led_matrix_test_mode(0);
	led_matrix_intensity(intensity);
	led_matrix_decode_mode(0);
	led_matrix_scan_limit(scan_limit);
	led_matrix_shutdown_mode();
	for (uint8_t i = 1; i <= 8; ++i) {
		spi0_write_tx_xfer((i << 8) | 0);
		delayMicros(2);
	}

	for (uint8_t i = 1; i <= 8; ++i) {
		spi0_write_tx_xfer((1 << 16) | (i << 8) | 0);
		delayMicros(2);
	}

}

#endif