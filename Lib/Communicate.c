#include "Communicate.h"

/**
 * @brief   MSP432串口发送函数
 * @param   data    待发送的字符串数组
 * @return  none
 */
void uart_send(char* data){
    int len = strlen(data);
    for(int i = 0; i < len; i++){
        MAP_UART_transmitData(EUSCI_A0_BASE, data[i]);
    }
}