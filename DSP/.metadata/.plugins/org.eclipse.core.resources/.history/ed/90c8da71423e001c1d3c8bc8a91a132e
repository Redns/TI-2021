#include "Calculate.h"

float* get_window(WINDOW_TYPE window_type, int window_len){
    float* window = (float*)malloc(sizeof(float)*window_len);

    switch(window_type){
        // hanning
        case WINDOW_HANNING:
            for(int n = 0; n < window_len; n++){
                window[n] = 0.5*(1 - cos(2*PI*n/(window_len - 1)));
            }
            break;

        // hamming
        case WINDOW_HAMMING:
            for(int n = 0; n < window_len; n++){
                window[n] = 0.54 - 0.46*cos(2*PI*n/(window_len - 1));
            }
            break;

        // rectange
        case WINDOW_RECTANG:
            memset(window, 1, sizeof(window));
            break;

        default:
            memset(window, 0, sizeof(window));            
    }

	return window;
}