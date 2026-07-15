#include <dueprinter.hpp>

//the CR has offset 0 from the base
DueSerialPorts Due_USART0{ &REG_USART0_CR, PIOA, 10, 11, ID_USART0 };
DueSerialPorts Due_USART1{ &REG_USART1_CR, PIOA, 12, 13, ID_USART1 };
DueSerialPorts Due_USART2{ &REG_USART2_CR, PIOB, 21, 20, ID_USART2 };
DueSerialPorts Due_USART3{ &REG_USART3_CR, PIOD, 5, 4, ID_USART3 };
DueSerialPorts Due_UART{ &REG_UART_CR, PIOA, 8, 9, ID_UART };