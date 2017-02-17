

main.out: main.c
	avr-gcc -mmcu=atmega162 main.c -o main.out


main.hex: main.out
	avr-objcopy -j .text -j .data -O ihex main.out main.hex
	


upload: main.hex 
	avrdude -c usbasp -p m162 -U flash:w:main.hex 

