CC = gcc
CFLAGS = -Wall -Wextra -g

.PHONY: build clean run

build: controller bulb

controller: src/controller.c include/errors.h
	$(CC) $(CFLAGS) -o controller src/controller.c

bulb: src/bulb.c include/errors.h
	$(CC) $(CFLAGS) -o bulb src/bulb.c

window: src/window.c include/errors.h
	$(CC) $(CFLAGS) -o window src/window.c

fridge: src/fridge.c include/errors.h
	$(CC) $(CFLAGS) -o fridge src/fridge.c

hub: src/hub.c include/errors.h
	$(CC) $(CFLAGS) -o hub src/hub.c

timer: src/timer.c include/errors.h
	$(CC) $(CFLAGS) -o timer src/timer.c

run: build
	./controller

clean:
	rm -f controller bulb windows fridge hub timer 
	rm -f /tmp/domotics_*