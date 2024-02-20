#include "application.h"
#include "ECU_Layer/Chr_LCD/ecu_chr_lcd.h"
#include "MCAL_Layer/ADC/hal_adc.h"

led_t led = {
  .port_name = PORTC_INDEX,
  .pin = GPIO_PIN0,
  .led_status = LED_OFF
};
button_t btn = {
  .button_pin.port = PORTC_INDEX,
  .button_pin.pin = GPIO_PIN1,
  .button_pin.direction = GPIO_DIRECTION_INPUT,
  .button_pin.logic = GPIO_LOW,
  .button_connection = BUTTON_ACTIVE_HIGH,
  .button_state = BUTTON_RELEASED,
};
uint8 value = 0;
int main() { 
    Std_ReturnType ret = E_NOT_OK;
    ret = led_initialize(&led);
    ret = button_initialize(&btn);
    application_intialize();
    while(1){
        ret = button_read_state(&btn,&value);
        if(value == BUTTON_PRESSED){
            ret = led_turn_on(&led);
        }
        else{
            ret = led_turn_off(&led);
        }
    }
    return (EXIT_SUCCESS);
}

void application_intialize(void){
    Std_ReturnType ret = E_NOT_OK;
    ecu_layer_intialize();
}