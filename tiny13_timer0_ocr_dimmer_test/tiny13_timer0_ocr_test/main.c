/*
 * tiny13_timer0_ocr_test.c
 *
 * Created: 3/6/2017 2:48:44 PM
 * Author : ash
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void hw_init()
{	//################### INPUT OUTPUT SETUP ###########################################
	DDRB &= ~(1<<PB1);//zcd pin connected to INT0 is set as input
	PORTB|= (1<<PB1);//internal pull up for INT0 
	DDRB |= (1<<PB0);//triac pin connected to OC0A is set as output
	DDRB &= ~(1<<PB2);//control input pin connected to ADC1 and PCINT2 is set as input
	//################### ZERO CROSSING INTERRUPT ######################################
	MCUCR |= (1<<ISC00)|(1<<ISC01);//ATTACHING INT0 INTERRUPT ON RISING EDGE
	GIMSK |= (1<<INT0);//ENABLE 
	//################### TIMER CONFIGURATION SETUP ####################################
	TCCR0A = 0x00;//
	TCCR0B = 0x00;//
	TIMSK0 = 0X00;//CLEAR ALL PREVIOUS CONFIGRATION FROM TIMER CONTROL AND TIMER MASK REGISTERS
	TIMSK0 |= (1<<OCIE0A)|(1<<TOIE0);//ENABLE OC0A COMPARE INTERRUPT AND TIMER OVERFLOW INTERRUPT
	sei();//SET GLOBAL INTERRUP ENABLE FOR USING INTERRUPTS
}

int speed_table[23]={20,40,60,70,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,161,162};

int main(void)
{
	hw_init();
	
	while (1)
	{
		OCR0A = speed_table[10];//till 15 to 255 //till 20 to 162
		//calibration from high to low respectively int speed_table[23]={20,40,60,70,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,161,162};
	}
}


//############################### INTERRUPT SERVICE ROUTINE ##############################

ISR(INT0_vect)
{
	TCNT0 = 0x00;//clear timer counter to 0
	TCCR0B |= (1<<CS00)|(1<<CS01);//F_CPU div 64 
}

ISR(TIM0_COMPA_vect)
{
	PORTB |= (1<<PB0);//turn on triac
	TCNT0 = 255-4;//fill the timer leaving only propagation delay time for triac on
}

ISR(TIM0_OVF_vect)
{
	PORTB &= ~(1<<PB0);//turn off triac
	TCCR0B &=~((1<<CS00)|(1<<CS01)|(1<<CS02));//remove clock source for stopping timer
}

/*

void start_timer()
{
	//TCCR0B |= (1<<CS00);			//F_CPU div 1 no clock source
	//TCCR0B |= (1<<CS01);			//F_CPU div 8
	TCCR0B |= (1<<CS00)|(1<<CS01);//F_CPU div 64
	//TCCR0B |= (1<<CS02);			//F_CPU div 256
	//TCCR0B |= (1<<CS00)|(1<<CS02);//F_CPU div 1024
	
}
void stop_timer()
{
	TCCR0B &=~((1<<CS00)|(1<<CS01)|(1<<CS02));//remove clock source for stopping timer
}

*/