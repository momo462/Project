BIN=httpd
SRC=httpd.c
OBJ=$(SRC:.c=.o)
CC=gcc
LDFLAGS=-pthread
#FLAGS=-D_DEBUG_

$(BIN):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
%.o:%.c
	$(CC) -c $< $(FLAGS)

.PHONY:clean
clean:
	rm -f $(BIN) *.o

.PHONY:debug
debug:
	@echo $(SRC)
	@echo $(OBJ)
