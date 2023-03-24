/////////////////////////////////////////////////////////////
///
///                    T R A P
///
///                  I M P O R T A N T
///
/// Commands send out only {NEWLINE} at the end
///
/// Answers come with {CARRIAGE RETURN} {NEWLINE} at the end
///
///  or
///     CR will be part of command data
///
#include "loop.h"
#include "blinker.h"
#include "uart1.h"

#include "parameter.h"

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include <time.h>
#include <stdlib.h>

#define CONSOLE_TIMEOUT 100000

static bool transmitter_on;
static int transmitter_counter = 10;

void send_sequencer(void)
{
    if (!transmitter_on)
        return;
    if (--transmitter_counter > 0)
        return;
    transmitter_counter = 10;
    uart1_outs(
            "WS+{ \"Name\": \"MC_Cindy\", \"Type\": \"PICO-4WD Car\", \"Check\": \"SunFounder Controller\"}\n");
}

void menu(void)
{
    printf("------------------------------------\n");
    printf("1 AT\n");
    printf("2 AT+GMR\n");
    printf("3 AT+CMD\n");
    printf("4 AT+GMR 015 012\n");
    printf("5 AT+RST 015 012\n");
    printf("6 ATE1 015 012\n");
    printf("------------------------\n");
    printf("a SET+SSID<ssid>\n");
    printf("b SET+PSK<password>\n");
    printf("c SET+PORT<port>\n");
    printf("d SET+MODE<mode>: set Wi-Fi Mode, STA = 1, AP = 2\n");
    printf("e SET+START: set Connect, return IP if connected\n");
    printf(
            "f SET+SMD<mode>: set send mode, 0:send original text; 1:send simplified text\n");
    printf("g WS+<data>: send data over websocket\n");
    printf("  WS+{\"value\": 25}\n");
    printf("h SET+MUELL\n");
    printf("l SET+LED1\n");
    printf("p show parameter\n");
    printf("z SET+RESET: reset esp8266\n");
    printf("0 space all off\n");
    printf("press key to select\n");
    printf("------------------------------------\n");
}

void showPara(void)
{
    printf("Parameter SSID:%s\n", PARA_SSID);
    printf("Parameter  PWD:%s\n", PARA_PWD);
}

void loop(void)
{
    int counter = 0;
    volatile int c;
    menu();
    uart1_init();
    unsigned char buffer[150];

    for (;;)
    {
        c = getchar_timeout_us(CONSOLE_TIMEOUT);
        blinker_toggle();
        send_sequencer();

        if (c == PICO_ERROR_TIMEOUT)
        {
            while (uart1_in_ready())
                putchar_raw(uart1_in());
            counter++;

        }
        else
        {
            switch (c)
            {
            case '1':
                uart1_outs("AT\n");
                break;
            case '2':
                uart1_outs("AT+GMR\n");
                break;
            case '3':
                uart1_outs("AT+CMD\n");
            case '4':
                uart1_outs("AT+GMR\015\012");
                break;
            case '5':
                uart1_outs("AT+RST\015\012");
                break;
            case '6':
                uart1_outs("ATE1\015\012");
                break;
            case 'a':
                //uart1_outs("SET+SSID" MY_SSID "\012"); // Backslash newline does not work here
                strcpy(buffer, "SET+SSID");
                strcat(buffer, PARA_SSID);
                strcat(buffer, "\n");
                uart1_outs(buffer);
                break;
            case 'b':
                //uart1_outs("SET+PSK" MY_PWD "\012");
                strcpy(buffer, "SET+PSK");
                strcat(buffer, PARA_PWD);
                strcat(buffer, "\n");
                uart1_outs(buffer);
                break;
            case 'c':
                uart1_outs("SET+PORT8765\n");
                break;
            case 'd':
                uart1_outs("SET+MODE1\n");
                break;
            case 'e':
                uart1_outs("SET+START\n");
                break;
            case 'f':
                uart1_outs("SET+SMD0\n");
                break;
            case 'g':
                uart1_outs("WS+Hello\n");
                break;
            case 'h':
                uart1_outs("SET+MUELL\n");
                break;
            case 'l':
                uart1_outs("SET+LED1\n");
                break;
            case 'p':
                showPara();
                break;
            case 's':
                transmitter_on = !transmitter_on;
                puts(transmitter_on ? "On\n" : "Off\n");
                break;
            case 'z':
                uart1_outs("SET+RESET\n");
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

