/*
 * LCD_Config.h
 *
 *  Created on: Sep 24, 2021
 *      Author: dell
 */

#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

/*to select Number of lines in LCD*/
#define NUM_LINES  LINE2


/*the Configuration Pins in LCD*/
#define LCD_RS_PIN  PIN0
#define LCD_R_W_PIN PIN1
#define LCD_EN_PIN  PIN2


/*to select the mode of LCD 4 Bit Or 8 Bit*/

#define LCD_MODE BIT_8


/*the selected PORT*/
#define LCD_DATA_PORT   PORTC
#define LCD_Config_PORT PORTD


#endif /* LCD_CONFIG_H_ */
