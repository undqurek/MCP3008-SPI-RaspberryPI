#pragma once

#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <iostream>

class Mcp3008Spi
{
private:
    std::string device;
    uint8_t mode;
    uint8_t bpw; // bits per word
    uint32_t speed;

    int handle;

public:
    Mcp3008Spi( std::string device, uint8_t mode = SPI_MODE_0, uint8_t bpw = 8, uint32_t speed = 500000 );
    ~Mcp3008Spi();

    bool open();
    void close();

    /**
     * @brief read Reads signal strength in percent for indicated channel.
     * @param channel channel number in range from 0 to 7
     * @param differential differential measurement mode
     * @return signal strength in percent
     */
    double read( uint8_t channel, bool differential = false );

private:
    bool transfer( uint8_t *data, uint16_t length );
};
