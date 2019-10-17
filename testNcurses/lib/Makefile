CC	:= gcc
C_FLAGS := -Wall -Wextra -fPIC 

BIN	:= ../bin
SRC	:= src
INCLUDE	:= ../include
LIB	:= lib

CFILEUI := user_interface.c
CFILECMD := user_commands.c
HFILES := $(INCLUDE)/user_interface.h $(INCLUDE)/user_commands.h


all: 
	make user_interface user_commands

user_interface: $(CFILEUI) $(HFILES)
	$(CC) $(C_FLAGS) -I$(INCLUDE) $(CFILEUI) -shared -o libuser_interface.so
	
user_commands: $(CFILECMD) $(HFILES)
	$(CC) $(C_FLAGS) -I$(INCLUDE) $(CFILECMD) -shared -o libuser_commands.so

install: 
	mv *.so $(BIN)/$(LIB)

clean:
	-$(RM) $(BIN)/$(LIB)/*.so