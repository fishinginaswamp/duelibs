#pragma once
#ifndef _SAM3X8E_
#include <sam3x8e.h>
#endif

#include <SAM3XDUE.h>


struct Promise {
	bool value;
	bool failed;
};

volatile uint32_t timeout_fail_addr;
volatile bool timeout_failed = false;

inline Promise set_timeout() {
	timeout_failed = false;

timeout_exit:
	if (!timeout_failed) {
		return Promise{ false, true };

	}

	return Promise{ false, false };
}


void TC0_Handler(void) {
	timeout_failed = true;
	asm volatile(
		"mov r3,$1\n\t"
		"mov lr,r3\n\t"
		"bx lr"
		::"r"(timeout_fail_addr));
}
