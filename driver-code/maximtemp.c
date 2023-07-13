#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "maximtemp_driver.h"

#define println(format, ...) printf(format "\n" __VA_OPT__(, ) __VA_ARGS__)

#define eprintln(format, ...)                                                  \
    fprintf(stderr, format "\n" __VA_OPT__(, ) __VA_ARGS__)

#ifdef DEBUG
#define HERE                                                                   \
    println("Here is line %s:%d in file %s", __func__, __LINE__, __FILE__);
#else
#define HERE
#endif

#define assert_true(condition)                                                 \
    if (!(condition)) {                                                        \
        HERE;                                                                  \
        exit(EXIT_FAILURE);                                                    \
    }

void test_convert() {
    int integer;
    int decimal;
    unsigned char buffer[MAXIMTEMP_SPI_BUFFER_SIZE];

    buffer[0] = 0;
    buffer[1] = 80;
    maximtemp_convert_to_micros(buffer, &integer, &decimal);
    assert_true(integer == 10 && decimal == 0);

    buffer[0] = 3;
    buffer[1] = 232;
    maximtemp_convert_to_micros(buffer, &integer, &decimal);
    assert_true(integer == 125 && decimal == 0);

    buffer[0] = 1;
    buffer[1] = 202;
    maximtemp_convert_to_micros(buffer, &integer, &decimal);
    assert_true(integer == 57 && decimal == 250000);

    buffer[0] = 0b111;
    buffer[1] = 0b11001101;
    maximtemp_convert_to_micros(buffer, &integer, &decimal);
    assert_true(integer == -6 && decimal == 375000);
}

int main() {
    test_convert();

    int fd = open("/sys/bus/iio/devices/iio:device0/in_temp_input", O_RDONLY);
    if (fd < 0) {
        eprintln("Error on open(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    char buffer[64];
    int result = read(fd, buffer, sizeof(buffer) - 1);
    if (result < 0) {
        eprintln("Error on read(fd: %d): %s", fd, strerror(errno));
        exit(EXIT_FAILURE);
    }
    buffer[result] = '\0';

    println("%s", buffer);
    return EXIT_SUCCESS;
}
