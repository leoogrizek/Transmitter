/*
 * joystick.h
 *
 * Created: 15. 05. 2023 23:24:17
 *  Author: Leo
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

void ADC_init();
uint8_t ADC_read(uint8_t ch);
void read_joystick_values(uint8_t* joystick_values);



#endif /* JOYSTICK_H_ */