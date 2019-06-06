#include "mcp3008spi.h"

using namespace std;

int main()
{
    // Przetwornik A/C MCP3008 10-bitowy 8-kanałowy SPI - DIP
    // https://botland.com.pl/przetworniki/2358-przetwornik-ac-mcp3008-ip-10-bitowy-8-kanalowy-spi-dip.html?search_query=mcp3008&results=3
    // https://learn.adafruit.com/reading-a-analog-in-and-controlling-audio-volume-with-the-raspberry-pi/script
    // http://www.hertaville.com/interfacing-an-spi-adc-mcp3008-chip-to-the-raspberry-pi-using-c.html - NOT WORKING

    //MARKS: before using please enable SPI bus (raspi-config command)

    Mcp3008Spi adc( "/dev/spidev0.0" );

    if( adc.open() )
    {
        while( true )
        {
//            for( int i = 0; i < 8; ++i )
            for( int i = 0; i < 1; ++i )
            {
                double signal = adc.read( i );

                cout << "[Channel] " << i << ","
                     << " signal: " << signal << " %"
                     << " (" << ( 3.3 * signal / 100.0 ) << " V)"
                     << endl;
            }

            sleep( 1 );
        }

        adc.close();
    }

    return 0;
}
