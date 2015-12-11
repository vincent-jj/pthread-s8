CC = gcc
#ORD_FIFO (default), ORD_FILO and ORD_FIBONACCI are recognised
CFLAGS = -g -Wall -Wextra -Isrc/
ORD_OPT=-DORD_FIFO
LPTH_FL = -lpthread
PTHD_FLAG = -DUSE_PTHREAD
EXEC = 		./build/test1 			\
		./build/test2 			\
		./build/test11 			\
		./build/test12 			\
		./build/test21 			\
		./build/test22 			\
		./build/test23 			\
		./build/test31 			\
		./build/test32 			\
		./build/test33 			\
		./build/test51			\
		./build/test61
EXEC_PTH =	./build/pthtest1 			\
		./build/pthtest2 			\
		./build/pthtest11 			\
		./build/pthtest12 			\
		./build/pthtest21 			\
		./build/pthtest22 			\
		./build/pthtest23 			\
		./build/pthtest31 			\
		./build/pthtest32 			\
		./build/pthtest51			\
		./build/pthtest61
SRC= 	src/thread.c \
	src/list.c
OBJ= $(SRC:.c=.o) 
SRC_MAIN=   	./tests/01-main.c		\
		./tests/02-switch.c		\
		./tests/11-join.c		\
		./tests/12-join-main.c		\
		./tests/21-create-many.c	\
		./tests/22-create-many-recursive.c \
		./tests/23-create-many-once.c	\
		./tests/31-switch-many.c	\
		./tests/32-switch-many-join.c	\
		./tests/33-list.c		\
		./tests/51-fibonacci.c		\
		./tests/61-mutex.c

OBJ_MAIN=$(SRC_MAIN:.c=.o)

.PHONY: all

all: $(EXEC) $(EXEC_PTH)

build/test1:  ./tests/01-main.c $(OBJ) 
	$(CC) $(CFLAGS) $< $(OBJ) -o $@  

build/test2:  ./tests/02-switch.c $(OBJ)
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ 

build/test11:  ./tests/11-join.c $(OBJ)
	$(CC)  $(CFLAGS) $< $(OBJ) -o $@ 

build/test12:  ./tests/12-join-main.c $(OBJ)
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ 

build/test21:  ./tests/21-create-many.c $(OBJ)
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ 

build/test22: ./tests/22-create-many-recursive.c $(OBJ)
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ 

build/test23:  ./tests/23-create-many-once.c $(OBJ)
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ 

build/test31:  ./tests/31-switch-many.c $(OBJ)
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ 

build/test32:  ./tests/32-switch-many-join.c $(OBJ)
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ 

build/test33:  ./tests/33-list.c $(OBJ)
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ 

build/test51:  ./tests/51-fibonacci.c
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ 

build/test61:  ./tests/61-mutex.c $(OBJ)
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ 

## ** Partie du Makefile à améliorer ou à modifier. Permet de compiler les main avec la lib pthread ** ##

build/pthtest1:  ./tests/01-main.c 
	$(CC) $(PTHD_FLAG) $(CFLAGS) $< -o $@ $(LPTH_FL) 

build/pthtest2:  ./tests/02-switch.c 
	$(CC) $(PTHD_FLAG) $(CFLAGS) $< -o $@ $(LPTH_FL)

build/pthtest11:  ./tests/11-join.c 
	$(CC) $(PTHD_FLAG) $(CFLAGS) $< -o $@ $(LPTH_FL)

build/pthtest12:  ./tests/12-join-main.c 
	$(CC) $(PTHD_FLAG) $(CFLAGS) $< -o $@ $(LPTH_FL)

build/pthtest21:  ./tests/21-create-many.c 
	$(CC) $(PTHD_FLAG) $(CFLAGS) $< -o $@ $(LPTH_FL)

build/pthtest22: ./tests/22-create-many-recursive.c 
	$(CC) $(PTHD_FLAG) $(CFLAGS) $< -o $@ $(LPTH_FL)

build/pthtest23:  ./tests/23-create-many-once.c 
	$(CC) $(PTHD_FLAG) $(CFLAGS) $< -o $@ $(LPTH_FL)

build/pthtest31:  ./tests/31-switch-many.c 
	$(CC) $(PTHD_FLAG) $(CFLAGS) $< -o $@ $(LPTH_FL)

build/pthtest32:  ./tests/32-switch-many-join.c 
	$(CC) $(PTHD_FLAG) $(CFLAGS) $< -o $@ $(LPTH_FL)

build/pthtest51:  ./tests/51-fibonacci.c 
	$(CC) $(PTHD_FLAG) $(CFLAGS) $< -o $@ $(LPTH_FL)

build/pthtest61:  ./tests/61-mutex.c 
	$(CC) $(PTHD_FLAG) $(CFLAGS) $< -o $@ $(LPTH_FL)

%.o: %.c	
	$(CC) $(CFLAGS) $(ORD_OPT) -o $@ -c $<

clean: 
	rm -f $(EXE) $(OBJ) $(OBJ_MAIN) $(EXEC_PTH) $(EXEC)
