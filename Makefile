CFLAGS = -std=gnu11 -Wall -Wextra
LDFLAGS = -Wl,--as-needed
LDLIBS = -lm -lSDL2
OBJECTS = main.o vec.o sdl.o
BINARY = raygun

INSTALL = install -m755

PREFIX = /usr/local

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += -Og -g
else
	CFLAGS += -flto -O2 -fopenmp
	LDFLAGS += -flto -O2
endif

all: $(BINARY)

raygun: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

main.o: vec.h sdl.h
vec.o: vec.h
sdl.o: sdl.h

install: $(BINARY)
	$(INSTALL) $^ $(DESTDIR)$(PREFIX)/bin

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/$(BINARY)

tags:
	ctags -R .

TAGS:
	etags *.c *.h

clean:
	$(RM) $(OBJECTS) $(BINARY) tags TAGS

.PHONY: all install uninstall clean
