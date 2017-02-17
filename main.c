#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>


#define F_OSC	12000000UL
#define F_BAUD	9600
#define BAUD_VALUE ((F_OSC)/(F_BAUD*16UL)-1)

#define low(x) ((x)&0xff)
#define high(x) (((x)>>8)&0xff)


void usart_send(unsigned char c) {
	while (!(UCSR0A&(1<<UDRE0)));
	UDR0 = c;
}


void usart_send_string(unsigned char* string) {
	unsigned char i = 0;

	for (;string[i] != '\0'; i++) {
		usart_send(string[i]);
	}
}



unsigned char mask = 0;


ISR(USART0_RXC_vect) {
	unsigned char data = UDR0;

	if (data >= 'a' && data <= 'f') {
		mask &= ~(1 << (data-'a'));
	} else if (data >= 'A' && data <= 'F') {
		mask |= (1 << (data-'A'));
	}
}


int main() {

	UBRR0H = high(BAUD_VALUE);
	UBRR0L = low(BAUD_VALUE);

	UCSR0B = (1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0);
	UCSR0C = (1<<URSEL0)|(1<<UCSZ01)|(1<<UCSZ00);
	
	sei();
	
	DDRA = 0xff;
	DDRB = 0x00;

	
	usart_send_string("# dsm-emukey v1.0\r\n");


	while(1) {
		unsigned char input = PINB;
		input &= mask;
		PORTA = input;
	}	

	return 0;
}
