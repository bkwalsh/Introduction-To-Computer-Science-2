/*
 * CS 152, Spring 2022
 * Exchange
 * 
 * You will modify this file
 *
 * Name: Bayard Walsh
 *
 * Sources used: None
 *
 * People consulted: None
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "order.h"
#include "book.h"
#include "action_report.h"
#include "util.h"
#include "exchange.h"

struct exchange {
  char *ticker;
  book_t *buy;
  book_t *sell;  
};

/* 
 * mk_exchange: make an exchange for the specified ticker symbol
 *
 * ticker: the ticker symbol for the stock
 *
 * Returns: an exchange
 */
exchange_t *mk_exchange(char *ticker) {
    exchange_t *out = (exchange_t*)malloc(sizeof(exchange_t));
    if (out == NULL) {
        fprintf(stderr, "exchange_t: Unable to allocate\n");
        exit(1);
    }
    out->buy = bookmaker(BUY_BOOK);
    out->sell = bookmaker(SELL_BOOK);  
    out->ticker = ticker;
    return out;
}

/*
 * free_exchange: free the space associated with the
 *   exchange
 *
 * exchange: an exchange
 */
void free_exchange(exchange_t *exchange) {
    free_book_lst (exchange->buy);
    free_book_lst (exchange->sell);
    free (exchange);
}

/* 
 * process_order: process an order. Returns a action_report for the
 *   actions completed in the process.
 *
 * exchange: an exchange
 * ord_str: a string describing the order (in the expected format)
 * time: the time the order was placed.
 * 
 * Creates order_t from mk_order_from line, then uses compute order //
 * add action to add to books and action reports. Pending shares starts as 
 * true to run the function, and then is turned false when there are no 
 * pending shares
 * Transaction is another order_t which represents the output from putting the
 * orgional order through compute_order. The logic for having two seperate 
 * orders is to compare their orefs to determine if a transaction is taking 
 * place. (if an oref is same between order and transaction, then no order)
 * Transactions are freed after every execute, and order is also freed if there
 * are NO pending shares AFTER a transaction. If there are pending shares,
 * it will be sent through compute order again and if its a buy/ sell order 
 * it will not be freed and instead added to the list.
 */
action_report_t  *process_order(exchange_t *exchange, char *ord_str, int time){
    assert(exchange != NULL);
    assert(ord_str != NULL);
    action_report_t *out = mk_action_report(exchange->ticker);
    order_t *order = mk_order_from_line(ord_str,time);
    order_t *transaction;
    bool pendshares = true;

    while(pendshares){
        if (is_buy_order(order)){ 
            transaction = compute_order(exchange->buy, exchange->sell, 
                order, &pendshares);
        } else { 
            transaction = compute_order(exchange->sell, exchange->buy,
                order, &pendshares);
        }
        if (transaction->oref != order->oref){
            add_action(out,EXECUTE,transaction->oref,transaction->price,
                transaction->shares);
            free_order(transaction);
            if (!pendshares){
                free_order(order);
            }
        } else {
            if (is_buy_order(order)){
                add_action(out,BOOKED_BUY,transaction->oref,
                    transaction->price, transaction->shares);
            } else {
                add_action(out,BOOKED_SELL,transaction->oref,
                    transaction->price, transaction->shares);
            }
        }
    }
    return out;
}

/*
 * print_exchange: print the contents of the exchange
 *
 * exchange: the exchange.
 */
void print_exchange(exchange_t *exchange) {
    printf("The Ticker of this exchange is: %s \n",exchange->ticker);
    print_contents_of_book(exchange->buy);
    print_contents_of_book(exchange->sell);   
    printf("\n"); 
}