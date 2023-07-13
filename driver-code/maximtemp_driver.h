#pragma once

#define MAXIMTEMP_SPI_BUFFER_SIZE 2

void maximtemp_convert_to_micros(
    unsigned char buffer[MAXIMTEMP_SPI_BUFFER_SIZE], int* integer,
    int* decimal) {

    short number = (short)((unsigned short)buffer[0] << 13) >> 5 | buffer[1];
    if (number < 0) {
        number = -number;
        *integer = -(number >> 3);
    } else {
        *integer = number >> 3;
    }
    *decimal = (unsigned int)(number & 0x7) * 125000;
}
