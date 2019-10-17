CC	    := gcc
C_FLAGS	    := -Wall -Wextra #-fPIC 

BIN	    := bin
SRC	    := $(wildcard lib/*.c)
INCLUDE	    := include
LIB	    := lib

LIBRARIES   := -lncurses

APP_NAME:= server

EXECUTABLE  := $(APP_NAME)

CFILES := src/main.c $(SRC)

all: $(CFILES)
	$(CC) $(C_FLAGS) -I$(INCLUDE) $(CFILES) $(LIBRARIES) -o $(BIN)/$(APP_NAME)

clean:
	-$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)
