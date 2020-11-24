CC = gcc
CFLAGS = -Wall -Wextra -Wpointer-arith -Wstrict-prototypes -std=gnu89 -g -fPIC -L.
LDFLAGS = -shared

all: libfastlog mytests

mytests: mytests.o fastlog.o
	$(CC) $(CFLAGS) $^ -o $@ 

libfastlog: fastlog.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@.so

# Note: For this test to run, the current directory
#   must be added to the LD_LIBRARY_PATH
#
#   export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:.
#
mylibtests: mytests.o
	$(CC) $(CFLAGS) $^ -o $@ -lfastlog

clean:
	rm -f *.o *.d a.out mytests mylibtests libfastlog.so
