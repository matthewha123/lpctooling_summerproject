////Replacing UART functions with macros////

// Defines
#define UART_BUFFER_SIZE 100

// Variables
static char str[10];

static RINGBUFF_T uart_rx_ring;
static uint8_t    _uart_rx_ring[UART_BUFFER_SIZE];
static RINGBUFF_T uart_tx_ring;
static uint8_t    _uart_tx_ring[UART_BUFFER_SIZE];

#define PRINT(str) {Chip_UART_SendRB(LPC_USART, &uart_tx_ring, str, strlen(str));}
#define PRINT_BLOCKING(str) {Chip_UART_SendBlocking(LPC_USART, str, strlen(str));}
#define PRINT_LN(str) {PRINT(str); PRINT("\r\n");}
#define PRINT_LN_BLOCKING(str) {PRINT_BLOCKING(str); PRINT_BLOCKING("\r\n")}
#define PRINT_NUM(num, base) {itoa(num, str, base); PRINT(str);}
#define PRINT_NUM_BLOCKING(num,base) {itoa(num, str, base); PRINT_BLOCKING(str);}
////R///////

////Repalcing GPIO functions with macros////
//name is a macro, "port, pin"
#define SET_GPIO_PIN(name, state) { Chip_GPIO_SetPinState(LPC_GPIO, name, state);}
#define READ_GPIO_PIN(name) {Chip_GPIO_GetPinState(LPC_GPIO, name);}
#define TOGGLE_GPIO_PIN(name) { Chip_GPIO_SetPinState(LPC_GPIO, name, !READ_GPIO_PIN(name));}
////////