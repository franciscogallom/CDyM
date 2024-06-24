#include <stdint.h>
 
char* UART_Get_String_From_Buffer(void);

void UART_Send_String_To_Transmit(char *);

void UART_Set_flag(uint8_t);

uint8_t UART_Get_Flag(void);

void UART_Update(void);