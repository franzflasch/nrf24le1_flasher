#ifndef SPI_H
#define SPI_H

int spi_begin(uint16_t vid, uint16_t pid, uint16_t index);
void spi_end();
int spi_transfer(uint8_t *bytes, size_t size);


#endif // SPI_H
