/*
 * CS 152, Spring 2022
 * Exchange Interface
 * 
 * Do NOT modify this file.
 */


#ifndef EXCHANGE_H
#define EXCHANGE_H

/* The type for an exchange.  This type is opaque */
typedef struct exchange exchange_t;

/* 
 * mk_exchange: make an exchange for the specified ticker symbol
 *
 * ticker: the ticker symbol for the stock
 *
 * Returns: an exchange
 */
exchange_t *mk_exchange(char *ticker);


/*
 * free_exchange: free the space associated with the
 *   exchange
 *
 * exc: an exchange
 */
void free_exchange(exchange_t *exc);


/* 
 * process_order: process an order. Returns a action_report for the
 *   actions completed in the process.
 *
 * exc: an exchange
 * ord_str: a string describing the order (in the expected format)
 * time: the time the order was placed.
 */
action_report_t  *process_order(exchange_t *exchange, char *ord_str, int time);


/*
 * print_exchange: print the contents of the exchange
 *
 * exc: the exchange.
 */
void print_exchange(exchange_t *exc);

#endif
