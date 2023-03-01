#include <stdbool.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"

#include "hardware/irq.h"

#include "uart1.h"

#define FAIL while(true);

#define BUF_IN_SIZE_POW2 10
#define BUF_IN_TOP (&(buf_in[1<<BUF_IN_SIZE_POW2]))

// receive interrupt buffer
volatile unsigned char buf_in[1 << BUF_IN_SIZE_POW2];
volatile unsigned char *buf_in_write_poi;
volatile unsigned char *buf_in_read_poi;
volatile unsigned int buf_in_count; // this must be atomic

////////////////////////////////////////////////////////////////////////////////////////
///
/// callback
///
#define CALLBACK_DELAY_US 125000

volatile int callback_counter = 0;


static void on_uart1_rec(void)
{
    callback_counter++;

    buf_in_count++;
    *buf_in_write_poi = uart_getc(uart1);
    buf_in_write_poi++;
    if( buf_in_write_poi >= BUF_IN_TOP) buf_in_write_poi = buf_in;
    if(buf_in_count >= (1<<BUF_IN_SIZE_POW2)) FAIL;
}
///////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
/// public functions

void uart1_init(void)
{
    buf_in_write_poi = buf_in;
    buf_in_read_poi = buf_in;
    buf_in_count = 0;

    uart_init(uart1, 115200);
    uart_set_hw_flow (uart1, false, false);
    uart_set_format (uart1, 8, 1, UART_PARITY_NONE );
    uart_set_irq_enables (uart1, true, false);

    // the wlan modul is connected to ....
    gpio_set_function(4, GPIO_FUNC_UART);
    gpio_set_function(5, GPIO_FUNC_UART);

    irq_set_exclusive_handler(UART1_IRQ, on_uart1_rec);
    irq_set_enabled(UART1_IRQ, true);
}

unsigned char uart1_in(void)
{
    unsigned char result;
    while(!uart1_in_ready());
    result = *buf_in_read_poi;
    buf_in_read_poi++;
    if( buf_in_read_poi >= BUF_IN_TOP) buf_in_read_poi = buf_in;
    buf_in_count--;
    return result;
}

bool uart1_in_ready(void)
{
    return buf_in_count > 0;
}

void uart1_out(unsigned char b )
{
    uart_putc_raw(uart1, b);
}


void uart1_outs(unsigned char *b )
{
    uart_puts(uart1, b);
}

bool uart1_out_ready(void)
{
    return uart_is_writable(uart1);
}

