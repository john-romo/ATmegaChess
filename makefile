DEVICE = atmega328p
PROGRAMMER = USBtiny
f = chess

COMPILE = avr-gcc -Os -mmcu=$(DEVICE)

default: compile flash clean

compile:
	
	$(COMPILE) -c $(f).c -o $(f).o
	$(COMPILE) -o $(f).elf  $(f).o
	avr-objcopy -j .text -j .data -O ihex  $(f).elf  $(f).hex
	avr-size --format=avr --mcu=$(DEVICE) $(f).elf

flash:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -U flash:w:$(f).hex:i 

clean:	
	rm $(f).o $(f).elf $(f).hex


size:
	$(COMPILE) -c chess.c
	$(COMPILE) -c board.c
	$(COMPILE) -c moves.c
#	$(COMPILE) -c tests.c
	$(COMPILE) -o $(f).elf *.o
	avr-objcopy -j .text -j .data -O ihex  $(f).elf  $(f).hex
	avr-size --format=avr --mcu=$(DEVICE) $(f).elf
	rm *.o $(f).elf $(f).hex
	
test:
	gcc -o $(f) *.c 
	./$(f)
	rm $(f)



