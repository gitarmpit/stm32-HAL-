#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "gpio_cpp.h"
#include <stdio.h>
#include "Adafruit_ILI9341.h"
#include "debug.h"
#include "system_time.h"
#include "fonts/7seg_4.h"
#include "fonts/consola12.h"
#include "fonts/FreeSerifBoldItalic18pt7b.h"

static void test4(Adafruit_ILI9341* display)
{
    systick_enable(TRUE);
    //display->setTextSize(4);

    uint32_t t0 = millis();

    for (int i = 0; i < 10; ++i)
    {
        display->setCursor(0, 24);
        display->write("Job2d724e6a-7680-4a38-bc93-477280c5bc9e.xml");
    }

    volatile uint32_t t = millis() - t0;

    ++t;


    while(1)
        ;

}

void test()
{
    delay(10);

    SPI* spi = GPIO_Helper::SetupSPI(SPI1_PA_5_6_7, true, false, false, SPI_BAUD_RATE_2);

    GPIO_PIN* rstPin = GPIO_Helper::GetPin("C4");
    rstPin->SetupGPIO_OutPP();
    rstPin->SetSpeedHigh();

    GPIO_PIN* dcPin = GPIO_Helper::GetPin("C5");
    dcPin->SetupGPIO_OutPP();
    dcPin->SetSpeedHigh();

    GPIO_PIN* ssPin = GPIO_Helper::GetPin("A4");
    ssPin->SetupGPIO_OutPP();
    ssPin->SetSpeedHigh();

    Adafruit_ILI9341 display(spi, dcPin, rstPin, ssPin);

    //display.fillScreen(ILI9341_CYAN);
    //display.clearScreen();
    //display.display();
    //while(1);

    //display.setBgColor();
    display.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    display.setRotation(1);
    display.fillScreen(ILI9341_BLACK);
    //display.setFont(&consola_ttf);
    //display.setFont(&FreeSerifBoldItalic18pt7b);
    //display.setYadvance(20);
    //test4 (&display);
    display.setCursor(0, 0);
//    display.printf ("TEST test %6.3f", 1.2345);
//    display.setCursor(1, 1);
//    display.printf ("TEST test %6.3f\n", 2.2345);
//    display.printf ("TEST test %6.3f", 3.2345);


    //display.setTextColor(ILI9340_BLACK, ILI9340_WHITE);

    //display.invertDisplay(true);
    //display.drawFastHLine(0, 0, 100, 10);

    //display.setCursor(0, 0);

    //display.write("Hello ILI9341 world!");


    //display.write("ABCabc-12345");
    display.setFont(&DSEG7ModernMini_BoldItalic_ttf);
    //display.setFont(&digital_7_monoitalic_ttf);
    display.setCursor(2, 2);
    display.write ("11:32");

    //display.write("ABCabc-123.O");
    //display.write(L"����");
    //display.write(1046);
    //display.write(371);
    //display.write(421);
    //display.write(521);
    //display.drawChar(0, 30, 'A', ILI9341_RED, 0, 1);

    //display.display();


        //display.write("test", 0, 0, 2);
        //display.drawString("test", 0, 0, 4);
        //display.drawString("test", 0, 0, 6);
        //display.drawString("test", 0, 0, 7);

     while(1)
    	 ;

}

int main()
{
#ifdef STM32F1
    RCC_EnableHSI_64Mhz_AHB_64Mhz_APB1_32MHz_APB2_64MHz();
#elif defined (STM32F4)
    RCC_EnableHSI_168Mhz();
#endif
    Debug_EnableCYCCNT (TRUE);
    test();

}
