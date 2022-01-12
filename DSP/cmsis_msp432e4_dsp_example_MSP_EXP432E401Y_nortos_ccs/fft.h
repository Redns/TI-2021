#ifndef __FFT_H
#define __FFT_H

#include "math.h"
#include "stdio.h"

#define PI 3.1415926

typedef float ElemType;

/* ���帴���ṹ�� */
typedef struct{
    ElemType real;
    ElemType imag;
}complex;


void     ChangeSeat(complex* input, int fft_len);          // ��ַ
complex  Mul_Complex(complex a, complex b);   // �����˷�
void     FFT(int fft_len, int fft_order, complex* data);                               // FFT
void     IFFT(int fft_len, int fft_order, complex* data);                              // IFFT

#endif