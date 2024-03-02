/* 
 * File:   application.h
 * Author: Ahmed Basem
 * LinkedIn : https://www.linkedin.com/in/ahmed-basem-988529272/
 * Created on 25 december 2023, 02:23
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/* Section : Includes */
#include "ECU_Layer/ecu_layer_init.h"
//#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
//#include "MCAL_Layer/EEPROM/hal_eeprom.h"
//#include "MCAL_Layer/ADC/hal_adc.h"
//#include "MCAL_Layer/Timer/hal_timer0.h"
//#include "MCAL_Layer/Timer/hal_timer1.h"
//#include "MCAL_Layer/Timer/hal_timer2.h"
#include "MCAL_Layer/Timer/hal_timer3.h"
#include "MCAL_Layer/CCP/hal_ccp.h"

/* Section: Macro Declarations */

/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */

/* Section: Function Declarations */
void application_intialize(void);
uint8 Password_check();
void password_taking();

#endif	/* APPLICATION_H */

