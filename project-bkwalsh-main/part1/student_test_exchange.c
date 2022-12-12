/*
 * CS 152, Spring 2022
 * Basic test framework for an simple exchange
 * 
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "order.h"
#include "action_report.h"
#include "exchange.h"
#include "util.h"

/*
char *sample_orders[] =
  { "I,UOCCS,A,S,100,550000,1000",
    "I,UOCCS,A,B,100,550000,3000",
    "I,UOCCS,A,S,100,550000,4000",
    "I,UOCCS,A,B,70,558000,1070",
    "I,UOCCS,A,S,20,540000,2000",
    "I,UOCCS,A,S,10,550000,1010",
    "I,UOCCS,A,B,20,555000,1020",
    "I,UOCCS,A,S,100,570000,2010",
    "I,UOCCS,A,B,100,560000,1040",
    "I,UOCCS,A,B,100,565000,1050",
    "I,UOCCS,A,S,70,550000,1001",
    "I,UOCCS,A,S,200,540000,1101"    
  };
  */

// My sample orders
 char *sample_orders[] =
      {
       "I,UOCCS,A,S,100,550000,1001",
       "I,UOCCS,A,B,100,550000,1002",
       "I,UOCCS,A,B,100,550000,1003",  
       "I,UOCCS,A,S,100,550000,1030",
       "I,UOCCS,A,S,100,550000,1031",       
       "I,UOCCS,A,B,99,550000,1032",
       "I,UOCCS,A,B,100,550000,1033",
       "I,UOCCS,A,S,99,550000,1034",
       "I,UOCCS,A,S,100,550000,1035", // at time 10
       "I,UOCCS,A,B,70,558000,1070",  // at time 20 //
       "I,UOCCS,A,S,20,540000,2000",  // at time 30
       "I,UOCCS,A,S,10,550000,1010",  // at time 40
       "I,UOCCS,A,B,20,555000,1020",  // at time 45
       "I,UOCCS,A,S,100,570000,2010", // at time 50
       "I,UOCCS,A,B,100,560000,1040", // at time 60
       "I,UOCCS,A,B,100,565000,1050", // at time 70
       "I,UOCCS,A,B,100,565000,5000", // at time 69
       "I,UOCCS,A,S,250,562000,7000",  // at time 80  
       "I,UOCCS,A,S,10,550000,14",  // at time 40
       "I,UOCCS,A,B,20,555000,15",  // at time 45
       "I,UOCCS,A,S,100,570000,16", // at time 50
       "I,UOCCS,A,B,100,560000,17", // at time 60
       "I,UOCCS,A,B,100,565000,18", // at time 70
       "I,UOCCS,A,B,100,565000,19", // at time 69
       "I,UOCCS,A,S,250,562000,20"  // at time 80  
       "I,UOCCS,A,S,100,550000,21", // at time 10
       "I,UOCCS,A,B,70,558000,22",  // at time 20
       "I,UOCCS,A,S,20,540000,23",  // at time 30
       "I,UOCCS,A,S,10,550000,24",  // at time 40
       "I,UOCCS,A,B,20,555000,25",  // at time 45
       "I,UOCCS,A,S,100,570000,26", // at time 50
       "I,UOCCS,A,B,100,560000,27", // at time 60
       "I,UOCCS,A,B,100,565000,28", // at time 70
       "I,UOCCS,A,B,100,565000,29", // at time 69
       "I,UOCCS,A,S,250,562000,30"  // at time 80       
  }; 


// Uses the fact that sample-orders is stack-allocated to find its
// size
#define NUM_SAMPLE_ORDERS (sizeof(sample_orders)/sizeof(char *))  


/* 
 * run through a series of orders.
 *
 * orders_to_use: an array of integer indices for sample orders
 * num_orders: the number of orders to test
 * times_for_orders: the times to use for the orders
 */

void run_test(char **orders, int orders_to_use[], int num_orders, int times_for_orders[]) {
    assert(num_orders >= 0);

    exchange_t *exchange = mk_exchange("UOCCS");
    assert(exchange != NULL);

    for (int i = 0; i < num_orders; i++) {
        // make sure the index is sensible
        assert(0 <= i && i < NUM_SAMPLE_ORDERS);

        // Grab the order string from orders
        char *o_str = orders[orders_to_use[i]];

        // process the order
        printf("processing order...%s\n", o_str);
        action_report_t *ar = process_order(exchange, o_str, times_for_orders[i]);

        // print the action report so we can see what happened.
        printf("After action report...\n");
        print_action_report(ar);
        printf("\n");
	// free up the action repor
        free_action_report(ar);
    
        // print the state of the exchange after the order
        // has been processed
        printf("Exchange after processing the order...\n");
        print_exchange(exchange);
    }

    // free up the exchange
    free_exchange(exchange);
}
  

/* do_none: Test creation and free for an empty exchange */
void do_none() {
    run_test(NULL, NULL, 0, NULL);
}

void do_one() {
    // run a couple orders through the exchange
    // Try different combinations orders here w/ 1, 2, 3, 4 orders.
    // Try different combinations of times
    int sample0[] = {0};
    int times0[] = {10};

    run_test(sample_orders, sample0, 1, times0);
}  

void do_sells() {
    // run a couple orders through the exchange
    // Try different combinations orders here w/ 1, 2, 3, 4 orders.
    // Try different combinations of times
    int sample0[] = {0,2,5,6};
    int times0[] = {10,20,30,40};

    run_test(sample_orders, sample0, 4, times0);
}    

void do_buys() {
    // run a couple orders through the exchange
    // Try different combinations orders here w/ 1, 2, 3, 4 orders.
    // Try different combinations of times
    int sample0[] = {1,3,6};
    int times0[] = {10,20,30};

    run_test(sample_orders, sample0, 3, times0);
}  



void do_a_few() {
    // run a couple orders through the exchange
    // Try different combinations orders here w/ 1, 2, 3, 4 orders.
    // Try different combinations of times
    int sample0[] = {0, 1};
    int times0[] = {10, 20};

    run_test(sample_orders, sample0, 2, times0);
}    


/* do_all: run all the sample orders in the order listed */
void do_all() {
    int sample1[NUM_SAMPLE_ORDERS];
    int times1[NUM_SAMPLE_ORDERS];

    for (int i=0; i < NUM_SAMPLE_ORDERS; i++) {
        sample1[i] = i;
        times1[i] = 10*i;
    }

    run_test(sample_orders, sample1, NUM_SAMPLE_ORDERS, times1);
}


void do_handout() {
    char *handout_orders[] =
      {"I,UOCCS,A,S,100,550000,1000", // at time 10
       "I,UOCCS,A,B,70,558000,1070",  // at time 20 //change 100 back to 70
       "I,UOCCS,A,S,20,540000,2000",  // at time 30
       "I,UOCCS,A,S,10,550000,1010",  // at time 40
       "I,UOCCS,A,B,20,555000,1020",  // at time 45
       "I,UOCCS,A,S,100,570000,2010", // at time 50
       "I,UOCCS,A,B,100,560000,1040", // at time 60
       "I,UOCCS,A,B,100,565000,1050", // at time 70
       "I,UOCCS,A,B,100,565000,5000", // at time 69
       "I,UOCCS,A,S,250,562000,7000"  // at time 80  
      };

    int times[] = {10, 20, 30, 40, 45, 50, 60, 70, 69, 80};
    // hack...
    int num_orders = sizeof(times)/sizeof(int);
    int orders_to_use[num_orders];

    for (int i=0; i < num_orders; i++) {
      orders_to_use[i] = i;
    }

    run_test(handout_orders, orders_to_use, num_orders, times);
}



int main() {
    // uncomment to check exchange constructor and free before trying
    // any orders.
    // do_none();
  //  do_buys();
   //do_a_few();
   //  do_one();
  // do_sells();
    // uncomment to process all the samples order
     do_all();
  //  do_handout();
}



