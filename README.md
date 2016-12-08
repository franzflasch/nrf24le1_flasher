nRF24LE1 Flasher
================

## Now using libmpsse for real hardware access!!!

This software is used to read, write or erase flash memory of Nordic nRF24LE1
chips. Product information can be found at [Nordic site][1].
If you are looking for a free SDK to programming that chip, you can find a good
one from Brennen at [his blog][2]

Right now, we are using a FTDI FT232R serial<->usb chip to send SPI commands to
nRF24LE1. But it's very simple to port to another platforms like Raspberry PI,
Beaglebone etc. Product information can be found at [ftdi site][3].


nRF24LE1 pinout
---------------

The pinout of this product it's very dependent of your manufacturer design, but
the common ones on chinese sites are the 24 pins format with pins like this:

|Left pins|  Right pins  |
|---------|--------------|
|   3v3   | P0.2 - FSCK  |
|   RESET | P0.3 - FMOSI |
|   RXEN  | P0.4 - FMISO |
|   P0.0  | P0.5 - FCSN  |
|   P0.1  | P0.6         |
|   PROG  | GND          |

The 3v3 is the pin 1 (normally have a square around it).

If you have another design or chip version, please make sure you are following
that pins (extracted from nRF24LE1 spec):

|       | 24pin-4x4 | 32pin-5x5 | 48pin-7x7 |
|-------|-----------|-----------|-----------|
| FCSN  |    P0.5   |    P1.1   |    P2.0   |
| FMISO |    P0.4   |    P1.0   |    P1.6   |
| FMOSI |    P0.3   |    P0.7   |    P1.5   |
| FSCK  |    P0.2   |    P0.5   |    P1.2   |


FTDI FT232R
-----------

With this chip, we have 8 "GPIOs" to do a mpsse hardware SPI implementation. You need
to check if your breakout can configure to 3v3 operation.
We hook each pin of nRF24LE1 on FT232R as described:

| nRF24LE1 | FT232R |
|----------|--------|
|   FCSN   |   AD3  |
|   FMISO  |   AD2  |
|   FMOSI  |   AD1  |
|   FSCK   |   AD0  |
|   RESET  |   AD4  |
|   PROG   |   AD5  |
|   GND    |   GND  |
|   3v3    |  VCCIO |

Dependencies
------------

- libftdi1 
- libusb-1.0
- libmpsse (https://github.com/franzflasch/libmpsse)


Usage
-----

First hook all pins on nRF24LE1 and FT232R, so plug on USB and type on terminal:

```
$ lsusb
```

It will list every FTDI device connected at your PC, something like:

```
Bus 001 Device 002: ID 0403:6014 Future Technology Devices International, Ltd FT232H Single HS USB-UART/FIFO IC
```

If you see more than one entry, please go to "Multiple FTDI Devices" section.

You need permission to access that USB device. You can become root, use sudo or
change permission of /dev/bus/usb/$BUS/$DEV specific.

The first task to do is a backup of your InfoPage!!!

```
# nrf24le1_flasher --read-ip ip_bkp.img
```

Just make sure that ip_bkp.img have 512 bytes, so you can save that file as your
personal backup.

Now to write a new program:

```
# nrf24le1_flasher --erase-all --write-flash my_program.hex --write-ip ip_bkp.img
```

This command clear all memory, writes your program and restore your InfoPage
that you saved before.

If you want to know other options, use the "--help" parameter.


Multiple FTDI Devices
---------------------

If you have more than one FTDI plugged, you need to choose which one to be used.
Type this command on terminal:

```
$ lsusb
```

It will list devices like:

```
Bus 001 Device 002: ID 0403:6014 Future Technology Devices International, Ltd FT232H Single HS USB-UART/FIFO IC
```

Now we know the vid (0x0403) and the pid (0x6014) of our FTDI device, so we can pass it
to nrf24le1_flasher: If there are more than one devices with this pid and vid you can also pass the index (here it is 0)

```
# nrf24le1_flasher -d 0x403-0x6014-0 --read-ip ip_bkp.img
```

Another programmers/flashers
----------------------------

There are some options over there, like a [kernel module][4] from Eder, a
[Raspberry PI version][5] from Derek and an [Arduino programmer][6] from Dean.


[1]: http://www.nordicsemi.com/eng/Products/2.4GHz-RF/nRF24LE1
[2]: http://blog.diyembedded.com/2010/06/nrf24le1-sdk-for-sdcc.html
[3]: http://www.ftdichip.com/Products/ICs/FT232R.htm
[4]: https://github.com/hltrd/nrf24le1
[5]: https://github.com/derekstavis/nrf24le1-libbcm2835
[6]: https://github.com/DeanCording/nRF24LE1_Programmer

