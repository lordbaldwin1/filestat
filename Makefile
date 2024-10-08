CC = gcc
DEFINES =
DEBUG  = -g
CFLAGS = $(DEBUG) -Wall

PROG1 = mystat
PROG2 = mywc
PROGS = $(PROG1) $(PROG2) $(PROG3)

all: $(PROGS)

$(PROG1): $(PROG1).o
	$(CC) $(CFLAGS) -o $(PROG1) $(PROG1).o
	chmod og-rx $(PROG1)

$(PROG2): $(PROG2).o
	$(CC) $(CFLAGS) -o $(PROG2) $(PROG2).o
	chmod og-rx $(PROG2)

cls clean:
	rm -f $(PROGS) *.o *~ \#* *.tar.gz

TAR_FILE = ${LOGNAME}_$(PROG).tar.gz
tar:
	rm -f $(TAR_FILE)
	tar cvzf $(TAR_FILE) *.[ch] [Mm]akefile

opt:
	make clean
	make CFLAGS=-O
