/*
 * @Author:         Redns
 * @Date: 	        2021-11-04 16:22:21
 * @LastEditTime: 	2021-11-04 16:23:24
 * @Description:    此库包含UART通信相关函数
 */

#ifndef __COMMUNICATE_H
#define __COMMUNICATE_H

#include <string.h>

void uart_send(char* data);     // 发送字符串数组

#endif