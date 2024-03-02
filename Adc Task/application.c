#include "application.h"
#include "MCAL_Layer/ADC/hal_adc.h"
#include "MCAL_Layer/Timer/hal_timer2.h"
dc_motor_t motor = {
  .dc_motor_pin[0] = {PORTA_INDEX,GPIO_PIN1,GPIO_DIRECTION_OUTPUT,GPIO_LOW},  
  .dc_motor_pin[1] = {PORTA_INDEX,GPIO_PIN2,GPIO_DIRECTION_OUTPUT,GPIO_LOW},  
};
keypad_t KeyPad = {
    .keypad_row_pins[0] = {PORTD_INDEX,GPIO_PIN0,GPIO_DIRECTION_OUTPUT,GPIO_LOW},
    .keypad_row_pins[1] = {PORTD_INDEX,GPIO_PIN1,GPIO_DIRECTION_OUTPUT,GPIO_LOW},
    .keypad_row_pins[2] = {PORTD_INDEX,GPIO_PIN2,GPIO_DIRECTION_OUTPUT,GPIO_LOW},
    .keypad_row_pins[3] = {PORTD_INDEX,GPIO_PIN3,GPIO_DIRECTION_OUTPUT,GPIO_LOW},
    .keypad_columns_pins[0] = {PORTD_INDEX,GPIO_PIN4,GPIO_DIRECTION_INPUT},
    .keypad_columns_pins[1] = {PORTD_INDEX,GPIO_PIN5,GPIO_DIRECTION_INPUT},
    .keypad_columns_pins[2] = {PORTD_INDEX,GPIO_PIN6,GPIO_DIRECTION_INPUT},
    .keypad_columns_pins[3] = {PORTD_INDEX,GPIO_PIN7,GPIO_DIRECTION_INPUT},
};
chr_lcd_4bit_t lcd = {
	.lcd_rs.port = PORTC_INDEX,
	.lcd_rs.pin = GPIO_PIN6,
	.lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
	.lcd_rs.logic = GPIO_LOW,
	.lcd_en.port = PORTC_INDEX,
	.lcd_en.pin = GPIO_PIN7,
	.lcd_en.direction = GPIO_DIRECTION_OUTPUT,
	.lcd_en.logic = GPIO_LOW,
	.lcd_data[0].port = PORTC_INDEX,
	.lcd_data[0].pin = GPIO_PIN0,
	.lcd_data[0].direction = GPIO_DIRECTION_OUTPUT,
	.lcd_data[0].logic = GPIO_LOW,
	.lcd_data[1].port = PORTC_INDEX,
	.lcd_data[1].pin = GPIO_PIN1,
	.lcd_data[1].direction = GPIO_DIRECTION_OUTPUT,
	.lcd_data[1].logic = GPIO_LOW,
	.lcd_data[2].port = PORTA_INDEX,
	.lcd_data[2].pin = GPIO_PIN3,
	.lcd_data[2].direction = GPIO_DIRECTION_OUTPUT,
	.lcd_data[2].logic = GPIO_LOW,
	.lcd_data[3].port = PORTC_INDEX,
	.lcd_data[3].pin = GPIO_PIN3,
	.lcd_data[3].direction = GPIO_DIRECTION_OUTPUT,
	.lcd_data[3].logic = GPIO_LOW
};
led_t Yellow_Led = {
    .port_name = PORTC_INDEX,
    .pin = GPIO_PIN5,
    .led_status = LED_OFF,
};
led_t Red_Led = {
    .port_name = PORTC_INDEX,
    .pin = GPIO_PIN4,
    .led_status = LED_OFF,
};
adc_conf_t adc = {
  .ADC_InterruptHandler = NULL,
  .priority = INTERRUPT_LOW_PRIORITY,
  .acquisition_time = ADC_12_TAD,
  .adc_channel = ADC_CHANNEL_AN0,
  .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
  .result_format = ADC_RESULT_RIGHT,
  .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLED
};
ccp_t ccp1 = {
  .CCP1_InterruptHandler = NULL,
  .CCP1_priority = INTERRUPT_LOW_PRIORITY,
  .ccp_mode = CCP_PWM_MODE_SELECTED,
  .PWM_Frequency = 2000,
  .ccp_pin.port = PORTC_INDEX,
  .ccp_pin.pin = GPIO_PIN2,
  .ccp_pin.direction = GPIO_DIRECTION_OUTPUT,
  .ccp_inst = CCP1_INST,
  .timer2_prescaler_value = 1,
  .timer2_postscaler_value = 1,
};
timer2_t Timer_obj = {
  .timer2_prescaler_value = TIMER2_PRESCALER_DIV_BY_1,
  .timer2_postscaler_value = TIMER2_POSTSCALER_DIV_BY_1,
  .timer2_preload_value = 0
};
uint8 System_Password[6] = "102030";
uint8 User_Password[6] = "";
uint8 Password_Index = 0;
uint8 string1[] = "Enter The Password :";
uint8 Pad_value = 0;
uint8 password_flag = 0;
uint8 lm35_Read = 0;
uint8 reading[] = "";
int main() { 
    Std_ReturnType ret = E_NOT_OK;  
    application_intialize();
    ret = lcd_4bit_send_string_pos(&lcd,1,1,string1);
    ret = led_turn_on(&Red_Led);
    while(1){   
        if(password_flag == 0)
        {
            password_taking();
            if(Password_Index == 6)
            {
                password_flag = Password_check();
                if(password_flag == 0){
                    led_turn_on(&Yellow_Led);
                    led_turn_off(&Red_Led);
                    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
                    ret = lcd_4bit_send_string(&lcd," Hello Home ");
                    __delay_ms(2000);
                    ret = lcd_4bit_send_command(&lcd,_LCD_CLEAR);
                    ret = lcd_4bit_send_char_data(&lcd," ");
                    ret = lcd_4bit_send_string_pos(&lcd,1,1,"Temp : ");
                    password_flag = 1;
                }
                else
                {
                    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
                    ret = lcd_4bit_send_string_pos(&lcd,1,1,string1);
                    Password_Index = 0;
                    password_flag = 0;
                }
            } 
        }
        else
        {
            ret = ADC_GetConversion_Blocking(&adc,ADC_CHANNEL_AN0,&lm35_Read);
            lm35_Read *= 0.488f;
            ret = CCP_PWM_Set_Duty(&ccp1,lm35_Read);
            if(lm35_Read >= 30) dc_motor_move_right(&motor);
            else dc_motor_stop(&motor);
            ret = convert_uint8_to_string(lm35_Read,reading);
            ret = lcd_4bit_send_string_pos(&lcd,1,8,reading);
        }
    }
    return (EXIT_SUCCESS);
}

void application_intialize(void){
    Std_ReturnType ret = E_NOT_OK;
    ret = keypad_initialize(&KeyPad);
    ret = lcd_4bit_intialize(&lcd);
    ret = led_initialize(&Red_Led);
    ret = led_initialize(&Yellow_Led);
    ret = ADC_Init(&adc);
    ret = dc_motor_initialize(&motor);
    ret = Timer2_Init(&Timer_obj);
    ret = CCP_Init(&ccp1);
    ret = CCP_PWM_Set_Duty(&ccp1,0);
    ret = CCP_PWM_Start(&ccp1);
}
uint8 Password_check()
{
uint8 password_flag = 0;
for(uint8 i = 0; i < 6; i++){
    if(System_Password[i] == User_Password[i])
    {
        /*nothing*/
    }
    else{
        password_flag = 1;
    }
}
return password_flag;
}

void password_taking()
{
while(Pad_value == '\0'){
    keypad_get_value(&KeyPad,&Pad_value);
    }
    if(Pad_value == '#' && Password_Index>0){
        Password_Index--;
        lcd_4bit_send_char_data_pos(&lcd,2,Password_Index,' ');
        Pad_value = '\0';
    }
    else{
        User_Password[Password_Index] = Pad_value;
        Password_Index++;
        lcd_4bit_send_char_data_pos(&lcd,2,Password_Index,'*');
        Pad_value = '\0';
    }
}