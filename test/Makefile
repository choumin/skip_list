CC=gcc
ROOT_DIR=$(shell pwd)
CFLAG=-I$(ROOT_DIR)
MAIN_BIN=main

CUR_SOURCE=${wildcard *.c}
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

all:$(CUR_OBJS) $(MAIN_BIN)

$(CUR_OBJS):%.o:%.c
	$(CC) -c $^ $(CFLAG)

$(MAIN_BIN):$(CUR_OBJS)
	$(CC) $^ -o $@


clean:
	@rm	$(ROOT_DIR)/*.o
