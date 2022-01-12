/*
 * @Author:         Redns
 * @Date: 	        2021-10-08 23:56:23
 * @LastEditTime: 	2021-10-13 14:32:51
 * @Description:    此库包含L298N驱动程序
 */
#ifndef __L298N_H
#define __L298N_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "sdkconfig.h"

//引脚定义
#define L298N_ENA 12
#define L298N_ENB 13
#define L298N_IN1 14
#define L298N_IN2 25
#define L298N_IN3 26
#define L298N_IN4 27

//PWM相关设置
#define LEDC_MAX_DUTY       8191    //8191->100%
#define INTR_ALLOC_FLAGS    0       //中断标记

#define L298N_IN1_HIGH() gpio_set_level(L298N_IN1, 1)
#define L298N_IN1_LOW()  gpio_set_level(L298N_IN1, 0)

#define L298N_IN2_HIGH() gpio_set_level(L298N_IN2, 1)
#define L298N_IN2_LOW()  gpio_set_level(L298N_IN2, 0)

#define L298N_IN3_HIGH() gpio_set_level(L298N_IN3, 1)
#define L298N_IN3_LOW()  gpio_set_level(L298N_IN3, 0)

#define L298N_IN4_HIGH() gpio_set_level(L298N_IN4, 1)
#define L298N_IN4_LOW()  gpio_set_level(L298N_IN4, 0)

//L298N模块对应PWM通道
typedef struct{
    ledc_channel_config_t ENA;
    ledc_channel_config_t ENB;
}L298N_PWM_CHANNEL;

void l298n_init(L298N_PWM_CHANNEL* l298n_pwm_channel);      //初始化l298n模块
void l298n_set_direction(uint8_t direction);                //设置方向
void l298n_set_speed(uint8_t direction, 
                     uint8_t speed, 
                     L298N_PWM_CHANNEL* l298n_pwm_channel); //设置速度

#endif