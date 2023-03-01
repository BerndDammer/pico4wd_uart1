
#include <stdbool.h>


void uart1_init(void);
unsigned char uart1_in(void);
bool uart1_in_ready(void);
void uart1_out(unsigned char b );
bool uart1_out_ready(void);
void uart1_outs(unsigned char *b );
