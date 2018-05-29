/*
 * attniy13_ADC1_0C0A.c
 *
 * Created: 3/14/2017 6:07:02 PM
 * Author : ash
 */ 

// 9.6 MHz, built in resonator
//#define F_CPU 9600000

#include <avr/io.h>

void adc_setup (void)
{
	// Set the ADC input to PB2/ADC1
	ADMUX |= (1 << MUX0);
	//set presentation alignment left to right
	ADMUX |= (1 << ADLAR);	
	// Set the prescaler to clock/16 & enable ADC
	// At 1.2 MHz this is 75 kHz.
	ADCSRA |= (1 << ADPS2) | (1 << ADEN);
}

int adc_read (void)
{
	// Start the conversion
	ADCSRA |= (1 << ADSC);
	// Wait for it to finish
	while (ADCSRA & (1 << ADSC));
	//read the result from ADCH register
	return ADCH;
}

void pwm_setup (void)
{
	// Set Timer 0 prescaler to clock/8.
	// At 9.6 MHz this is 1.2 MHz.
	// dont enable if div by 8 fuse bit is enabled
	TCCR0B |= (1 << CS00) | (1 << CS01);//we as already have fuse div/8 setand now we set div/64 which becomes = div/72
	// Set to 'Fast PWM' mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	// Set OC0A output on compare match
	TCCR0A |= (1 << COM0A0) | (1 << COM0A1);
}

void pwm_write (int val)
{
	OCR0A = val;
}

int main (void)
{
	int adc_in;
	//ADC1 PB2 as input
	DDRB |= (1 << PB2);
	// OC0A PB0 as an output.
	DDRB |= (1 << PB0);
	
	adc_setup();
	pwm_setup();
	
	while (1) {
		// Get the ADC value
		adc_in = adc_read();
		// Now write it to the PWM counter
		pwm_write(adc_in);
	}
}