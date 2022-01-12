#include "l298n.h"

/**
 * @brief   l298n模块初始化  
 * @param   l298n_pwm_channel   l298n模块对应PWM通道结构体
 * @return  none
 */
void l298n_init(L298N_PWM_CHANNEL* l298n_pwm_channel){
    //设置相应端口为GPIO
    gpio_pad_select_gpio(L298N_IN1);
    gpio_pad_select_gpio(L298N_IN2);
    gpio_pad_select_gpio(L298N_IN3);
    gpio_pad_select_gpio(L298N_IN4);

    //设置GPIO模式为输出
    gpio_set_direction(L298N_IN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(L298N_IN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(L298N_IN3, GPIO_MODE_OUTPUT);
    gpio_set_direction(L298N_IN4, GPIO_MODE_OUTPUT);

    //初始化GPIO电平
    L298N_IN1_LOW();
    L298N_IN2_LOW();
    L298N_IN3_LOW();
    L298N_IN4_LOW();

    //初始化PWM
    ledc_timer_config_t l298n_pwm_config = {
        .duty_resolution    =   LEDC_TIMER_13_BIT,      //PWM分辨率(13bit)
        .freq_hz            =   5000,                   //频率
        .speed_mode         =   LEDC_HIGH_SPEED_MODE,   //速度
        .timer_num          =   LEDC_TIMER_0,           //选择定时器0
        .clk_cfg            =   LEDC_AUTO_CLK,          //自动选择时钟源
    };
    ledc_timer_config(&l298n_pwm_config);

    //配置PWM通道
    (l298n_pwm_channel->ENA).channel       =   LEDC_CHANNEL_0;         //PWM通道
    (l298n_pwm_channel->ENA).duty          =   0;                      //占空比
    (l298n_pwm_channel->ENA).gpio_num      =   L298N_ENA;              //IO映射
    (l298n_pwm_channel->ENA).speed_mode    =   LEDC_HIGH_SPEED_MODE;   //速度
    (l298n_pwm_channel->ENA).hpoint        =   0;
    (l298n_pwm_channel->ENA).timer_sel     =   LEDC_TIMER_0;           //选择定时器

    (l298n_pwm_channel->ENB).channel       =   LEDC_CHANNEL_1;         //PWM通道
    (l298n_pwm_channel->ENB).duty          =   0;                      //占空比
    (l298n_pwm_channel->ENB).gpio_num      =   L298N_ENB;              //IO映射
    (l298n_pwm_channel->ENB).speed_mode    =   LEDC_HIGH_SPEED_MODE;   //速度
    (l298n_pwm_channel->ENB).hpoint        =   0;
    (l298n_pwm_channel->ENB).timer_sel     =   LEDC_TIMER_0;           //选择定时器

    ledc_channel_config(&(l298n_pwm_channel->ENA));
    ledc_channel_config(&(l298n_pwm_channel->ENB));
}


/**
 * @brief:  设置运动方向  
 * @param   direction   设定的方向, 以字符形式表示
 *                      'F':前进
 *                      'B':后退
 *                      'L':左转
 *                      'R':右转
 *                      'S':停止
 *                      'M':逆时针旋转
 *                      'W':顺时针旋转
 * @return  none
 */
void l298n_set_direction(uint8_t direction){
    switch(direction){
        //前进
        case 'F':
            L298N_IN1_HIGH();
            L298N_IN2_LOW();
            L298N_IN3_HIGH();
            L298N_IN4_LOW();
            break;

        //后退
        case 'B':
            L298N_IN1_LOW();
            L298N_IN2_HIGH();
            L298N_IN3_LOW();
            L298N_IN4_HIGH();
            break;

        //左转
        case 'L':
            L298N_IN1_HIGH();
            L298N_IN2_LOW();
            L298N_IN3_HIGH();
            L298N_IN4_LOW();
            break;   

        //右转     
        case 'R':
            L298N_IN1_HIGH();
            L298N_IN2_LOW();
            L298N_IN3_HIGH();
            L298N_IN4_LOW();
            break;

        //停止    
        case 'S':
            L298N_IN1_LOW();
            L298N_IN2_LOW();
            L298N_IN3_LOW();
            L298N_IN4_LOW();
            break;

        //逆时针旋转    
        case 'M':
            L298N_IN1_LOW();
            L298N_IN2_HIGH();
            L298N_IN3_HIGH();
            L298N_IN4_LOW();
            break;

        //顺时针旋转    
        case 'W':
            L298N_IN1_HIGH();
            L298N_IN2_LOW();
            L298N_IN3_LOW();
            L298N_IN4_HIGH();
            break;

        default:
            L298N_IN1_LOW();
            L298N_IN2_LOW();
            L298N_IN3_LOW();
            L298N_IN4_LOW();
            break;    
    }
}


/**
 * @brief   设置速度
 * @param   direction           小车当前的方向
 * @param   speed               设置的速度值(0 ~ 100)
 * @param   l298n_pwm_channel   l298n模块PWM对应通道
 * @return  none
 */
void l298n_set_speed(uint8_t direction,
                     uint8_t speed, 
                     L298N_PWM_CHANNEL* l298n_pwm_channel){                   
    switch(direction){
        //前进
        case 'F':
            ledc_set_duty((l298n_pwm_channel->ENA).speed_mode, 
                           (l298n_pwm_channel->ENA).channel,
                           (uint32_t)(speed/100.0*LEDC_MAX_DUTY));
            ledc_update_duty((l298n_pwm_channel->ENA).speed_mode,
                             (l298n_pwm_channel->ENA).channel);

            ledc_set_duty((l298n_pwm_channel->ENB).speed_mode, 
                           (l298n_pwm_channel->ENB).channel,
                           (uint32_t)(speed/100.0*LEDC_MAX_DUTY));
            ledc_update_duty((l298n_pwm_channel->ENB).speed_mode,
                             (l298n_pwm_channel->ENB).channel);     
            break;             

        //后退
        case 'B':
            ledc_set_duty((l298n_pwm_channel->ENA).speed_mode, 
                           (l298n_pwm_channel->ENA).channel,
                           (uint32_t)(speed/100.0*LEDC_MAX_DUTY));
            ledc_update_duty((l298n_pwm_channel->ENA).speed_mode,
                             (l298n_pwm_channel->ENA).channel);

            ledc_set_duty((l298n_pwm_channel->ENB).speed_mode, 
                           (l298n_pwm_channel->ENB).channel,
                           (uint32_t)(speed/100.0*LEDC_MAX_DUTY));
            ledc_update_duty((l298n_pwm_channel->ENB).speed_mode,
                             (l298n_pwm_channel->ENB).channel);    
            break;

        //左转
        //speed     ENA_Duty(%)    ENB_Duty(%)
        //  0           0              0
        // 100          50             100   
        case 'L':
            ledc_set_duty((l298n_pwm_channel->ENA).speed_mode, 
                           (l298n_pwm_channel->ENA).channel,
                           (uint32_t)(speed/200.0*LEDC_MAX_DUTY));
            ledc_update_duty((l298n_pwm_channel->ENA).speed_mode,
                             (l298n_pwm_channel->ENA).channel);

            ledc_set_duty((l298n_pwm_channel->ENB).speed_mode, 
                           (l298n_pwm_channel->ENB).channel,
                           (uint32_t)(speed/100.0*LEDC_MAX_DUTY));
            ledc_update_duty((l298n_pwm_channel->ENB).speed_mode,
                             (l298n_pwm_channel->ENB).channel);
            break;

        //右转
        case 'R':
            ledc_set_duty((l298n_pwm_channel->ENA).speed_mode, 
                           (l298n_pwm_channel->ENA).channel,
                           (uint32_t)(speed/100.0*LEDC_MAX_DUTY));
            ledc_update_duty((l298n_pwm_channel->ENA).speed_mode,
                             (l298n_pwm_channel->ENA).channel);

            ledc_set_duty((l298n_pwm_channel->ENB).speed_mode, 
                           (l298n_pwm_channel->ENB).channel,
                           (uint32_t)(speed/200.0*LEDC_MAX_DUTY));
            ledc_update_duty((l298n_pwm_channel->ENB).speed_mode,
                             (l298n_pwm_channel->ENB).channel);
            break;

        //逆时针旋转    
        case 'M':
            ledc_set_duty((l298n_pwm_channel->ENA).speed_mode, 
                           (l298n_pwm_channel->ENA).channel,
                           (uint32_t)(speed/100.0*LEDC_MAX_DUTY));
            ledc_update_duty((l298n_pwm_channel->ENA).speed_mode,
                             (l298n_pwm_channel->ENA).channel);

            ledc_set_duty((l298n_pwm_channel->ENB).speed_mode, 
                           (l298n_pwm_channel->ENB).channel,
                           (uint32_t)(speed/100.0*LEDC_MAX_DUTY));
            ledc_update_duty((l298n_pwm_channel->ENB).speed_mode,
                             (l298n_pwm_channel->ENB).channel);
            break;

        //顺时针旋转
        case 'W':
            ledc_set_duty((l298n_pwm_channel->ENA).speed_mode, 
                           (l298n_pwm_channel->ENA).channel,
                           (uint32_t)(speed/100.0*LEDC_MAX_DUTY));
            ledc_update_duty((l298n_pwm_channel->ENA).speed_mode,
                             (l298n_pwm_channel->ENA).channel);

            ledc_set_duty((l298n_pwm_channel->ENB).speed_mode, 
                           (l298n_pwm_channel->ENB).channel,
                           (uint32_t)(speed/100.0*LEDC_MAX_DUTY));
            ledc_update_duty((l298n_pwm_channel->ENB).speed_mode,
                             (l298n_pwm_channel->ENB).channel);
            break;

        default:
            ledc_set_duty((l298n_pwm_channel->ENA).speed_mode, 
                           (l298n_pwm_channel->ENA).channel,
                           0);
            ledc_update_duty((l298n_pwm_channel->ENA).speed_mode,
                             (l298n_pwm_channel->ENA).channel);

            ledc_set_duty((l298n_pwm_channel->ENB).speed_mode, 
                           (l298n_pwm_channel->ENB).channel,
                           0);
            ledc_update_duty((l298n_pwm_channel->ENB).speed_mode,
                             (l298n_pwm_channel->ENB).channel);
            break;                    
    }
}
