BINFILES := cliente servidor 

CC := gcc 

SRCFILES := cliente.c servidor.c 
OBJFILES := $(SRCFILES:%.c=%.o)

CFLAGS := -Wall -Wextra -Werror


all: $(BINFILES)

cliente: cliente.o
	$(CC) cliente.o -o $@

servidor: servidor.o
	$(CC) servidor.o -o $@

libclaves.so: 
	

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean: 
	rm -f $(OBJFILES)

fclean: clean
	rm -f $(BINFILES)

re: fclean all

.PHONY: all clean fclean re