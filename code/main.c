
#include "avr/io.h"
#include "avr/interrupt.h"


#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"


#include "DIO_int.h"
#include "ADC_int.h"
u8 wanted;
u16 u16AdVal;
float desired=0.0;
float  error = 0.0;
float action;
float serror=0.0;
float deferror=0.0;
float prev_error=0.0;
float final_error=0.0;
u8 t1 = 0;
u8 t2 = 0;
u8 t = 0;
float actual_speed=0.0;
float kp = 0.8;
float ki = 0.2;
float kd = 0.4;
u8 flag = 0 ;
u8 overflows=0;

void main(void)
{
	/* ADC PIN0 */
	DIO_SetPinDirection(PORTA,PIN0,INPUT);
	/* PWM PIN OC0 */
	DIO_SetPinDirection(PORTB,PIN3,OUTPUT);
	DIO_SetPinValue(PORTB,PIN3,LOW);

	//emergency stop button at  PORTD PIN2
	DIO_SetPinDirection(PORTD,PIN2,INPUT);
	DIO_SetPinValue(PORTD,PIN2,HIGH);

	//emergency stop button at  PORTD PIN1
		DIO_SetPinDirection(PORTD,PIN3,INPUT);
		DIO_SetPinValue(PORTD,PIN3,HIGH);

	/*start button*/
	DIO_SetPinDirection(PORTD,PIN0,INPUT);
	/*pull up */
	DIO_SetPinValue(PORTD,PIN0,HIGH);
	/*enable at   PORTC PIN0*/
	DIO_SetPinDirection(PORTC,PIN0,OUTPUT);
	/*enable high*/
	DIO_SetPinValue(PORTC,PIN0,HIGH);



	ADC_vidInit();

	/*interrupt 0 & 1 configuration*/

		MCUCR |= 0x0A;

		GICR |= 0xC0;


	/*TIMER 0  */

	/* Fast PWM */
	SET_BIT(TCCR0,WGM00);
	SET_BIT(TCCR0,WGM01);

	/* CLR OC0 on CMP and SET on TOP */
	CLR_BIT(TCCR0,COM00);
	SET_BIT(TCCR0,COM01);

	/* CLK Prescaler 64 */
	SET_BIT(TCCR0,CS00);
	SET_BIT(TCCR0,CS01);
	CLR_BIT(TCCR0,CS02);

	/*TIMER 2 */


	/*Normal Mode*/
	CLR_BIT(TCCR2,WGM20);
	CLR_BIT(TCCR2,WGM21);

	/*initiallization */
	TCNT2 =256-62;

	/* CLK Prescaler 64 */
	CLR_BIT(TCCR2,CS20);
	CLR_BIT(TCCR2,CS21);
	SET_BIT(TCCR2,CS22);

	/* Enable Interrupt of OVF */
	SET_BIT(TIMSK,TOIE2);


	/*enable global interrupt*/
	sei();

	u16AdVal = ADC_u16GetAdValue(ADC_CHANNEL_0);
	
	desired=(u8)(((u32)u16AdVal * 100)/1023) ;




	OCR0 =(u8) (desired * 255)/100;




	while(1)
	{

		if(DIO_GetPinValue(PORTD,PIN0)==0){
		DIO_SetPinValue(PORTC,PIN0,HIGH);

	}

	}
}


ISR(INT0_vect)
{

	DIO_SetPinValue(PORTC,PIN0,LOW);
}

ISR(INT1_vect)
{

	if (flag==0)
	{
		t1 = TCNT2;
		flag=1;
	}
	else
	{
		t2 = TCNT2;
		if (t2>=t1){

		t = ((t2 - t1)*5.3333);
		}
		else if (t1>t2){

			t = (((255-t1)+t2)*5.3333);
		}

		actual_speed= (1000*60)/(60*t);


		flag=0;
	}

}


ISR(TIMER2_OVF_vect)
{

	overflows++;
	if (overflows==74)
	{

		u16AdVal = ADC_u16GetAdValue(ADC_CHANNEL_0);
				desired=(u8)(((u32)u16AdVal * 100)/1023) ;

	prev_error=error;
	error = desired-actual_speed;
	serror+=error;

	action = kp * error + ki * serror + kd * deferror;

	OCR0 = (u8)(action * 255.0)/100.0;

	overflows=0;
	TCNT2 =256-62;

			
}
}

