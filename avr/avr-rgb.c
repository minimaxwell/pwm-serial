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

#define RISING ( _BV(ISC01) | _BV(ISC00) )
#define FALLING ( _BV(ISC01) )
#define CHANGING ( _BV(ISC00) )

/* ----------------------------------------------------------------- */
/* Variables globales */

volatile uint8_t buff_index;
volatile uint8_t buffer[255];
volatile uint8_t done_reading;

/* ----------------------------------------------------------------- */
void serie_putchar(char c){
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

void pins_init(void){
    DDRD |= _BV(PD5);
    DDRD |= _BV(PD3);
    DDRD |= _BV(PD6);
    DDRB |= _BV(PB1);
    DDRB |= _BV(PB2);
    DDRB |= _BV(PB3);
}


// OCR0A : Bleu 1
// OCR0B : Vert 1
// OCR1A : Bleu 2 ( 16 bits )
// OCR1B : Vert 2 ( 16 bits )
// OCR2A : Rouge 1
// OCR2B : Rouge 2

void pwm_init(void){
	TCCR0A = 0xF3;
	TCCR0B = 0x01;
	OCR0A = 0xFF;
	OCR0B = 0xFF;

	TCCR1A = 0xF1;
	TCCR1B = 0x09;
	OCR1A = 0xFFFF;
	OCR1B = 0xFFFF;

	TCCR2A = 0xF3;
	TCCR2B = 0x01;
	OCR2A = 0xFF;
	OCR2B = 0xFF;

}


void led_set(uint8_t led, uint8_t r, uint8_t g, uint8_t b) {
	if (!led) {
		OCR2A = (255-r);
		OCR0B = (255-g);
		OCR0A = (255-b);
	} else {
		OCR2B = (255-r);
		OCR1BL = (255-g);
		OCR1AL = (255-b);
	}
}

void serial_init(void){
    //
    // definition de la vitesse
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8N1
    UCSR0B = _BV(TXEN0);   // enable Tx, pas besoin de Rx

}


int main(void){

    // parametrage des pins en input
    pins_init();

    // parametrage de la liaison serie
    serial_init();

	pwm_init();

	led_set(0, 255,50,100);
	led_set(1, 255,50,100);
    // listening mode
    while( 1 ){

		led_set(0, 255,50,100);
		led_set(1, 255,50,100);

		_delay_ms(1000.0);

		led_set(0, 0,50,100);
		led_set(1, 0,100,10);

		_delay_ms(1000.0);

    }

    // analyse de la commande

    // envoi sur le bus/liaison serie
    
}

