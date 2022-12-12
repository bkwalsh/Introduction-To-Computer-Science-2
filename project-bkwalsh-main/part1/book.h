/*
 * CS 152, Spring 2022
 * Book Data Structure Interface.
 * 
 * You will modify this file.
 */

/********* Do not modify this section *********/
#ifndef BOOK_H
#define BOOK_H

enum book_type {BUY_BOOK, SELL_BOOK};

/* The book type is opaque, which means that the client cannot reach
 * into the implementation and make changes directly.  Do NOT move the
 * structure type into this file.
 */
typedef struct book book_t;


/********** Modify below **********************/
// The prototypes for your public book functions
// go here.  Don't forget to include header
// comments that describe the purpose of the
// functions, the arguments, and the return value.

/* 
 * bookmaker: Creates a new book with an empty order_list 
 *
 * val: enum book_type indicating what type the book should have
 * 
 * Returns: Initalized book
 */
book_t *bookmaker(enum book_type val);

/* 
 * compute_order: This is the heart of process order, takes two books and order
 * and a pending shares boolean flag
 * 
 * Will compare the order to the opposite book, if there is no match (based on
 * check_transaction helper function), the order will be added to it's own
 * book order list, pending shares will equal false
 * 
 * The returned order will be input order set to be added to the action
 *  report in Process_order
 * 
 * If there is a match, it will update shares based on update_order_shares.
 * In this case the returned order will be the pending order, which will be 
 * removed if incomng orders>= pending orders 
 * if pending orders > incoming orders, pending is updated, not removed, 
 * and returned
 * if incoming orders> pending orders, it will loop again through
 * returning pendshares as true if there are remaining shares 
 * in which case it will loop over this part of process_order again
 */
order_t *compute_order(book_t *samebook, book_t *oppbook, order_t *order, 
    bool *pendshares);

/* 
 * free_book_lst: Frees all values in a book
 *
 * value: book to be freed
 */
void free_book_lst(book_t *lst);

/* 
 * print_contents_of_book: Prints all the contents in a book list
 *
 * book: book to be printed
 */
void print_contents_of_book(book_t *book);

#endif
