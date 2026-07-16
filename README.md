# duelibs
I learned about the Due through its datasheet and not through the atmel HAL or any other formal training, and I wrote my own (worse) HAL for the features I was using. I do use the HAL for things involving the PMC and the NVIC, maybe other "system" things.

I don't think anyone would be crazy enough to actually use this, instead it is more so that my other published projects have all the code necessary to run. I started writing this a really long time ago and never revisited it because so many projects depended on it.


## Files
### Library Files
The rest of the files have somewhat descriptive names. These are mostly provided "as-is". Most of them have more functionality that I predicted I would want but never got around to using, but nothing was intensively stress-tested (some were used much more than others).

- ```SAM3XDUE``` - The main file that contains way too much. PIO control, ```delayMicros``` so you do not need to ```#include <Arduino.h>``` (saves space!), TFT LCD code using the Due's SMC, PWM code, some UART stuff, untested TWI stuff, 8x8 LED Matrix code, SPI0 stuff (which I stopped using after making ```due_spi```, but I never heavily used SPI)

- ```due_spi``` - simple SPI0 interface (not great on its own but fast enough)

- ```dueprinter``` - Simple UART/USART lib that supports non-blocking (via peripheral DMA) and blocking transactions. Only UART0 (the serial port associated with the USB programming port) was tested, but I don't see why the other peripherals wouldn't work. The USART ports have more functionality and this lib would use them in "UART" mode (I think...).

### Untested/Unfinished Library Files
- ```duedac.hpp``` - I spent maybe half an hour looking through the datasheet and throwing this together before I stopped and never got back to it.

- ```timeout``` - I honestly might have been drunk putting this together, I had something in mind but really stopped caring completely because I was never going to use this.

- ```SAM3XDUE_timers``` - literally empty lmao

- ```SAM3X_DMA``` - I don't really use this anymore, it's just a wrapper around the Due's DMAC linked list transfer items. Using the DMAC with linked lists require 5 fields and this was to make it less irritating.

### Misc Files
Any other files not mentioned are not important, probably from atmel (you can check for an atmel copyright, I left them intact of course).
- ```library.json``` - for platformio
- ```sam3x...``` - from the atmel libraries, pulled from the Arduino IDE. They are free to distrubute and their copyrights have not been tampered or removed


## Usage
Most of the libraries might require defining the interrupt handlers (void PERIPHERAL_Handler(void){}) to function. This is because many of my projects used those interrupts for other things and I personally do not mind defining interrupts myself over some other method.

This was not originally intented for use with platformio but I did a really quick swap from VisualMicro to platformio so some other things might be necessary but this is all I had to do:

You can include this library by adding this line to ```platformio.ini```:
   
    lib_deps = symlink://PATH_TO_LIB

Then you can ```#include``` whatever files you want.

- ```dueprinter``` - Make an instance of the ```DuePrinter``` class global. This has two modes for transfers: blocking and non-blocking. Blocking just uses a while loop, similar to ```Serial.print```. Non-blocking is interrupt-driven and requires the user to define the interrupt handler for the peripheral they are using (e.g., UART, USARTx) and place ```DuePrinter_instance.handle()``` within it. The user can set callbacks for when a message has finished sending/receiving with the ```set_read_callback``` and ```set_tx_callback``` functions. One additional message can be queued if one message is currently being sent/received.

## Limitations
A lot of this is either not tested or not tested thoroughly. It's all "as-is". For the projects that use it, it works well enough.
```due_spi``` - Only works with SPI0. I only wanted a quick wrapper for the SPI peripheral, only needed one instance, and didn't care about future-proofing (still don't tbh).

- ```dueprinter``` - The rx/tx callback functions were never used by me so they do not take arguments or return anything. Maybe making them take a variable number of arguments or a void pointer would be more flexible. Also, there can only be one callback. Also, it only supports queuing one additional message to read/write while one is transferring. If you try to queue one when the queue is already full, it will be dropped. Maybe if the ```queue``` functions returned a bool, at least then the caller can tell if the queue failed, but this was written without much care.

## Notes
Any of the libraries that use DMA/PDMA *must* have the buffers both within RAM and *must not* go out of scope while the transfer is running. The simplest thing to do is make the buffers static (either with ```static``` or put them in the global scope). Buffers can be local and non-static if you guarantee that the DMAC transfer will finish before it goes out of scope (maybe with a blocking function?).

The reason being is the Due's DMAC/PDC *do not* have connections to the massive interal flash so everything must live within its depressing amount of RAM.

## Personal Comments
This is *not* a finished product, it is more of a sendoff. I want my previous efforts to be witnessed and documented, but I may never come back to working on these.

This quickly turned into a dumping ground for initialization code I repeated across multiple projects, so it is something I appended to rather than actually fixed up. The code sucks, but its purpose was to get something working and contains a lot more than it should if it was an actual product. I was never consistent with how I wrote the code because they were written at various points in my life at different levels of exhaustion with fluctuating amounts of focus and care.

Some of the comments might be... unhinged. The ```#ifdef fuckarduino``` in ```SAM3XDUE``` came from a place of hate when I changed from C++whatever to C++23. I don't remember. That, and the entirety of ```SAM3XDUE.cpp```, are blurs.
