OBJS1 =demo1.o 
LDFLAGS += -pthread

all: demo1

cw1m: $(OBJS1)
	$(CC) -o demo1 $(CFLAGS) $(LDFLAGS) $(OBJS1) 

$(OBJS1) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f demo1 *.o

