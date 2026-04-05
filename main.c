#include <stdint.h>
#include <stdio.h>
#include "sleep.h"

// IIC
#define IIC_CFG   *((uint32_t *)0xE0005000)
#define IIC_ADDR  *((uint32_t *)0xE0005008)
#define IIC_DATA  *((uint32_t *)0xE000500C)
#define IIC_ISR   *((uint32_t *)0xE0005010)
#define IIC_SIZE  *((uint32_t *)0xE0005014)

// SLCR
#define SLCR_LOCK   *((uint32_t *)0xF8000004)
#define SLCR_UNLOCK *((uint32_t *)0xF8000008)
#define SLCR_IIC    *((uint32_t *)0xF8000224)

// UART
#define UART_CR   *((uint32_t *)0xE0001000)
#define UART_MR   *((uint32_t *)0xE0001004)
#define UART_BG   *((uint32_t *)0xE0001018)
#define UART_SR   *((uint32_t *)0xE000102C)
#define UART_TX   *((uint32_t *)0xE0001030)
#define UART_BD   *((uint32_t *)0xE0001034)

// 7 segment
#define SEG_CTL  *((uint32_t *)0x43C10000)
#define SEG_DAT  *((uint32_t *)0x43C10004)

#define SENSOR 0x48

int main()
{
    char text[100];
    float temp;
    int value;

    // UART INIT 
    UART_CR = 0x03;
    while (UART_CR & 0x02);
    while (UART_CR & 0x01);

    UART_MR = 0x20;
    UART_BG = 0x7C;
    UART_BD = 6;
    UART_CR |= 0x14;

    // simple print
    char *startMsg = "Program start\r\n";
    while (*startMsg)
    {
        while (UART_SR & 0x10);
        UART_TX = *startMsg++;
    }

    // IIC RESET 
    SLCR_UNLOCK = 0xDF0D;
    SLCR_IIC = 0x3;
    SLCR_IIC = 0x0;
    SLCR_LOCK = 0x767B;

    //IIC INIT 
    IIC_CFG = 0x0C0F;

    SEG_CTL = 1;

    while (1)
    {
        int timeout = 1000000;
        uint8_t hi, lo;
        int16_t raw;

        // request 2 bytes
        IIC_SIZE = 2;
        IIC_ADDR = SENSOR;

        // wait until done
        while (!(IIC_ISR & 1) && timeout > 0)
        {
            timeout--;
        }

        if (timeout == 0)
        {
            // error
            char *err = "IIC fail\r\n";
            while (*err)
            {
                while (UART_SR & 0x10);
                UART_TX = *err++;
            }

            SEG_DAT = 0x99999999; // show error
        }
        else
        {
            // read data
            hi = IIC_DATA;
            lo = IIC_DATA;

            raw = (hi << 8) | lo;
            raw = raw >> 5;

            temp = raw * 0.125f;

            sprintf(text, "T = %.2f \r\n", temp);

            char *p = text;
            while (*p)
            {
                while (UART_SR & 0x10);
                UART_TX = *p++;
            }

            value = (int)temp;

            if (value < 0) value = -value;
            if (value > 9999) value = 9999;

            // simple digit display 
            uint32_t out = 0;
            int i;

            for (i = 0; i < 4; i++)
            {
                out |= (value % 10) << (i * 8);
                value /= 10;
            }

            out |= 0x80808080;
            SEG_DAT = out;
        }

        sleep(1);
    }

    return 0;
}
