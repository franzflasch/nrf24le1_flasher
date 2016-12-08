#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mpsse.h>
#include "spi.h"

#define LOW	0
#define HIGH	1

// pins defs
//#define PIN_FCSN	(1 << 1)	/* RXD */	// nRF2LE1 Chip select
//#define PIN_FMISO	(1 << 2)	/* RTS */
//#define PIN_FMOSI	(1 << 3)	/* CTS */
//#define PIN_FSCK	(1 << 4)	/* DTR */
#define PIN_RESET	GPIOL0	/* DSR */	// nRF24LE1 Reset
#define PIN_PROG	GPIOL1 	/* DCD */	// nRF24LE1 Program

//#define BYTES_PER_BIT 3
//#define FTDI_READ_FIFO_SIZE 384
//#define delay_ns(t)
#define delay_us(t) usleep(t)
#define delay_ms(t) usleep(t * 1000)
#define delay_s(t) sleep(t)


static struct mpsse_context *nrf24le1 = NULL;

static uint8_t digitalWrite(int pin, int value)
{
	if (value)
		PinHigh(nrf24le1, pin);
	else
		PinLow(nrf24le1, pin);

	return 0;
}

static void prog_begin()
{
	digitalWrite(PIN_PROG, HIGH);
	digitalWrite(PIN_RESET, LOW);
	delay_us(1); // we need only 0.2 us
	digitalWrite(PIN_RESET, HIGH);
	delay_ms(2); // we need to wait at least 1.5 ms before send commands
}

static void prog_end()
{
	digitalWrite(PIN_PROG, LOW);
	digitalWrite(PIN_RESET, LOW);
	delay_us(1);
	digitalWrite(PIN_RESET, HIGH);
}

//Open(supported_devices[i].vid, supported_devices[i].pid, mode, freq, endianess, IFACE_A, NULL, NULL))

int spi_begin(uint16_t vid, uint16_t pid, uint16_t index)
{	

	if((vid != 0) && (pid != 0))
	{
		nrf24le1 = OpenIndex(vid, pid, SPI0, FOUR_HUNDRED_KHZ, MSB, IFACE_A, NULL, NULL, index);
		printf("Using VID PID BASIS!\n");
	}
	else
	{
		nrf24le1 = MPSSE(SPI0, FOUR_HUNDRED_KHZ, MSB);
		printf("USING std basis!\n");
	}

	if((nrf24le1 != NULL) && nrf24le1->open)
	{
		printf("%s initialized at %dHz (SPI mode 0)\n", GetDescription(nrf24le1), GetClock(nrf24le1));
	}
	else
	{
		printf("Failed to initialize MPSSE: %s\n", ErrorString(nrf24le1));
		return -1;
	}

	digitalWrite(PIN_PROG, LOW);
	digitalWrite(PIN_RESET, HIGH);

	prog_begin();

	return 0;
}

void spi_end()
{
	prog_end();

	Close(nrf24le1);
}

//#define SPI_DEBUG

int spi_transfer(uint8_t *bytes, size_t size)
{
	char *data = NULL;// *data1 = NULL;

	#ifdef SPI_DEBUG
	int i = 0;
	printf("Send:\n");
	for(i=0;i<size;i++)
	{
		printf("%hhx ", bytes[i]);
	}
	printf("\n\n");
	#endif

	Start(nrf24le1);
	data = Transfer(nrf24le1, (char *)bytes, size);
	Stop(nrf24le1);

	if(data)
	{
		#ifdef SPI_DEBUG
		printf("Read:\n");
		for(i=0;i<size;i++)
		{
			printf("%hhx ", data[i]);
		}
		printf("\n\n");
		#endif
		
		memcpy(bytes, data, size);
		free(data);
		return size;
	}	
	return 0;
}
