#include "fft.h"

void ChangeSeat(complex* input, int fft_len){

    int nextValue, nextM, i, k, j = 0;
    complex temp;

    /* 变址运算，即把自然顺序变成倒位序，采用雷德算法 */
    nextValue = fft_len/2;
    nextM = fft_len - 1;
    for(i = 0; i < nextM; i++){

        // 如果i<j,即进行变址
        if(i < j){
           temp     = input[j];
           input[j] = input[i];
           input[i] = temp;
        }

        k = nextValue;
        while(k <= j){
           j = j - k;
           k = k/2;
        }
        j = j + k;
    }
}


complex Mul_Complex(complex a, complex b){
    complex temp;

    temp.real = a.real * b.real - a.imag * b.imag;
    temp.imag = b.imag * a.real + a.imag * b.real;

    return temp;
}


void FFT(int fft_len, int fft_order, complex* data){
    int L = 0, B = 0, J = 0, K = 0;
    int step = 0;
    ElemType P = 0;
    complex W, Temp_XX;

    ChangeSeat(data, fft_len);
    for(L = 1; L <= fft_order; L++){
        B = 1<<(L - 1);

        for(J = 0; J <= B - 1; J++){
            P = (1<<(fft_order-L))*J;
            step = 1<<L;

            for(K = J; K <= fft_len-1; K = K + step){
                W.real =  cos(2*PI*P/fft_len);
                W.imag = -sin(2*PI*P/fft_len);

                Temp_XX = Mul_Complex(data[K+B], W);
                data[K+B].real = data[K].real - Temp_XX.real;
                data[K+B].imag = data[K].imag - Temp_XX.imag;

                data[K].real = data[K].real + Temp_XX.real;
                data[K].imag = data[K].imag + Temp_XX.imag;
            }
        }
    }
}


void IFFT(int fft_len, int fft_order, complex* data){
    int L = 0, B = 0, J = 0, K = 0;
    int step=0;
    ElemType P=0;
    complex W, Temp_XX;

    ChangeSeat(data, fft_len);
    for(L = 1; L <= fft_order; L++){
        B = 1<<(L-1);

        for(J = 0; J <= B-1; J++){
            P = (1<<(fft_order - L))*J;
            step = 1<<L;

            for(K = J; K <= fft_len - 1; K = K+step){
                W.real =  cos(2*PI*P/fft_len);
                W.imag =  sin(2*PI*P/fft_len);

                Temp_XX = Mul_Complex(data[K+B], W);
                data[K+B].real = data[K].real - Temp_XX.real;
                data[K+B].imag = data[K].imag - Temp_XX.imag;

                data[K].real = data[K].real + Temp_XX.real;
                data[K].imag = data[K].imag + Temp_XX.imag;
            }
        }
    }
}
