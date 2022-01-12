/*
 * @Author:         Redns
 * @Date: 	        2021-10-14 16:58:58
 * @LastEditTime: 	2021-10-16 21:18:55
 * @Description:    此库包含机械臂相关处理函数
 */
#ifndef __ARM_H
#define __ARM_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "sdkconfig.h"

#define SERVO_SPIN_PIN 32   //旋转舵机引脚
#define SERVO_FLEX_PIN 33   //伸缩舵机引脚
#define SERVO_UPDN_PIN 34   //升降舵机引脚
#define SERVO_CLAW_PIN 35   //机械爪子引脚
#define LEDC_MAX_DUTY 8191

//机械臂对应PWM通道
typedef struct{
    ledc_channel_config_t spin;
    ledc_channel_config_t flex;
    ledc_channel_config_t updn;
    ledc_channel_config_t claw;
}ARM_CHANNEL;

esp_err_t arm_init(ARM_CHANNEL* arm_channel);
esp_err_t arm_set_spin(uint8_t level, ARM_CHANNEL* arm_channel);       
esp_err_t arm_set_flex(uint8_t level, ARM_CHANNEL* arm_channel);      
esp_err_t arm_set_updn(uint8_t level, ARM_CHANNEL* arm_channel);     
esp_err_t arm_set_claw(uint8_t level, ARM_CHANNEL* arm_channel); 

#endif