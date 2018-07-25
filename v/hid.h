// See LICENSE for license details.

#ifndef HID_HEADER_H
#define HID_HEADER_H

#include <stdint.h>
#include <stddef.h>

// extern volatile uint64_t *const sd_base;
extern const size_t err, eth, ddr, rom, bram, intc, clin, hid ;

extern void hid_init(void);
extern void hid_console_putchar(unsigned char ch);
extern void hid_send_string(const char *str);
extern int myputchar(int ch);
extern void external_interrupt(void);

#endif
