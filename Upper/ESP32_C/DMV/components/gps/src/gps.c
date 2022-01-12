#include "gps.h"

esp_err_t gps_init(){
    esp_err_t e = ESP_OK;

    //串口配置结构体
    uart_config_t uart_gps_config;

    //设置串口
    uart_gps_config.baud_rate   =   115200;
    uart_gps_config.data_bits   =   UART_DATA_8_BITS;
    uart_gps_config.parity      =   UART_PARITY_DISABLE;
    uart_gps_config.stop_bits   =   UART_STOP_BITS_1;
    uart_gps_config.flow_ctrl   =   UART_HW_FLOWCTRL_DISABLE;
    e |= uart_param_config(GPS_UART_NUM, &uart_gps_config);

    //IO映射
    e |= uart_set_pin(GPS_UART_NUM, 
                 GPS_TX_PIN,
                 GPS_RX_PIN,
                 UART_PIN_NO_CHANGE,
                 UART_PIN_NO_CHANGE);

    //注册串口服务、设置缓冲区大小
    e |= uart_driver_install(GPS_UART_NUM,
                        GPS_RX_BUFF_SIZE*2,
                        GPS_TX_BUFF_SIZE*2,
                        0,
                        NULL,
                        0);             
    return e;
}


/**
 * @brief   按指定字符分割字符串
 * @param   src         源字符串的首地址
 * @param   separator   指定的分割字符
 * @param   dest        接收子字符串的数组
 * @param   num         分割后子字符串的个数
 * @return  ESP_OK/FAIL
*/
esp_err_t split(char *src, const char *separator, char **dest, int *num){
    char *pNext;

    if(src == NULL || separator == NULL){
        *num = 0;
        return ESP_FAIL;
    }

    else{
        pNext = strtok(src, separator); 
        while(pNext != NULL){
            *dest++ = pNext;
            (*num)++;
            pNext = (char *)strtok(NULL, separator);  
        }  
        return ESP_OK;
    }  
}   


/**
 * @brief       从GPS GLL数据流中获取信息  
 * @param       gps_info            获取的GPS信息
 * @param       gps_gll_stream      GLL数据流
 * @return      esp_err_t
 */
esp_err_t gps_getInfo(GPS_GLL_INFO* gps_info, char** gps_gll_stream){

    //初始化gps_info
    memset(gps_info, 0, sizeof(GPS_GLL_INFO));

    //获取定位信息
    memcpy(gps_info->gps_sys_mark, gps_gll_stream[0] + 1, 2);                       //卫星发射器标识                   
    memcpy(gps_info->gps_lat, gps_gll_stream[1], strlen(gps_gll_stream[1]));        //纬度     
    memcpy(&(gps_info->gps_uLat), gps_gll_stream[2], 1);                            //纬度方向                       
    memcpy(gps_info->gps_lon, gps_gll_stream[3], strlen(gps_gll_stream[3]));        //经度
    memcpy(&(gps_info->gps_uLon), gps_gll_stream[4], 1);                            //经度方向
    memcpy(gps_info->gps_UTCtime, gps_gll_stream[5], strlen(gps_gll_stream[5]));    //UTC时间
    memcpy(&(gps_info->gps_valid), gps_gll_stream[6], 1);                           //数据有效性
    memcpy(&(gps_info->gps_mode), gps_gll_stream[7], 1);                            //定位模式
    memcpy(&(gps_info->gps_CS), gps_gll_stream[7] + 2, 1);                          //校验和

    //释放split使用的空间
    for(int i = 0; i < 8; i++){
        free(gps_gll_stream[i]);
    }
    return ESP_OK;
}


/**
 * @brief       gps自动刷新函数, 自动读取GLL数据流并通过TCP发送
 * @param       none
 * @return      none
 */
void GPS_Fresh_Auto(){
    
    GPS_GLL_INFO gps_gll_info;
    char gps_gll_sub_stream[15][20] = {0};
    uint8_t* gps_gll_stream = (uint8_t*)malloc(GPS_RX_BUFF_SIZE + 1);

    while(1){
        memset(&gps_gll_info, 0, sizeof(gps_gll_info));
        memset(gps_gll_stream, 0, GPS_RX_BUFF_SIZE + 1);

        //获取数据流
        const int gps_gll_stream_len = uart_read_bytes(GPS_UART_NUM,
                                                       gps_gll_stream,
                                                       GPS_RX_BUFF_SIZE,
                                                       10/portTICK_RATE_MS);
        gps_gll_stream[gps_gll_stream_len] = 0; 

        //将数据流转换为char型
        char* data = (char*)gps_gll_stream;

        //分割数据流
        int gps_gll_sub_stream_num = 0;
        split(data, ",", &gps_gll_sub_stream, &gps_gll_sub_stream_num);

        if(gps_gll_sub_stream_num != 8)     return;
        else                                gps_getInfo(&gps_gll_info, gps_gll_sub_stream);
    }
    free(gps_gll_stream);
}