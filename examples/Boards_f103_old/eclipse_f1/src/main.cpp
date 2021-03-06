#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "si4432.h"
#include "GPIO_Helper.h"
#include "usart_cpp.h"
#include "fp2.h"
#include "telemetry.h"
#include <assert.h>
#include "fp2.h"
#include "system_time.h"
#include "systick.h"
#include "debug.h"

static void uart()
{
    uint32_t clockSpeed = 400000;
    USART* usart = GPIO_Helper::SetupUSART(USART2_PA_2_3, clockSpeed);
    unsigned char buf[1024];

    while(1)
    {
        while (!usart->IsRXNE())
            ;

        {
            if (usart->Receive() == 23)
            {
                ///for (uint16_t i = 257; i < 64; ++i)
                 usart->SendDWord(fix16_from_dbl(-31000.12345));

                //break;
            }
        }

    }
    while(1)
        ;
}

static void radio_rx_loop()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->EnablePeripheralClock(true);

    uint32_t clockSpeed = 400000; //400k
    USART* usart = GPIO_Helper::SetupUSART(USART1_PA_9_10, clockSpeed);


    SPI* spi = GPIO_Helper::SetupSPI(SPI2_PB_13_14_15, true, false, false, SPI_BAUD_RATE_32);
    GPIO_PIN* ssPin = GPIOB::GetInstance()->GetPin(GPIO_PIN2);
    ssPin->SetupGPIO_OutPP();
    GPIO_PIN* sdnPin = GPIOB::GetInstance()->GetPin(GPIO_PIN10);
    sdnPin->SetupGPIO_OutPP();

    volatile uint8_t len = 0;
    uint8_t data[64]={0};

    Si4432 radio(spi, ssPin, sdnPin, 0);
    bool rc = radio.init();
    if (!rc) while(1){}
    //assert (rc);
    radio.setBaudRate(50);  //kbs
    radio.setFrequency(433); //633

//    while(1)
//    {
//        len = 4;
//        for (uint8_t i = 0; i < len; ++i)
//        {
//            data[i] = i + 1;
//            usart->SendByte(data[i]);
//        }
//        delay(200);
//    }

    telemetry tel, tel2;
    uint32_t cnt = 0;
    len = sizeof(tel);

//    while(1)
//    {
//        if (radio.waitForPacket(1))
//        {
//            //radio.getPacketReceived((uint8_t*)&len, (uint8_t*)&tel);
////            if (len == sizeof(telemetry))
////            {
////                float y  = fix16_to_float(tel.y);
////                float p  = fix16_to_float(tel.p);
////                float r  = fix16_to_float(tel.r);
////
////                static int i;
////                ++i;
////            }
//
//            tel.timestamp_ms = cnt++;
//
//
//            while (!usart->IsRXNE())
//                ;
//
//            if (usart->Receive() == 23)
//            {
//                for (uint8_t i = 0; i < len; ++i)
//                {
//                    usart->SendByte(((uint8_t*)&tel)[i]);
//                }
//            }
//        }
//        //delay(200);
//    }

    //volatile int cnt = 0;
    radio.startListening();
    radio.clearFIFO();

    tel.timestamp_ms = 0;
    uint32_t last_recv = 0;
    uint32_t timeout = 2000;
    uint16_t timeouts = 0;
    tel.throttle = 0;
    uint16_t delay_ms = 100;
    while (1)
    {
        if (usart->IsRXNE())
        {
            delay_ms = usart->GetDR()*100;
        }

        //start of message
        usart->SendByte(0xde);
        usart->SendByte(0xad);
        usart->SendByte(0xbe);
        usart->SendByte(0xef);

        for (uint8_t i = 0; i < len; ++i)
        {
            usart->SendByte(((uint8_t*)&tel)[i]);
        }
        uint32_t now = millis();
        if (radio.isPacketReceived())
        {
            radio.getPacketReceived((uint8_t*)&len, (uint8_t*)&tel);
            tel.timestamp_ms = ++cnt;
            tel.throttle = timeouts;
            //tel.y = - 2.345;
            radio.startListening();
            radio.clearFIFO();
            last_recv = now;
        }
        else if (last_recv != 0 && (now - last_recv) > timeout)
        {
            tel.throttle = ++timeouts;
            radio.init();
            delay(10);
            radio.startListening();
            radio.clearFIFO();
            last_recv = now;
        }


        delay(delay_ms);

    }

}

static void test2()
{
    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOB::GetInstance()->EnablePeripheralClock(true);

    SPI* spi = GPIO_Helper::SetupSPI(SPI2_PB_13_14_15, true, false, false, SPI_BAUD_RATE_32);
    GPIO_PIN* ssPin = GPIOB::GetInstance()->GetPin(GPIO_PIN2);
    ssPin->SetupGPIO_OutPP();
    GPIO_PIN* sdnPin = GPIOB::GetInstance()->GetPin(GPIO_PIN10);
    sdnPin->SetupGPIO_OutPP();



    Si4432 radio(spi, ssPin, sdnPin, 0);
    radio.init();
    radio.setBaudRate(30);  //kbs
    radio.setFrequency(433); //633
    radio.readAll();

    //TX
    uint8_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    while(1)
    {
        while (!radio.sendPacket(sizeof data, data))
            ;

        delay(300);
    }


}


int main()
{
    RCC_EnableHSI_64Mhz_AHB_32Mhz_APB1_16MHz_APB2_16MHz();

    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIO_PIN* p = GPIOA::GetInstance()->GetPin(GPIO_PIN0);
    p->SetupGPIO_OutPP();
    p->Set();
    while(1)
    	;

    systick_enable(true);
    radio_rx_loop();

}
