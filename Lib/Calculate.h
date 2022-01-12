/*
 * @Author:         Redns
 * @Date: 	        2021-11-04 16:24:48
 * @LastEditTime: 	2021-11-04 16:33:55
 * @Description:    此库包含FFT计算相关函数
 */

#ifndef __CALCULATE_H
#define __CALCULATE_H

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.1415926

typedef enum{
    WINDOW_HANNING,
    WINDOW_HAMMING,
    WINDOW_RECTANG
}WINDOW_TYPE;

void get_window(WINDOW_TYPE window_type, int window_len, float* window);

#endif
