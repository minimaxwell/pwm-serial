/*
 * Documentation : 
 *
 * Prog AVR ATMega328P
 * http://www.atmel.com/webdoc/avrassembler/avrassembler.wb_IN.html
 * http://hackaday.com/2010/11/05/avr-programming-03-reading-and-compiling-code/
 * http://www.nongnu.org/avr-libc/user-manual/modules.html
 * http://www.atmel.com/images/Atmel-8271-8-bit-AVR-Microcontroller-ATmega48A-48PA-88A-88PA-168A-168PA-328-328P_datasheet_Complete.pdf
 *
 * Utilisation liaison serie sur ATMega328P
 * http://www.appelsiini.net/2011/simple-usart-with-avr-libc
 */

#define F_CPU 8000000UL  // 8MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

// pour liaison serie
#define BAUD 57600       // baudrate

#include <util/setbaud.h>

uint8_t serial_getchar(void) {
	loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
	return UDR0;
}

void pins_init(void){
	/* Setup PortD 3,5,6 and PortB 1,2,3 as out */
    DDRD |= _BV(PD3) | _BV(PD5) | _BV(PD6);
    DDRB |= _BV(PB1) | _BV(PB2) | _BV(PB3);
}


// OCR0A : Bleu 1
// OCR0B : Vert 1
// OCR1A : Bleu 2 ( 16 bits )
// OCR1B : Vert 2 ( 16 bits )
// OCR2A : Rouge 2
// OCR2B : Rouge 1

void pwm_init(void){
	TCCR0A = 0xA3;
	TCCR0B = 0x01;
	OCR0A = 0xFF;
	OCR0B = 0xFF;

	TCCR1A = 0xA1;
	TCCR1B = 0x09;
	OCR1A = 0xFFFF;
	OCR1B = 0xFFFF;

	TCCR2A = 0xA3;
	TCCR2B = 0x01;
	OCR2A = 0xFF;
	OCR2B = 0xFF;
}


void led_set(uint8_t led, uint8_t r, uint8_t g, uint8_t b) {
	if (!led) {
		OCR2B = r;
		OCR0B = g;
		OCR0A = b;
	} else {
		OCR2A = r;
		OCR1BL = g;
		OCR1AL = b;
	}
}

void serial_init(void){
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    UCSR0B = _BV(RXEN0);   // enable Rx, pas besoin de Tx

}


int main(void){

    pins_init();

    serial_init();

	pwm_init();

	led_set(0, 10,10,10);
	led_set(1, 10,10,10);
	uint8_t r,g,b,l;
    while( 1 ){

		r = serial_getchar();
		g = serial_getchar();
		b = serial_getchar();
		l = serial_getchar();
		led_set(l, r, g, b);

    }

}

