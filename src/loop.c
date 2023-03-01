#include "loop.h"
#include "blinker.h"
#include "uart1.h"

#include <stdio.h>
#include "pico/stdlib.h"
//#include "hardware/timer.h"
//#include "hardware/watchdog.h"
//#include "hardware/clocks.h"

#include <time.h>
#include <stdlib.h>

#define CONSOLE_TIMEOUT 100000


void menu(void)
{
    printf("------------------------------------\n");
    printf("1 AT\n");
    printf("2 AT+GMR\n");
    printf("3 AT+CMD\n");
    printf("------------------------\n");
    printf("a SET+SSID<ssid>\n");
    printf("b SET+PSK<password>\n");
    printf("c SET+PORT<port>\n");
    printf("d SET+MODE<mode>: set Wi-Fi Mode, STA = 1, AP = 2\n");
    printf("e SET+START: set Connect, return IP if connected\n");
    printf("f SET+SMD<mode>: set send mode, 0:send original text; 1:send simplified text\n");
    printf("g WS+<data>: send data over websocket\n");
    printf("  WS+{\"value\": 25}\n");
    printf("h SET+MUELL\n");
    printf("l SET+LED1\n");
    printf("z SET+RESET: reset esp8266\n");
    printf("0 space all off\n");
    printf("press key to select\n");
    printf("------------------------------------\n");
}

void loop(void)
{
    int counter = 0;
    volatile int c;
    menu();
    uart1_init();

    for (;;)
    {
        c = getchar_timeout_us(CONSOLE_TIMEOUT);
        blinker_toggle();

        if (c == PICO_ERROR_TIMEOUT)
        {
            while( uart1_in_ready()) putchar_raw( uart1_in());
            counter++;
        }
        else
        {
            switch (c)
            {
            case '1':
                uart1_outs("AT\r\n");
                break;
            case '2':
                uart1_outs("AT+GMR\r\n");
                break;
            case '3':
                uart1_outs("AT+CMD\r\n");
                break;
            case 'a':
                uart1_outs("SET+SSID\r\n");
                break;
            case 'b':
                uart1_outs("SET+PSK\r\n");
                break;
            case 'c':
                uart1_outs("SET+PORT8765\r\n");
                break;
            case 'd':
                uart1_outs("SET+MODE1\r\n");
                break;
            case 'e':
                uart1_outs("SET+START\r\n");
                break;
            case 'f':
                uart1_outs("SET+SMD1\r\n");
                break;
            case 'g':
                uart1_outs("WS+Hello\r\n");
                break;
            case 'h':
                uart1_outs("SET+MUELL\r\n");
                break;
            case 'l':
                uart1_outs("SET+LED1\r\n");
                break;
            case 'z':
                uart1_outs("SET+RESET\r\n");
                break;
            case ' ':
            case '0':
                break;
            default:
                menu();
                break;
            }
        }
    }
}
