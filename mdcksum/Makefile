CFLAGS=-Wall -Wextra -Wpedantic
BIN:=mdcksum
OBJ:=main.o

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

clean:
	rm -f $(BIN) $(OBJ)

.PHONY: $(BIN) clean
