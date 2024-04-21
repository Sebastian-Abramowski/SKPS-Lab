OBJS1 =cw4a.o 
OBJS2 =cw4b.o

LDFLAGS += -pthread -lrt
LDFLAGS2 = $(LDFLAGS)

all: cw4a cw4b

cw4a: $(OBJS1)
	$(CC) -o cw4a $(OBJS1) $(LDFLAGS)

$(OBJS1) : %.o : %.c *.h
	$(CC) -c $(CFLAGS) $< -o $@

cw4b: $(OBJS2)
	$(CC) -o cw4b $(OBJS2) $(LDFLAGS2)

$(OBJS2) : %.o : %.c *.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f cw4a cw4b *.o

