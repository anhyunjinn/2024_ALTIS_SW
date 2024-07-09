#ifndef interface_H
#define interface_H
#include <Arduino.h>
#include "pinmap.h"

void interface()
{
    pinMode(buzzer_pin, OUTPUT);
    pinMode(led1_pin, OUTPUT);
    pinMode(led2_pin, OUTPUT);
}

void interface(int state)
{
    // 0: 버저 + LED1 + LED2 켜기
    // 1: LED1 켜기
    // 2: LED2 켜기
    // 3: 버저 켜기

    // 버저 제어
    if (state == 0 || state == 3)
    {
        digitalWrite(buzzer_pin, HIGH);
    }
    else
    {
        digitalWrite(buzzer_pin, LOW);
    }

    // LED1 제어
    if (state == 0 || state == 1)
    {
        digitalWrite(led1_pin, HIGH);
    }
    else
    {
        digitalWrite(led1_pin, LOW);
    }

    // LED2 제어
    if (state == 0 || state == 2)
    {
        digitalWrite(led2_pin, HIGH);
    }
    else
    {
        digitalWrite(led2_pin, LOW);
    }
}

#endif