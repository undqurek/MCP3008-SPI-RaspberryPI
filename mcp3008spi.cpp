#include "mcp3008spi.h"

using namespace std;


Mcp3008Spi::Mcp3008Spi( std::string device, uint8_t mode, uint8_t bpw, uint32_t speed )
{
    this->device = device;
    this->mode = mode;
    this->bpw = bpw;
    this->speed = speed;

    this->handle = -1;
}

Mcp3008Spi::~Mcp3008Spi()
{
    this->close();
}

bool Mcp3008Spi::open()
{
    if( this->handle > -1 )
        return false;

    this->handle = ::open( this->device.c_str(), O_RDWR );

    if( this->handle < 0 )
    {
        std::cerr << "Could not open SPI device." << std::endl;

        return false;
    }

    if( ::ioctl( this->handle, SPI_IOC_WR_MODE, &this->mode ) < 0 )
    {
        this->close();
        std::cerr << "Could not set SPI mode (ioctl write fail)." << std::endl;

        return false;
    }

    if( ::ioctl( this->handle, SPI_IOC_RD_MODE, &this->mode ) < 0 )
    {
        this->close();
        std::cerr << "Could not set SPI mode (ioctl read fail)." << std::endl;

        return false;
    }

    if( ::ioctl( this->handle, SPI_IOC_WR_BITS_PER_WORD, &this->bpw ) < 0 )
    {
        this->close();
        std::cerr << "Could not set SPI bits per word (ioctl write fail)." << std::endl;

        return false;
    }

    if( ::ioctl( this->handle, SPI_IOC_RD_BITS_PER_WORD, &this->bpw ) < 0 )
    {
        this->close();
        std::cerr << "Could not set SPI bits per word (ioctl read fail)." << std::endl;

        return false;
    }

    if( ::ioctl( this->handle, SPI_IOC_WR_MAX_SPEED_HZ, &this->speed ) < 0 )
    {
        this->close();
        std::cerr << "Could not set SPI speed (ioctl write fail)." << std::endl;

        return false;
    }

    if( ::ioctl( this->handle, SPI_IOC_RD_MAX_SPEED_HZ, &this->speed ) < 0 )
    {
        this->close();
        std::cerr << "Could not set SPI speed (ioctl read fail)." << std::endl;

        return false;
    }

    return true;
}

void Mcp3008Spi::close()
{
    if( this->handle == -1 )
        return;

    ::close( this->handle );
	
    this->handle = -1;
}

double Mcp3008Spi::read( uint8_t channel , bool differential )
{
    if( channel > 7 )
    {
        std::cerr << "Incorrect channel number (allowed 0-7)." << std::endl;

        return 0.0 / 0.0; // NaN
    }

    if( this->handle == -1 )
        return 0.0 / 0.0; // NaN

    uint8_t buffer[ 3 ];

    // http://www.hertaville.com/files/uploads/2013/07/gg4.png
    buffer[ 0 ] = 1;
    buffer[ 1 ] = ( differential ? 0 : ( 1 << 7 ) ) | ( channel << 4 );
    buffer[ 2 ] = 0;

    if( this->transfer( buffer, 3 ) )
    {
        uint16_t hi = ( buffer[ 1 ] << 8 ) & 0x0300;
        uint16_t lo = ( buffer[ 2 ] << 0 ) & 0x00FF;

        uint16_t value = hi | lo;

        return 100.0 * ( (double)value / 1023.0 );
    }

    return 0.0 / 0.0; // NaN
}

bool Mcp3008Spi::transfer( uint8_t *data, uint16_t length )
{
    if( this->handle == -1 )
        return false;

    struct ::spi_ioc_transfer pack;

    pack.tx_buf = (unsigned long)data; // transmit from "data"
    pack.tx_nbits = 8;
    pack.rx_buf = (unsigned long)data; // receive into "data"
    pack.rx_nbits = 8;
    pack.len = length;
    pack.delay_usecs = 0 ;
    pack.speed_hz = this->speed;
    pack.bits_per_word = this->bpw ;
    pack.cs_change = 0;

    if( ::ioctl( this->handle, SPI_IOC_MESSAGE( 1 ), &pack ) < 0 )
    {
        std::cerr << "SPI data transmission error." << std::endl;

        return false;
    }

    return true;
}
