#include "arm.h"

/**
 * @brief   机械臂初始化函数
 * @param   arm_channel 机械臂PWM通道结构体
 * @return  esp_err_t
 */
esp_err_t arm_init(ARM_CHANNEL* arm_channel){
    //初始化PWM
    ledc_timer_config_t arm_config = {
        .duty_resolution    =   LEDC_TIMER_13_BIT,
        .freq_hz            =   5000,
        .speed_mode         =   LEDC_HIGH_SPEED_MODE,
        .timer_num          =   LEDC_TIMER_1,
        .clk_cfg            =   LEDC_AUTO_CLK,
    };
    ledc_timer_config(&arm_config);

    //配置PWM通道
    ledc_channel_config_t spin  =   {
        .channel    =   LEDC_CHANNEL_0,
        .duty       =   0,
        .gpio_num   =   SERVO_SPIN_PIN,
        .speed_mode =   LEDC_HIGH_SPEED_MODE,
        .hpoint     =   0,
        .timer_sel  =   LEDC_TIMER_1,
    };

    ledc_channel_config_t flex  =   {
        .channel    =   LEDC_CHANNEL_1,
        .duty       =   0,
        .gpio_num   =   SERVO_FLEX_PIN,
        .speed_mode =   LEDC_HIGH_SPEED_MODE,
        .hpoint     =   0,
        .timer_sel  =   LEDC_TIMER_1,
    };

    ledc_channel_config_t updn  =   {
        .channel    =   LEDC_CHANNEL_2,
        .duty       =   0,
        .gpio_num   =   SERVO_UPDN_PIN,
        .speed_mode =   LEDC_HIGH_SPEED_MODE,
        .hpoint     =   0,
        .timer_sel  =   LEDC_TIMER_1,
    };

    ledc_channel_config_t claw  =   {
        .channel    =   LEDC_CHANNEL_3,
        .duty       =   0,
        .gpio_num   =   SERVO_CLAW_PIN,
        .speed_mode =   LEDC_HIGH_SPEED_MODE,
        .hpoint     =   0,
        .timer_sel  =   LEDC_TIMER_1,
    };

    arm_channel->spin = spin;
    arm_channel->flex = flex;
    arm_channel->updn = updn;
    arm_channel->claw = claw;

    ledc_channel_config(&(arm_channel->spin));
    ledc_channel_config(&(arm_channel->flex));
    ledc_channel_config(&(arm_channel->updn));
    ledc_channel_config(&(arm_channel->claw));
    return ESP_OK;
}


/**
 * @brief   机械臂旋转设置函数
 * @param   level       旋转程度(0~100)
 * @param   arm_channel 机械臂PWM通道
 * @return  esp_err_t
 */
esp_err_t arm_set_spin(uint8_t level, ARM_CHANNEL* arm_channel){
    ledc_set_duty((arm_channel->spin).speed_mode, 
                  (arm_channel->spin).channel,
                  (uint32_t)(level/100.0*LEDC_MAX_DUTY));
    ledc_update_duty((arm_channel->spin).speed_mode,
                     (arm_channel->spin).channel);  
    return ESP_OK;                             
}


/**
 * @brief   机械臂伸缩设置函数
 * @param   level       旋转程度(0~100)
 * @param   arm_channel 机械臂PWM通道
 * @return  esp_err_t
 */
esp_err_t arm_set_flex(uint8_t level, ARM_CHANNEL* arm_channel){
    ledc_set_duty((arm_channel->flex).speed_mode, 
                  (arm_channel->flex).channel,
                  (uint32_t)(level/100.0*LEDC_MAX_DUTY));
    ledc_update_duty((arm_channel->flex).speed_mode,
                     (arm_channel->flex).channel);  
    return ESP_OK;                 
}    


/**
 * @brief   机械臂升降设置函数
 * @param   level       旋转程度(0~100)
 * @param   arm_channel 机械臂PWM通道
 * @return  esp_err_t
 */
esp_err_t arm_set_updn(uint8_t level, ARM_CHANNEL* arm_channel){
    ledc_set_duty((arm_channel->updn).speed_mode, 
                  (arm_channel->updn).channel,
                  (uint32_t)(level/100.0*LEDC_MAX_DUTY));
    ledc_update_duty((arm_channel->updn).speed_mode,
                     (arm_channel->updn).channel);  
    return ESP_OK;                 
}   


/**
 * @brief   机械爪子设置函数
 * @param   level       旋转程度(0~100)
 * @param   arm_channel 机械臂PWM通道
 * @return  esp_err_t
 */
esp_err_t arm_set_claw(uint8_t level, ARM_CHANNEL* arm_channel){
    ledc_set_duty((arm_channel->claw).speed_mode, 
                  (arm_channel->claw).channel,
                  (uint32_t)(level/100.0*LEDC_MAX_DUTY));
    ledc_update_duty((arm_channel->claw).speed_mode,
                     (arm_channel->claw).channel);
    return ESP_OK;                   
}           