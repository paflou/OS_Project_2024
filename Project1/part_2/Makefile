CC=gcc
CFLAGS=-Wall -O3
LDFLAGS=-lm

all: integral_mc_seq integral_mc_shm integral_mc_shm_sem

integral_mc_seq: integral_mc_seq.c
	$(CC) $(CFLAGS) -o bin/integral_mc_seq integral_mc_seq.c $(LDFLAGS)

integral_mc_shm: integral_mc_shm.c
	$(CC) $(CFLAGS) -o bin/integral_mc_shm integral_mc_shm.c $(LDFLAGS)

integral_mc_shm_sem: integral_mc_shm_sem.c
	$(CC) $(CFLAGS) -o bin/integral_mc_shm_sem integral_mc_shm_sem.c $(LDFLAGS)

clean:
	rm -f bin/*

.PHONY: all clean

# Create the bin directory if it doesn't exist
$(shell mkdir -p bin)
