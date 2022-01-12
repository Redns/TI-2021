#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "lwip/sockets.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"

static const char* TAG = "MyModule";

#define TCP_SERVER_PORT 8888            // TCP服务器端口号

#define TXD2_PIN        GPIO_NUM_17     // TXD2引脚号
#define RXD2_PIN        GPIO_NUM_16     // RXD2引脚号

#define BUFF_SIZE       2048            // 缓冲区大小

int client_socket = 0;
uint8_t buff[BUFF_SIZE];


static esp_err_t event_handler(void* ctx, system_event_t* event){
    switch(event->event_id){
        case SYSTEM_EVENT_WIFI_READY:
            printf("[Wifi]......Ready\n");
            break;

        case SYSTEM_EVENT_AP_START:
            printf("[Wifi]......Start\n");
            break;

        case SYSTEM_EVENT_AP_STACONNECTED:
            printf("[Wifi]......Connected\n");
            break;

        case SYSTEM_EVENT_AP_STADISCONNECTED:
            printf("[Wifi]......Disconnected\n");
            break;

        default:
            break;                
    }

    return ESP_OK;
}

void wifi_init(){
    //初始化tcpip
    tcpip_adapter_init();

    //设置回调函数为event_handler
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    //初始化wifi设置
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    //配置wifi
    wifi_config_t wifi_config = {
        .ap = {
            .ssid           =   "CCC",                      //WIFI名称
            .ssid_len       =   3,                          //WIFI名长度
            .max_connection =   1,                          //最大连接数
            .password       =   "12345678",                 //WIFI密码
            .authmode       =   WIFI_AUTH_WPA_WPA2_PSK,     //WIFI加密方式
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));

    //启动WIFI
    ESP_ERROR_CHECK(esp_wifi_start());
}

void uart_init(){
    /* uart配置结构体 */
    uart_config_t uart2_config = {
        .baud_rate = 115200,                     // 波特率
        .data_bits = UART_DATA_8_BITS,           // 数据位
        .parity    = UART_PARITY_DISABLE,        // 校验位
        .stop_bits = UART_STOP_BITS_1,           // 停止位
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE    // 硬件流控
    };
    uart_param_config(UART_NUM_2, &uart2_config);

    /* IO端口映射 */
    uart_set_pin(UART_NUM_2, TXD2_PIN, RXD2_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    /* 使能串口并设置缓冲区大小 */
    uart_driver_install(UART_NUM_2, BUFF_SIZE*2, BUFF_SIZE*2, 0, NULL, 0);
}

void tcp_server_init(int* client_socket){
    //1.新建socket
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if(server_socket < 0){
        printf("[TCP]......Create socket failed\n");
    }

    //2.配置服务器
    struct sockaddr_in dest_addr = {
        .sin_family         =   AF_INET,
        .sin_addr.s_addr    =   htonl(INADDR_ANY),
        .sin_port           =   htons(TCP_SERVER_PORT),
    };

    //3.绑定地址
    int err = bind(server_socket, (struct socketassr*)&dest_addr, sizeof(dest_addr));
    if(err){
        printf("[TCP]......Bind failed\n");
        close(server_socket);
    }

    //4.开始监听
    err = listen(server_socket, 5);
    if(err){
        printf("[TCP]......Listen failed\n");
        close(server_socket);
    }

    //5.等待客户端连接
    while(1){
        struct sockaddr_in6 source_addr;
        uint32_t addr_len = sizeof(source_addr);
        *(client_socket) = accept(server_socket, (struct sockaddr*)&source_addr, &addr_len);
        if(*(client_socket) < 0){
            printf("[TCP]......Connect failed\n");
            close(server_socket);
        }    

        else{
            printf("[TCP]......Connected\n");
            break;
        }
    }
}

void uart2_rx_task(){
    while(1){
        memset(buff, 0, sizeof(buff));
        const int len = uart_read_bytes(UART_NUM_2, buff, BUFF_SIZE, 300/portTICK_RATE_MS);

        if(len > 0){
            send(client_socket, buff, len, 0);
        }

        vTaskDelay(100/portTICK_RATE_MS);
    }
}


void app_main(void){

    /* 初始化NVS */
    ESP_ERROR_CHECK(nvs_flash_init());

    /* 初始化WIFI和UART */
    wifi_init();
    uart_init();

    /* 初始化TCP */
    tcp_server_init(&client_socket);

    /* 创建uart2串口任务 */
    xTaskCreate(uart2_rx_task, "uart2_rx_task", BUFF_SIZE*2, NULL, configMAX_PRIORITIES, NULL);
    ESP_LOGI(TAG, "Create task done!\n");
}