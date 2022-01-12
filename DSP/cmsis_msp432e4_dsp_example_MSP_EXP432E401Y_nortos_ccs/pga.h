#ifndef __PGA_H
#define __PGA_H

#define GPIO_EN1_PORT
#define GPIO_EN2_PORT
#define GPIO_EN1_PIN
#define GPIO_EN2_PIN

#define EN1_H()
#define EN1_L()
#define EN2_H()
#define EN2_L()

typedef enum{
    PGA_x1,
    PGA_x4,
    PGA_x10,
    PGA_x40
}PGA_RATE;


void PgaInit();                    // ��ʼ��PGA
void PgaSetRate(PGA_RATE rate);    // ����PGA����

#endif
