CC = gcc
C++ = g++
LINK = g++
ROOT_DIR = $(shell cd .. && pwd)
CFLAG = -I$(ROOT_DIR)
MAIN_BIN = main

#$(warning $(ROOT_DIR))

CFILES=${wildcard $(ROOT_DIR)/*.c}
CPPFILES=${wildcard *.cpp}

OBJFILES = $(CFILES:.c=.o) $(CPPFILES:.cpp=.o)

#$(warning $(OBJFILES))
#$(warning $(C++))

all:$(OBJFILES) $(MAIN_BIN)

%.o:%.c
	$(CC) -c $^ $(CFLAG) -o $@

%.o:%.cpp
	$(C++) -c $^ $(CFLAG) -o $@

$(MAIN_BIN):$(OBJFILES)
	$(LINK) $^ -o $@


clean:
	@rm $(OBJFILES)
