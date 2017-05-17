/***************************************************
 This is our library for the Adafruit ILI9341 Breakout and Shield
 ----> http://www.adafruit.com/products/1651

 Check out the links above for our tutorials and wiring diagrams
 These displays use SPI to communicate, 4 or 5 pins are required to
 interface (RST is optional)
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries.
 MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_ILI9341.h"
#include "debug.h"
#include <string.h>

Adafruit_ILI9341::Adafruit_ILI9341(SPI* spi, GPIO_PIN* dcPin, GPIO_PIN* rstPin, GPIO_PIN* ssPin)
: Adafruit_GFX(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT)
{
    _spi = spi;
    _dcPin = dcPin;
    _rstPin = rstPin;
    _ssPin = ssPin;

    DR = _spi->GetDRAddr();
    SR = _spi->GetSRAddr();

    init();
}

void Adafruit_ILI9341::writecommand(uint8_t cmd)
{
    _dcPin->Reset();
    _ssPin->Reset();
    _spi->TransmitByte(cmd);
    _ssPin->Set();

}

void Adafruit_ILI9341::writedata(uint8_t data)
{
    _dcPin->Set();
    _ssPin->Reset();
    _spi->TransmitByte(data);
    _ssPin->Set();
}

uint8_t Adafruit_ILI9341::readdata()
{
    _dcPin->Set();
    _ssPin->Reset();
    uint8_t data = _spi->TransmitByte(0x0);
    _ssPin->Set();
    return data;
}


//#define DELAY 0x80


void Adafruit_ILI9341::init(void)
{
    if (_rstPin != 0)
    {
        _rstPin->Reset();
    }

    // toggle RST low to reset
    if (_rstPin != 0)
    {
        _rstPin->Set();
        delay(5);
        _rstPin->Reset();
        delay(20);
        _rstPin->Set();
        delay(120);
    }

    writecommand(0xEF);
    writedata(0x03);
    writedata(0x80);
    writedata(0x02);

    writecommand(0xCF);
    writedata(0x00);
    writedata(0XC1);
    writedata(0X30);

    writecommand(0xED);
    writedata(0x64);
    writedata(0x03);
    writedata(0X12);
    writedata(0X81);

    writecommand(0xE8);
    writedata(0x85);
    writedata(0x00);
    writedata(0x78);

    writecommand(0xCB);
    writedata(0x39);
    writedata(0x2C);
    writedata(0x00);
    writedata(0x34);
    writedata(0x02);

    writecommand(0xF7);
    writedata(0x20);

    writecommand(0xEA);
    writedata(0x00);
    writedata(0x00);

    writecommand(ILI9341_PWCTR1);    //Power control
    writedata(0x23);   //VRH[5:0]

    writecommand(ILI9341_PWCTR2);    //Power control
    writedata(0x10);   //SAP[2:0];BT[3:0]

    writecommand(ILI9341_VMCTR1);    //VCM control
    writedata(0x3e); //¶Ô±È¶Èµ÷½Ú
    writedata(0x28);

    writecommand(ILI9341_VMCTR2);    //VCM control2
    writedata(0x86);  //--

    writecommand(ILI9341_MADCTL);    // Memory Access Control
    writedata(0x48);

    writecommand(ILI9341_PIXFMT);
    writedata(0x55);

    writecommand(ILI9341_FRMCTR1);
    writedata(0x00);
    writedata(0x18);

    writecommand(ILI9341_DFUNCTR);    // Display Function Control
    writedata(0x08);
    writedata(0x82);
    writedata(0x27);

    writecommand(0xF2);    // 3Gamma Function Disable
    writedata(0x00);

    writecommand(ILI9341_GAMMASET);    //Gamma curve selected
    writedata(0x01);

    writecommand(ILI9341_GMCTRP1);    //Set Gamma
    writedata(0x0F);
    writedata(0x31);
    writedata(0x2B);
    writedata(0x0C);
    writedata(0x0E);
    writedata(0x08);
    writedata(0x4E);
    writedata(0xF1);
    writedata(0x37);
    writedata(0x07);
    writedata(0x10);
    writedata(0x03);
    writedata(0x0E);
    writedata(0x09);
    writedata(0x00);

    writecommand(ILI9341_GMCTRN1);    //Set Gamma
    writedata(0x00);
    writedata(0x0E);
    writedata(0x14);
    writedata(0x03);
    writedata(0x11);
    writedata(0x07);
    writedata(0x31);
    writedata(0xC1);
    writedata(0x48);
    writedata(0x08);
    writedata(0x0F);
    writedata(0x0C);
    writedata(0x31);
    writedata(0x36);
    writedata(0x0F);

    writecommand(ILI9341_SLPOUT);    //Exit Sleep
    delay(120);
    writecommand(ILI9341_DISPON);    //Display on

}

void Adafruit_ILI9341::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{

    writecommand(ILI9341_CASET); // Column addr set
    writedata(x0 >> 8);
    writedata(x0 & 0xFF);     // XSTART
    writedata(x1 >> 8);
    writedata(x1 & 0xFF);     // XEND

    writecommand(ILI9341_PASET); // Row addr set
    writedata(y0 >> 8);
    writedata(y0);     // YSTART
    writedata(y1 >> 8);
    writedata(y1);     // YEND

    writecommand(ILI9341_RAMWR); // write to RAM
}

void Adafruit_ILI9341::pushColor(uint16_t color)
{
    _dcPin->Set();
    _ssPin->Reset();

//    _spi->TransmitByte(color >> 8);
//    _spi->TransmitByte(color);

    while (!(*SR & SPI_SR_TXE))
        ;
    *DR = (color>>8); //_frameBuffer[px]>>8;

//        while(!(*SR & SPI_SR_RXNE))
//            ;

    //tmp  = *DR;

    while (!(*SR & SPI_SR_TXE))
        ;

    *DR = color; //_frameBuffer[px];

//        while(!(*SR & SPI_SR_RXNE))
//            ;
    //tmp  = *DR;


    _ssPin->Set();
}



#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O3")
#endif
void Adafruit_ILI9341::display()
{
    setAddrWindow(0x00, 0x00, ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT); //go home
    _dcPin->Set();

    _ssPin->Reset();
    volatile uint16_t tmp;
    for (int px = 0; px < _GRAMSIZE; px++)
    {
        while (!(*SR & SPI_SR_TXE))
            ;
        *DR = 0xf8; //_frameBuffer[px]>>8;

//        while(!(*SR & SPI_SR_RXNE))
//            ;

        //tmp  = *DR;

        while (!(*SR & SPI_SR_TXE))
            ;
        *DR = 0x0; //_frameBuffer[px];

//        while(!(*SR & SPI_SR_RXNE))
//            ;
        //tmp  = *DR;


    }
    _ssPin->Set();


}
#ifdef __GNUC__
#pragma GCC pop_options
#endif


void Adafruit_ILI9341::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
        return;

    setAddrWindow(x, y, x + 1, y + 1);
    pushColor(color);
}

void Adafruit_ILI9341::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{

    // Rudimentary clipping
    if ((x >= _width) || (y >= _height))
        return;

    if ((y + h - 1) >= _height)
        h = _height - y;

    setAddrWindow(x, y, x, y + h - 1);

    uint8_t hi = color >> 8, lo = color;

    _dcPin->Set();
    _ssPin->Reset();

    while (h--)
    {
//        _spi->TransmitByte(hi);
//        _spi->TransmitByte(lo);

        while (!(*SR & SPI_SR_TXE))
            ;
        *DR = hi; //_frameBuffer[px]>>8;

//        while(!(*SR & SPI_SR_RXNE))
//            ;

        //tmp  = *DR;

        while (!(*SR & SPI_SR_TXE))
            ;
        *DR = lo; //_frameBuffer[px];

//        while(!(*SR & SPI_SR_RXNE))
//            ;
        //tmp  = *DR;

    }
    _ssPin->Set();


}

void Adafruit_ILI9341::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{

    // Rudimentary clipping
    if ((x >= _width) || (y >= _height))
        return;
    if ((x + w - 1) >= _width)
        w = _width - x;

    setAddrWindow(x, y, x + w - 1, y);

    uint8_t hi = color >> 8, lo = color;

    _dcPin->Set();
    _ssPin->Reset();

    while (w--)
    {
//        _spi->TransmitByte(hi);
//        _spi->TransmitByte(lo);
        while (!(*SR & SPI_SR_TXE))
            ;
        *DR = hi; //_frameBuffer[px]>>8;

//        while(!(*SR & SPI_SR_RXNE))
//            ;

        //tmp  = *DR;

        while (!(*SR & SPI_SR_TXE))
            ;
        *DR = lo; //_frameBuffer[px];

//        while(!(*SR & SPI_SR_RXNE))
//            ;
        //tmp  = *DR;
    }
    _ssPin->Set();


}


void Adafruit_ILI9341::fillScreen(uint16_t color)
{
    fillRect(0, 0, _width, _height, color);
}

// fill a rectangle
void Adafruit_ILI9341::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{

    // rudimentary clipping (drawChar w/big text requires this)
    if ((x >= _width) || (y >= _height))
        return;
    if ((x + w - 1) >= _width)
        w = _width - x;
    if ((y + h - 1) >= _height)
        h = _height - y;

    setAddrWindow(x, y, x + w - 1, y + h - 1);

    uint8_t hi = color >> 8, lo = color;
    volatile uint8_t tmp;

    _dcPin->Set();
    _ssPin->Reset();

    for (y = h; y > 0; y--)
    {
        for (x = w; x > 0; x--)
        {
//            _spi->TransmitByte(hi);
//            _spi->TransmitByte(lo);

            while (!(*SR & SPI_SR_TXE))
                ;
            *DR = hi; //_frameBuffer[px]>>8;

//            while(!(*SR & SPI_SR_RXNE))
//                ;
//            tmp  = *DR;

            while (!(*SR & SPI_SR_TXE))
                ;
            *DR = lo; //_frameBuffer[px];

//            while(!(*SR & SPI_SR_RXNE))
//                ;
//            tmp  = *DR;

        }
    }
    _ssPin->Set();
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Adafruit_ILI9341::color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void Adafruit_ILI9341::setRotation(uint8_t m)
{
    writecommand(ILI9341_MADCTL);
    rotation = m % 4; // can't be higher than 3
    switch (rotation)
    {
    case 0:
        writedata(MADCTL_MX | MADCTL_BGR);
        _width = ILI9341_TFTWIDTH;
        _height = ILI9341_TFTHEIGHT;
        break;
    case 1:
        writedata(MADCTL_MV | MADCTL_BGR);
        _width = ILI9341_TFTHEIGHT;
        _height = ILI9341_TFTWIDTH;
        break;
    case 2:
        writedata(MADCTL_MY | MADCTL_BGR);
        _width = ILI9341_TFTWIDTH;
        _height = ILI9341_TFTHEIGHT;
        break;
    case 3:
        writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
        _width = ILI9341_TFTHEIGHT;
        _height = ILI9341_TFTWIDTH;
        break;
    }
}

void Adafruit_ILI9341::invertDisplay(bool i)
{
    writecommand(i ? ILI9341_INVON : ILI9341_INVOFF);
}

