SRC := src
TIME := mytimeout
OBJ := obj
OBJ2 := obj2
BIN := bin
EXECUTABLE := shell
EXECUTABLE2 := mytimeout

SRCS := $(wildcard $(SRC)/*.c)
TIMES := $(wildcard $(SRC)/$(TIME)/*.c)
@echo "TIMES: $(TIMES)"
OBJS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
OBJS2 := $(patsubst $(SRC)/$(TIME)/%.c,$(OBJ2)/%.o,$(TIMES))
INCS := -Iinclude/
DIRS := $(OBJ)/ $(BIN)/
DIRS2 := $(OBJ2)/ $(BIN)/
EXEC := $(BIN)/$(EXECUTABLE)
EXEC2 := $(BIN)/$(EXECUTABLE2)

CC := gcc
CFLAGS := -g -Wall -std=c99 $(INCS)
LDFLAGS :=

all: $(DIRS) $(DIRS2) $(EXEC) $(EXEC2)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(EXEC)

$(EXEC2): $(OBJS2)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS2) -o $(EXEC2)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ2)/%.o: $(SRC)/$(TIME)/%.c
	@echo "Compiling $< to $@"
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXEC)
	$(EXEC)

clean:
	rm -f $(OBJS) $(OBJS2) $(EXEC) $(EXEC2)

$(DIRS):
	mkdir -p $@

$(DIRS2):
	mkdir -p $@

.PHONY: run clean all

