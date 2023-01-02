# Makefile to build an example file to test file using library NOKIA_5110_RPI
# ProjectURL : https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
# Options :
# 1. make 
# 2. make clean
# 3. make run

# ************************************************
# *** User SRC directory Option Section **********
# Pick one example "SRC" dir path and ONE ONLY.
# Comment out the other's . 

SRC=src/HelloWorld
#SRC=src/HelloWorld_SWSPI
#SRC=src/TestRun
#SRC=src/SpeedTest
# ************************************************


CC=g++
LDFLAGS= -lbcm2835 -lNOKIA_5110_RPI
CFLAGS = -Iinclude/  -c -Wall 
MD=mkdir
OBJ=obj
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst $(SRC)/%.cpp,  $(OBJ)/%.o, $(SRCS))
BIN=./bin
TARGET=$(BIN)/test

all: clean pre-build $(TARGET) 

run:
	sudo $(TARGET) 


pre-build:
	@echo '***************'
	@echo 'START!'
	@echo 'Making : ' $(SRC)
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


