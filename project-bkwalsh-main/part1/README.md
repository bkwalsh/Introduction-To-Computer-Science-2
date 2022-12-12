CS 152, Spring 2022
Stock Exchange Project - Part 1

This directory contains the files for the project

Makefile: file to be used with make.  Run:
  make - to compile your code and generate an executable
    for the basic testing framework and for the single automated test

  make student_test_exchange - to compile your code and generate an
    executable for the basic testing framework and for the single
    automated test

  make vg - to compile your code with student_test_exchange then, if
    successful run valgrind

  make test_exchange - to compile your code and generate an
    executable for the single automated test

  make clean - remove generated files (a good thing to do before you
    add/commit/push your code).

########## You must modify these files ########## 

book.c: C file for the book data structure implementation
book.h: Header file for the book data structure interface

exchange.c: C file for the exchange data structure implementation
exchange.h: Header file for the exchange data structure interface

##########  You can and should modify this file ########## 

student_test_exchange.c: basic testing framework

##########  You can modify these files, if you like ########## 

order.c: C file for the order data structure implementation
order.h: Header file for the order data structure interface


##########  Do NOT modify these files ########## 

test_exchange.c: sample automated test (the hidden autograder will
  have more tests)

action_report.c: C file for the action report data structure implementation
action_report.h: Header file for the action report data structure interface

util.c: C file with some useful utility functions
util.h: C header file for the useful utility functions

README.md: this file
