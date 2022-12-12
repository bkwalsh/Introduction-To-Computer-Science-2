/*
 * CS 152, Spring 2022
 * Simulation -- main file
 * 
 * You will modify this file
 *
 * Name: Bayard Walsh
 *
 * Sources used: None
 *
 * People consulted: None
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "action_report.h"
#include "exchange.h"

#define MAX_ORDER_LEN 1000
#define MAX_TEST_FILENAME 30

// Your helper functions go here.

void query(FILE *test, exchange_t *exchange, char *ticker, int order_num) {
	FILE *out;
	int c;
	int clock=0;
	bool flag=false;
	int i=0;
	char buffer[25];
	char out_string[25];
	sprintf(out_string, "tests/test%d_actions.csv", order_num);
	out = fopen(out_string,"w"); 
	action_report_t *ar=mk_action_report(ticker);
	while(flag!=true){
		c = fgetc(test);
		buffer[i] = c;	
		++i;
		if (c == '\n' || (c == EOF && c != '\n' &&  i != 1)) {
			buffer[i]='\0';
			ar = process_order(exchange,buffer,clock);
			write_action_report_to_file(ar, out, clock);
			clock++;
			i = 0;
		}
		if (c == EOF){
			flag = true;
		} 
	}
	fclose(out);
}

int main(int argc, char **argv) {
	if (argc!=3){
		fprintf(stderr,"usage: simulate <ticker symbol> <test number> \n");
		exit(1);
	}
	int order_num = atoi(argv[2]);
	FILE *order_stack;
	char buffer[25];
	sprintf(buffer, "tests/test%d_orders.csv", order_num);
	order_stack = fopen(buffer,"r");
	if(order_stack == NULL){
		fprintf(stderr,"usage: invalid file number \n");
		exit(1);
	}
	exchange_t *exchange = mk_exchange(argv[1]);
	query(order_stack, exchange, argv[1],order_num);
	fclose(order_stack);
}

