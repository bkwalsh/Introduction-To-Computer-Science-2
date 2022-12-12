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


/* 
 * update_order_shares: Updates the amount of shares when a transaction is
 * being made. Returns an order to be reported to the action report
 * as well as modifying the shares on either pending list
 *
 * Returns: Updated order
 */
order_t *update_order_shares(order_t *pend_order, order_t *new_order,
    bool *pendshares, bool *rm_pend);

/* 
 * rm_val: Swaps the root val with desired removal index. Sets index to null
 * if only val is removed
 * 
 * book: Where the value is to be removed from
 * rm_index: index of the desired removal val
 *
 * Returns: Nothing, modifies BST, modifes book->num_occupied up to date
 */
void rm_val(book_t *book, int rm_index);


/*
 * insert: Inserts a value into a book in the appropriate place. Modifes memory
 * as needed
 * 
 * book: Book where the value is to be added to
 * inc_order: incoming order to be added
 *
 * Returns: Nothing, modifies BST, modifes book->num_occupied up to date, adds
 *  order and arranges memory. Uses sift_up for ordering
 */
void insert(book_t *book, order_t *inc_order);

/* 
 * best_order: Returns the "best order" for a book. Will be the first order
 * Best order is first order in priority lists. If book is empty, returns NULL
 * 
 * book: Book where the order is to be drawn from
 *
 * Returns: Desired order if book is not empty, otherwise NULL
 */
order_t *best_order(book_t *book);

/* 
 * compute_cancel: Removes a cancel order from a book if possible
 * Logic of doing compute_cancel in book.c is because doing the array work
 * in exchange.c would enable exchange.c to see the length/ amount of orders
 * in the current heap if the cancel DNE. This felt like a violation of the 
 * opaqueness of the book type. Compute cancel considers the logic of cancels
 * and removes orders appropriately
 * 
 * book: Book to query for a cancel order
 * order: Incoming cancel order
 * out: out paremeter set to cancel order to add to action report later
 *
 * Returns: None. Modifes the book if appropriate and sets out parameter
 */
void compute_cancel(book_t *book, order_t *order, order_t **out, bool *sv);

#endif
