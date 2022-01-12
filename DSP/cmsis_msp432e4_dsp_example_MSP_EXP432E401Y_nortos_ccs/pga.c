#include "pga.h"

void PgaInit(){
    PgaSetRate(PGA_x1);
}


void PgaSetRate(PGA_RATE rate){
    switch(rate){
        case PGA_x1:
            EN1_L();
            EN2_L();
            break;

        case PGA_x4:
            EN1_H();
            EN2_L();
            break;

        case PGA_x10:
            EN1_L();
            EN2_H();
            break;

        case PGA_x40:
            EN1_H();
            EN2_H();
            break;

        default:
            EN1_L();
            EN2_L();
            break;
    }
}
