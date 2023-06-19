
#include"util/delay.h"
#include "../LIB/STDTYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/PORT_Driver/PORT_REG.h"
#include "../MCAL/PORT_Driver/PORT_Config.h"
#include "../MCAL/PORT_Driver/PORT_Private.h"
#include "../MCAL/PORT_Driver/PORT_Interface.h"
#include "../MCAL/DIO_Driver/DIO_REG.h"
#include "../MCAL/DIO_Driver/DIO_Interface.h"
/*HAL layard*/
#include "../HAL/LCD_Module/LCD_Config.h"
#include "../HAL/LCD_Module/LCD_Private.h"
#include "../HAL/LCD_Module/LCD_Interface.h"

#include "../HAL/Keypad_Module/Keypad_Config.h"
#include "../HAL/Keypad_Module/Keypad_Private.h"
#include "../HAL/Keypad_Module/Keypad_Interface.h"

#include "../HAL/DC_MOTOR/DC_MOTOR_Config.h"
#include "../HAL/DC_MOTOR/DC_MOTOR_Interface.h"

/*Stack implementation for Password with its Vars and Functions prototype*/
#define MAX_SIZE 4
s8 top=-1;
u8 item[MAX_SIZE];
void push(u8 Element);
void pop(void);
void Reset(void);
static u8  Global_u8OriginalPassWord[4]={1,2,3,4};
static u8  Global_u8TempPassWord[4];
static u8  Global_u8ConfirmPassWord[4];
static u8  Global_u8CheckError=0;
static u8  Global_u8FalseNumbers=0;
static u8  Global_u8Done=0;
static u8  Global_u8ConfirmDone=0;
static u8  Global_u8OpenCheck=0;

int main(void)
{
	PORT_VoidInit();
	LCD_VoidInit();
	u8 Local_u8Char_Pressed;

		 while(1)
		 {
			 do
			 {
				 Local_u8Char_Pressed=Keypad_u8GetPressedKey();

			 }while(Local_u8Char_Pressed==0xff);

			 /*to hide the Entering Password with char '*' */
			 if(Local_u8Char_Pressed!='E'&&Local_u8Char_Pressed!='D'&&Local_u8Char_Pressed!='N'&&Local_u8Char_Pressed!='C'&&Local_u8Char_Pressed!='R'&&Local_u8Char_Pressed!=0xff)
			 {
              push(Local_u8Char_Pressed-'0');
				 LCD_VoidSendData('*');
			 }



		 switch(Local_u8Char_Pressed)
		 {


		 		 case 'D':
						 {
							pop();
							LCD_VoidSendCommand(1);
							 for(u8 i=0;i<=top;i++)
							 {
								LCD_VoidSendData('*');
							 }

							break;

						 }
				/*To lock the door of Safe if oridy locked it tell you */
		 		 case 'C':
						 {if(Global_u8OpenCheck==1)
						 {	Global_u8OpenCheck=0;
							 Global_u8Done=0;
							 LCD_VoidSendString("Locking.....");
							DC_MOTOR_VidRotateClockWise();
							  _delay_ms(2000);
						    DC_MOTOR_VidStop();
						   Reset();
						 }
						 else
						 {
							 LCD_VoidSendString("Oridy,Locked");
							 _delay_ms(2000);
							  Reset();
						 }
						 break;
						 }

				/*to Check if Password Correct or not to open the door and if it is Incorrect you have 3 trials only
				 after these trials and you enter false PW you trials Expired and Cant Enter again  */
		 		 case 'E':
					    {
							 for(u8 i=0;i<4;i++)
							 {
								if(item[i]==Global_u8OriginalPassWord[i])
								{
									Global_u8CheckError++;
								}
							 }
							 if(Global_u8CheckError==4)
							 {
								 if(Global_u8OpenCheck==0)
								 {
								 LCD_VoidClearDisplay();
								LCD_VoidSendString("Welcome");
								Global_u8FalseNumbers=0;
								Global_u8OpenCheck=1;
								DC_MOTOR_VidRotateUntiClockWise();
								_delay_ms(2000);
								DC_MOTOR_VidStop();
								 Reset();
								 Global_u8Done=1;
								 }
								 else
								 {
									 LCD_VoidClearDisplay();
									 LCD_VoidSendString("Oridy,Opened");
									 _delay_ms(2000);
									 LCD_VoidClearDisplay();
									  Global_u8Done=1;
								 }
							 }
							 else
							 {
								 if(Global_u8OpenCheck==0)
							     {
								 Reset();
								 LCD_VoidSendString("False,Try Again!");
								 _delay_ms(1500);
								 LCD_VoidClearDisplay();
								 Global_u8FalseNumbers++;
								 }
								 else
								 {
									 LCD_VoidClearDisplay();
									 LCD_VoidSendString("Oridy,Opened");
									 _delay_ms(2000);
									 LCD_VoidClearDisplay();

								 }
							 }

							if(Global_u8FalseNumbers==3)
							{  Global_u8FalseNumbers=0;
								Reset();
								//LCD_VoidSendString("Your Tries is Expired");
								for(u8 k=0;k<5;k++)
								{
									DIO_U8SetPinVal(PORTD,PIN5,HIGH);
									LCD_VoidSendString("Your Tries is Expired");
									_delay_ms(250);
									 LCD_VoidClearDisplay();
									DIO_U8SetPinVal(PORTD,PIN5,LOW);
									_delay_ms(250);
								}

							}

							break;


					 }

				/*to Change the Password to new Number and Confirm it to assure and
				 f the New PW  and its confirm are same ,it tell you that the process Successfuly done if not ,tell two that Failed Process*/
		 		 case 'N':
						 {

							 if(Global_u8Done)
							 {   Global_u8Done=0;
							 	 LCD_VoidClearDisplay();
								 LCD_VoidSendString("Enter the New Password :");

								// LCD_VoidClearDisplay();
								 for(u8 l=0;l<4;l++)
								 {
										 do
									 {
										 Local_u8Char_Pressed=Keypad_u8GetPressedKey();

									 }while(Local_u8Char_Pressed==0xff);

									 if(Local_u8Char_Pressed!='E'&&Local_u8Char_Pressed!='D'&&Local_u8Char_Pressed!='N'&&Local_u8Char_Pressed!='C'&&Local_u8Char_Pressed!='R')
									 {
										 Global_u8TempPassWord[l]=Local_u8Char_Pressed-'0';
										 LCD_VoidSendData('*');
									 }

								 }
								 LCD_VoidClearDisplay();
								 LCD_VoidSendString("Enter the Confirm Password :");

								// LCD_VoidClearDisplay();
								 for(u8 l=0;l<4;l++)
								 {
										 do
									 {
										 Local_u8Char_Pressed=Keypad_u8GetPressedKey();

									 }while(Local_u8Char_Pressed==0xff);

									 if(Local_u8Char_Pressed!='E'&&Local_u8Char_Pressed!='D'&&Local_u8Char_Pressed!='N'&&Local_u8Char_Pressed!='C'&&Local_u8Char_Pressed!='R')
									 {
										 Global_u8ConfirmPassWord[l]=Local_u8Char_Pressed-'0';
										 LCD_VoidSendData('*');
									 }

								 }
								 LCD_VoidClearDisplay();
								 for(u8 l=0;l<4;l++)
								 {
									 if(Global_u8ConfirmPassWord[l]==Global_u8TempPassWord[l])
									 {
										 Global_u8ConfirmDone++;

									 }
									// LCD_VoidSendString('a');
								 }
								 if(Global_u8ConfirmDone==4)
								 {


									 for(u8 l=0;l<4;l++)
									 {
										 Global_u8OriginalPassWord[l]= Global_u8TempPassWord[l];
									 }
									 LCD_VoidSendString("Succesfully,Done");
									 _delay_ms(2500);
									 Reset();

								 }
								 else
								 {
									 LCD_VoidSendString("Sorry, Refused");
									 _delay_ms(2500);
									 Reset();
								 }


							 }
							 else
							 {
								 LCD_VoidClearDisplay();
								 LCD_VoidSendString("you must open the lock,firstly");
							 }
							break;
						 }

				 /*To rest the system*/
		 		 case 'R':
						 {
							Reset();
							break;
						 }

		 	 }


		 }

	return 0;

}





void Reset()
{
	top=-1;
	 Global_u8CheckError=0;
    // Global_u8FalseNumbers=0;
	 Global_u8Done=0;
	 Global_u8ConfirmDone=0;

	 LCD_VoidClearDisplay();

}


u8 isEmpty()
{
	return top < 0;
}

void push(u8 Element)
{
	if (top >= MAX_SIZE-1)
	{
		LCD_VoidSendString("Stack Full");
	}
	else
	{
		top++;
		item[top] = Element;
	}
}
void pop()
{
	if (isEmpty())
		LCD_VoidSendString("No Data deleted");
	else
		top--;
}

