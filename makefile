BINFILES := cliente servidor 

CC := gcc 

SRCFILES := cliente.c servidor.c  
OBJFILES := $(SRCFILES:%.c=%.o)



LDFLAGS= -L.
CFLAGS := -Wall -I. 
LDLIBS := -lpthread -lrt

all: $(BINFILES)

servidor: servidor.o
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

cliente: cliente.o libclaves.so
	$(CC) -o $@ cliente.c $(LDFLAGS) $(CFLAGS) -Bdynamic -lclaves $(LDLIBS)

libclaves.so: claves.c
	$(CC) -fPIC -c -o claves.o claves.c
	$(CC) -shared claves.o -o libclaves.so 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean: 
	rm -f $(OBJFILES)

fclean: clean
	rm -f $(BINFILES) libclaves.so

re: fclean all

.PHONY: all clean fclean re