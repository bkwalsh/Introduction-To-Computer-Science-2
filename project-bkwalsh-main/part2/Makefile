OBJECTS = 
CFLAGS = -g -Wall -O0 --std=c11
LDLIBS= -l criterion -lm
CC=clang
FILES= order.c util.c book.c action_report.c exchange.c


all: test_exchange student_test_exchange simulate

student_test_exchange: ${FILES} student_test_exchange.c

test_exchange:  ${FILES} test_exchange.c

simulate:  ${FILES} simulate.c

vg: student_test_exchange
	valgrind --leak-check=full ./student_test_exchange

clean:
	rm -f *.o student_test_exchange test_exchange simulate
	rm -rf *.dSYM *~ \#*


