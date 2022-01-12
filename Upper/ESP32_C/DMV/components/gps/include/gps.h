/*
 * @Author:         Redns
 * @Date: 	        2021-10-09 15:14:10
 * @LastEditTime: 	2021-10-13 21:34:17
 * @Description:    此库包含GPS处理相关函数
 */
#ifndef __GPS_H
#define __GPS_H

#include "esp_system.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "string.h"

//GPS通信串口(禁止使用UART0)
//UART1 -- RX:4
//         TX:5
//UART2 -- RX:13
//         TX:12
#define GPS_UART_NUM        UART_NUM_1
#define GPS_RX_BUFF_SIZE    256
#define GPS_TX_BUFF_SIZE    256
#define GPS_RX_PIN          GPIO_NUM_4  
#define GPS_TX_PIN          GPIO_NUM_5

//GPS GLL数据流信息
typedef struct{
    char gps_sys_mark[2];    //发送器标识
    char gps_lat[9];         //纬度
    char gps_uLat;           //纬度方向
    char gps_lon[9];         //经度
    char gps_uLon;           //经度方向
    char gps_UTCtime[10];    //UTC时间
    char gps_valid;          //数据有效性
    char gps_mode;           //定位模式
    char gps_CS;             //校验和
}GPS_GLL_INFO;

//GPS数据有效性
typedef enum{
    GPS_VALID_YES = 'A',     //数据有效
    GPS_VALID_NO = 'V',      //数据无效
}GPS_VALID;

//GPS定位模式
typedef enum{
    GPS_MODE_AUTO = 'A',            //自主模式
    GPS_MDOE_RECKON = 'E',          //估算模式
    GPS_MODE_DATA_INVALID = 'N',    //数据无效
    GPS_MODE_DIFFERENCE = 'D',      //差分模式
    GPS_MDOE_UNLOCATED = 'M',       //未定位
}GPS_MDOE;

esp_err_t gps_init();
esp_err_t gps_getInfo(GPS_GLL_INFO* gps_info, char** gps_gll_stream);
esp_err_t split(char *src, const char *separator, char **dest, int *num);
void GPS_Fresh_Auto();

#endif