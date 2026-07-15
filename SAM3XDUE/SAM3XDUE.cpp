#include "SAM3XDUE.h"

uint8_t crnt_digit = 0;
uint8_t matrix_prev_digit = 3;

uint32_t led_matrix_buff0[16] = { (1 << 8) | 1 ,(2 << 8) ,(3 << 8),(4 << 8),(5 << 8),(6 << 8),(7 << 8),(8 << 8),
										(1 << 16) | (1 << 8),(1 << 16) | (2 << 8),(1 << 16) | (3 << 8),(1 << 16) | (4 << 8),
										(1 << 16) | (5 << 8),(1 << 16) | (6 << 8),(1 << 16) | (7 << 8),(1 << 16) | (8 << 8) };


volatile const uint32_t col_addr_set = 0x2A;
volatile const uint32_t page_addr_set = 0x2B;
volatile const uint32_t mem_write = 0x2C;
volatile const uint32_t set_brightness_cmd = 0x51;
volatile uint8_t* const data = (uint8_t*)0x61000000;	//sets NCS0 HIGH for data
volatile uint8_t* const command = (uint8_t*)0x60000000; //sets NCS0 LOW for cmd
volatile const uint16_t tft_debug_newline_height = 311;	//amount of rows to scroll for a new line when debugging
volatile uint16_t tft_vscroll_val = 0;
volatile char tft_debug_buff[17];
volatile bool two_byte_per_pix_mode = false;
volatile uint32_t spi0_rx_buffer;
volatile uint32_t spi0_tx_buffer;
volatile uint16_t lcd_width = 239;
volatile uint16_t lcd_height = 319;