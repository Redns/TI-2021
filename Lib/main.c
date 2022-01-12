#include <stdio.h>
#include "Calculate.h"

int main(){
    float* window = get_window(WINDOW_HAMMING, 100);
    for(int i = 0; i < 100; i++){
        printf("%.5f,", window[i]);
    }

    return 0;
}