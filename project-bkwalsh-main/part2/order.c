/*
 * CS 152, Spring 2022
 * Order Data Structure Implementation
 * 
 * You can add functions to this file, but you may not change
 * the existing functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

#include "util.h"
#include "order.h"

#define MAX_ORDER_LEN 1000
#define MAX_TICKER_LEN 10
#define NUM_FIELDS 8

/*
 * mk_order: make an order from the parts
 *
 * venue: the venue the order is from
 * ticker: the ticker symbol for the company
 * typ: the type of order 
 * book: 'B' for buy or 'S' for sell
 * shares: the number of shares to be bought or sold
 * price: the desired price
 * oref: a unique identifier
 * time: the time the order was placed
 *
 * Returns: a pointer to an order struct.
 */
order_t *mk_order(char venue, char *ticker, char typ, char book, 
		  int shares, long long price, long long oref, int time) {
    order_t *o = (order_t*) ck_malloc(sizeof(order_t), "mk_order");
    o->venue = venue;
    o->ticker = ck_strdup(ticker, "mk_order");
    o->type = typ;
    o->book = book;
    o->shares = shares;
    o->price = price;
    o->oref = oref;
    o->time = time;
    printf("making...%lld\n", oref);
    return o;
}


/*
 * mk_order_from_line: constructs an order from an string describing
 *  the order and the time of the order.
 *
 * line: a string describing the order.  
 *  Format:Venue,Ticker,Type,Book,Shares,Price,Oref
 * time: the time the order was placed
 *
 * Returns: the order or NULL, if the string did not parse
 *   properly.
 */
order_t *mk_order_from_line(char *line, int time) {
    char venue, typ, book;
    char ticker[MAX_TICKER_LEN+1];
    int shares;
    long long price, oref;

    int num_matched = sscanf(line, "%c,%[^,],%c,%c,%d,%lld,%lld",
                             &venue, ticker, &typ, &book, &shares, 
                             &price, &oref);

    if (num_matched != NUM_FIELDS-1) {
        return NULL;
    }

    return mk_order(venue, ticker, typ, book, shares, price, oref, time);
}

/*
 * copy_order: make a copy of an order
 *
 * Returns: a pointer to an order struct.
 */
order_t *copy_order(order_t *order) {
  return mk_order(order->venue, order->ticker, order->type, order->book, 
		  order->shares, order->price, order->oref, order->time);
}


/* 
 * free_order: free an order 
 *
 * order: the order to free
 */
void free_order(order_t *order) {
    printf("freeing...%lld\n", order->oref); 
    free(order->ticker);
    free(order);
}


/* 
 * to_string_helper: convert an order to a string, fills in provided space
 *  Format:Venue,Ticker,Type,Book,Shares,Price,Oref,Time
 *
 * tmp_str: an array of characters to fill
 * tmp_str_size: the size of tmp_str
 * order: the order to convert
 */
void to_string_helper(char tmp_str[], int tmp_str_size, order_t *order) {
    assert(order != NULL);
    snprintf(tmp_str, tmp_str_size, "%c,%s,%c,%c,%d,%lld,%lld @ %d",
             order->venue,
             order->ticker,
             order->type,
             order->book,
             order->shares,
             order->price,
             order->oref,
             order->time);
}


/* 
 * to_string_order: convert an order to a string
 *  Format:Venue,Ticker,Type,Book,Shares,Price,Oref,Time
 *
 * order: the order to convert
 *
 * Returns: string in the format listed above
 */
char *to_string_order(order_t *order) {
    char tmp_str[MAX_ORDER_LEN+1];

    to_string_helper(tmp_str, MAX_ORDER_LEN+1, order);
    return ck_strdup(tmp_str, "to_string_order");
}


/* 
 * print_order: print an order to stdout
 *   Format:Venue,Ticker,Type,Book,Shares,Price,Oref,Time
 *
 * order: the order to convert
 */
void print_order(order_t *order) {
    print_order_with_prefix(order, "");
}


/* 
 * print_order: print an order to stdout
 *   Format:Venue,Ticker,Type,Book,Shares,Price,Oref,Time
 *
 * order: the order to convert
 */
void print_order_with_prefix(order_t *order, char *padding) {
    char tmp_str[MAX_ORDER_LEN+1];

    if (padding == NULL) {
      padding = "";
    }

    to_string_helper(tmp_str, MAX_ORDER_LEN+1, order);
    printf("%s%s\n", padding, tmp_str);
}


/* 
 * is_buy_order: is the order a buy order
 *
 * o: an order
 *
 * Returns: true, if the order is a buy order, false otherwise.
 */
bool is_buy_order(order_t *order) {
    assert(order != NULL);
    return ((order->type == 'A') && (order->book == 'B'));
}


/* 
 * is_sell_order: is the order a sell order?
 *
 * o: an order
 *
 * Returns: true, if the order is a sell order, false otherwise.
 */
bool is_sell_order(order_t *order) {
    assert(order != NULL);
    return ((order->type == 'A') && (order->book == 'S'));
}

/* 
 * is_c_buy_order: is the order a cancel buy order?
 *
 * o: an order
 *
 * Returns: true, if the order is a cancel buy order, false otherwise.
 */
bool is_c_buy_order(order_t *order) {
    assert(order != NULL);
    return ((order->type == 'C') && (order->book == 'B'));
}

/* 
 * is_c_sell_order: is the order a cancel sell order?
 *
 * o: an order
 *
 * Returns: true, if the order is a cancel sell order, false otherwise.
 */
bool is_c_sell_order(order_t *order) {
    assert(order != NULL);
    return ((order->type == 'C') && (order->book == 'S'));
}