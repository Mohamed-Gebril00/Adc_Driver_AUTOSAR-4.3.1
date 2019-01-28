/*
 * button.h
 *
 *  Created on: Jan 27, 2019
 *      Author: Gebro
 */

#ifndef BUTTON_H_
#define BUTTON_H_
#include "E:\TIVA c\projects\ADC_17_9\ADC.h"
#include "button_cfg.h"

typedef void(*BUTTON_FnPtrType)(void);
typedef enum{Button_NotPressed=0, Resistance_burnt, Button_Pressed}Button_StateType;
typedef enum{Button_OK=0, Button_E_PARAM_GROUP,Button_E_ADC}Button_FunctionValidation;

typedef struct{
    uint8_t AdcGroupIdx;
		uint8_t Buttons_Number;
    Button_StateType Button_State;
    // BUTTON_FnPtrType CbkFnPtr;

}ButtonGroup;

Button_FunctionValidation Read_Button(uint8_t Button_Group, Button_StateType* Button_PTRState);


#endif /* BUTTON_H_ */
