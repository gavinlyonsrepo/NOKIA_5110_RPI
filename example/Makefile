# Makefile to build an example file to test file using library NOKIA_5110_RPI
# ProjectURL : https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
# Options :
# 1. make 
# 2. make clean
# 3. make run

CC=g++
LDFLAGS= -lbcm2835 -lNOKIA_5110_RPI
CFLAGS = -Iinclude/  -c -Wall 
MD=mkdir

SRC=src
OBJ=obj
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst $(SRC)/%.cpp,  $(OBJ)/%.o, $(SRCS))
BIN=./bin
TARGET=$(BIN)/test

all: pre-build $(TARGET) 

run:
	sudo $(TARGET) 


pre-build:
	@echo '***************'
	@echo 'START!'
	$(MD) -vp $(BIN)
	$(MD) -vp $(OBJ)


$(TARGET): $(OBJS) $(OBJ)
	@echo 'MAKE EXE FILE'
	$(CC)  $(OBJS) -o $@ $(LDFLAGS)
	@echo 'DONE!'
	@echo '***************'

$(OBJ)/%.o :  $(SRC)/%.cpp   $(OBJ) 
	@echo 'MAKE OBJECT FILE'
	$(CC) $(CFLAGS) $< -o  $@ 

clean:
	@echo 'CLEANUP!'
	rm -vf  $(OBJS) $(TARGET)
	@echo 'DONE!'
	@echo '***************'


