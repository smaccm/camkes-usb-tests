/*
 * Copyright 2016, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the term of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(D61_BSD)
 */

#include <camkes.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef CONFIG_ARCH_X86
#define VID 0x03EB //Tardec
#define DID 0x0007 //Mobility
#else
#define VID 0x1d50 //Black Sphere Tech
#define DID 0x6018 //Black Magic Probe
#endif

extern void clear_fault(uintptr_t udev, uint16_t fault);
extern void set_status(uintptr_t udev, uint8_t status);
extern void set_vehicle_velocity(uintptr_t udev, uint16_t drive, uint16_t turn);

static inline void udelay(uint32_t us)
{
	volatile uint32_t i;
	for (; us > 0; us--) {
		for (i = 0; i < 1000; i++);
	}
}

int run()
{
	uintptr_t track = NULL;

	printf("Starting the Mobility\n");
	printf("-------------------\n");

	/* Find the device */
	while (!track) {
		track = cdc_find(VID, DID);
		udelay(5000);
	}
	printf("Found Mobility\n");

	/* Connect to the sub class driver */
	cdc_connect(track);

	cdc_configure(track, 115200, 1, 0, 8);

	/* Send Data */
	clear_fault(track, 0xFFFF);
	set_status(track, 4);

	printf("Let's move!!!\n");

	while (1) {
		udelay(2000);
		set_vehicle_velocity(track, 32768 + 100, 32768);
	}

	return 0;
}
