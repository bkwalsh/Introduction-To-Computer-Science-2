/*
 * CS 152, Spring 2022
 * Order Data Structure Interface
 *
 * You can add function prototypes to this file, but you may not
 * change the existing prototypes.
 */


#ifndef ORDER_H
#define ORDER_H

typedef struct order {
    char venue;
    char *ticker;
    char type;
    char book;
    int shares;
    long long price;
    long long oref;
    int time;
} order_t;

order_t *copy_order(order_t *order);

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
 * Returns: an order
 */
order_t *mk_order(char venue, char *ticker, char typ, char book,
		          int shares, long long price, long long oref, 
                  int time);


/*

 * mk_order_from_line: constructs an order from an string describing
 *  the order and the time of the order.
 *
 * line: a string describing the order.  
 *  Format:Venue,Ticker,Type,Book,Shares,Price,Oref
 * time: the time the order was placed
 *
 * Returns: an order
 */
order_t *mk_order_from_line(char *line, int time);


/* 
 * free_order: free an order 
 */
void free_order(order_t *order);


/* 
 * to_string_order: convert an order to a string
 *  Format:Venue,Ticker,Type,Book,Shares,Price,Oref,Time
 *
 * order: the order to convert
 *
 * Returns: string in the format listed above
 */
char *to_string_order(order_t *order);


/* 
 * print_order: print an order to stdout
 *   Format:Venue,Ticker,Type,Book,Shares,Price,Oref,Time
 *
 * order: the order to convert
 * padding: string of spaces to print before order
 */
void print_order(order_t *order);


/* 
 * print_order: print an order to stdout w/ prefix prepended.
 *   Format:Venue,Ticker,Type,Book,Shares,Price,Oref,Time
 *
 * order: the order to convert
 * prefix: string to print in front of the order
 */
void print_order_with_prefix(order_t *order, char *prefix);


/* 
 * is_buy_order: is the order a buy order
 *
 * o: a pointer to an order struct
 *
 * Returns: true, if the order is a buy order, false otherwise.
 */
bool is_buy_order(order_t *order);

/* 
 * is_sell_order: is the order a sell order
 *
 * o: a pointer to an order struct
 *
 * Returns: true, if the order is a sell order, false otherwise.
 */
bool is_sell_order(order_t *order);


/* 
 * is_c_buy_order: is the order a cancel buy order?
 *
 * o: an order
 *
 * Returns: true, if the order is a cancel buy order, false otherwise.
 */
bool is_c_buy_order(order_t *order);

/* 
 * is_c_sell_order: is the order a cancel sell order?
 *
 * o: an order
 *
 * Returns: true, if the order is a cancel sell order, false otherwise.
 */
bool is_c_sell_order(order_t *order);
#endif
