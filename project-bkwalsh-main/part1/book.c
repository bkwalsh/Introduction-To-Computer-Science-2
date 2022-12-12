/*
 * CS 152, Spring 2022
 * Book Data Structure Implementation
 * 
 * You will modify this file.
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "order.h"
#include "book.h"
#include "util.h"
 
/* These types are not visible outside this file.
 * Do NOT move them.
 */
typedef struct order_list order_list_t;


struct order_list {
  order_t *order;
  order_list_t *next;
};

struct book {
    enum book_type type;     // BUY_BOOK or SELL_BOOK
    order_list_t *pending;   // orders still in play.
};


/* 
 * order_cmp: Compares two orders. If b1 should come before b2, returns true;
 *
 * b1: An order, b2: a different order
 * 
 * Uses is_buy_order and is_sell_order to measure what
 * order_cmp should be based
 * on (either less than or greater than, based on buyer seller hierarchy) 
 * if price is equal returns time as tiebreaker (previous orders go first
 *  for both cases)
 * 
 * Note: only measuring b1 for being buyer/ seller order because the way
 * that I intend to apply order_cmp is in cases where orders to order_list_t
 * are already orginized. This is why there is not any error catching to 
 * be certain that both b1 and b2 are of the same order type 
 * 
 * Returns: boolean, true if b1 should come before b2
 */
bool order_cmp(order_t *b1, order_t *b2) { 
    assert(b1 != NULL);
    assert(b2 != NULL);
    if (b1->price != b2->price){
       if (is_buy_order(b1)) {
            return b1->price > b2->price;
        } else if (is_sell_order(b1)){
            return b1->price < b2->price ; 
        } 
    }
    return b1->time < b2->time;
}

/* 
 * make_order_list: Makes a new node on an order list with the given element 
 * first. Takes an order_list_t to determine what comes behind it
 *
 * first: first element in a list
 * rest: the list following that element  (if any)
 * 
 * Returns: List with the first element attached
 */
order_list_t *make_order_list(order_t *first, order_list_t *rest) { 
    order_list_t *lst = (order_list_t*)malloc(sizeof(order_list_t));
    if (lst == NULL) {
        fprintf(stderr, "make_intlist: Unable to allocate\n");
        exit(1);
    }
    lst->order= first;
    lst->next = rest;
    return lst;
}

/* 
 * add_to_order_list: Uses make_order_list to add a new node to an existing
 * order_list_t. Uses order_cmp to consider placing. Returns an order list
 * ordered based on if the incoming order is buy or sell
 * returns a new list with the given node if lst is null
 * 
 * lst: pointer to an order_list_t (assumed to be sorted based on applicaiton)
 * inc_order: order to be added to the sorted list
 * 
 * Returns: modified and sorted order_list_t
 */
order_list_t *add_to_order_list(order_list_t *lst, order_t *inc_order) {
	if (lst == NULL) {
		return make_order_list(inc_order, NULL);
	}
    order_list_t *head = lst;
    if(lst->next != NULL && lst->next->order != NULL && 
        order_cmp(inc_order,lst->next->order)) {
        lst->next = make_order_list(inc_order, lst->next);
        return head;
    }
    order_list_t *prev = lst;
    while(lst->next != NULL) {
        lst = lst->next;
        if (order_cmp(inc_order,lst->order)){
            prev->next = make_order_list(inc_order, prev->next);
            return head;
        }
        prev = lst;
    }
    lst->next = make_order_list(inc_order, NULL); 
    return head;
} 

/* 
 * bookmaker: Creates a new book with an empty order_list 
 *
 * val: enum book_type indicating what type the book should have
 * 
 * Returns: Initalized book
 */
book_t *bookmaker(enum book_type val){
    book_t *out = (book_t*)malloc(sizeof(book_t));
    if (out == NULL) {
        fprintf(stderr, "book_t: Unable to allocate\n");
        exit(1);
    }
    out->type = val;
    out->pending = add_to_order_list(NULL, NULL);
    return out;
}



/* 
 * free_order_lst: Frees all values in an order_lst
 *
 * lst: list to be freed
 */
void free_order_lst(order_list_t *lst) {
    order_list_t *next;
    while(lst != NULL) {
        next = lst->next;
        if (lst->order != NULL){
            free_order(lst->order);
            free(lst);
        }
        lst = next;
    }
}

/* 
 * free_book_lst: Frees all values in a book
 *
 * value: book to be freed
 */
void free_book_lst(book_t *value){
    if (value->pending != NULL){
        free_order_lst(value->pending);        
    }
    free(value->pending);
    free (value);
}



/* 
 * print_order_lst: Prints all values in an order list
 *
 * lst: list to be printed
 */
void print_order_lst(order_list_t *lst) {
    if (lst->next != NULL){
        lst = lst->next;
    }
    order_list_t *curr = lst;
    while (curr != NULL) {
        if (curr->order != NULL){
            print_order(curr->order);
            printf("\n");
        } else {
            printf("Order List is Empty\n");
        }
        curr = curr->next;
    }
}


/* 
 * print_contents_of_book: Prints all the contents in a book list
 *
 * book: book to be printed
 */
void print_contents_of_book(book_t *book){
    if (book->type==BUY_BOOK) {
        printf("Buy book: \n");
    } else {
        printf("Sell book: \n");
    }
    print_order_lst(book->pending);
}



/* 
 * check_transaction: Compares 2 orders together to check if a transaction
 * is possible, based on if the orders are sells or buys and their prices
 * uses the buyer/seller compatability formula in the write up
 *
 * Returns bool, true if a transaction is possible, false otherwise
 */
bool check_transaction(order_t *pend_order, order_t *incoming_order){
    if (is_buy_order (incoming_order)){
        if(incoming_order->price >= pend_order->price) {
           return true;
        } else {
            return false;
        }
    } else {
        if(incoming_order->price <= pend_order->price) {
           return true;
        } else {
            return false;
        }
    }
}


/* 
 * update_order_shares: Updates the amount of shares when a transaction is
 * being made. Returns an order to be reported to the action report
 * as well as modifying the shares on either pending list
 *
 * Also takes/ modifes two boolean pointers:
 * rm_pend: true if the current pending order needs to be freed. This will be
 * done in compute_order
 * pendshares: true if there are leftover shares (this is used to determine if 
 * compute_order needs to be ran again)
 * 
 * 3 cases:
 * Pend_order shares> new_order:
 * Update pending order shares, new_order is removed
 * 
 * Pend_order shares= new_order:
 * Remove pend_order and new_order
 * 
 * Pend_order shares< new_order:
 * Removed pend_order, update new_order and run it again via pendshares flag
 * 
 * 
 */
order_t *update_order_shares(order_t *pend_order, order_t *new_order, 
    bool *pendshares, bool *rm_pend){
    if (pend_order->shares == new_order->shares){
        *pendshares = false;
        *rm_pend = true;
        return pend_order;
    } else if (pend_order->shares > new_order->shares ){
        *pendshares = false;
        *rm_pend = false;
        pend_order->shares = pend_order->shares - new_order->shares;
        order_t *out = copy_order(pend_order); 
        out->shares = new_order->shares; 
        return out;
    } else{
        *pendshares = true;
        *rm_pend = true;
        new_order->shares = new_order->shares - pend_order->shares;
        return pend_order;
    }
}

/* 
 * compute_order: This is the heart of process order, takes two books and an
 * order to be computed and a pending shares flag
 * 
 * Will compare the order to the opposite book, if there is no match in the 
 * first value (based on check_transaction helper function), 
 * the order will be added to it's own book order list. If the first isn't a 
 * match, none of the values will be, as it is an ordered list set to  
 * maximize transactions 
 * 
 * The returned order will be the given order set to be reported in 
 * Process_order
 * 
 * If there is no match, the order will be booked into it's respective book
 * 
 * If there is a match, it will update shares based on update_order_shares.
 * Then it will remove shares if need be (based on rm_pend flag)
 * The returned order will be the pending order
 *
 * It will also return pendshares as true if there are remaining shares 
 * in which case it will loop over this part of process_order again 
 * 
 * Each compute order will add an action, as implemented in exchange.c
 */
order_t *compute_order(book_t *samebook, book_t *oppbook, order_t *order, 
    bool *pendshares) {
    order_list_t *samelst = samebook->pending;
    order_list_t *opplst = oppbook->pending;
    if (opplst->next == NULL ){
        samebook->pending = add_to_order_list(samelst, order);
        *pendshares = false;
        return order;
    } 
    order_list_t *head = samelst;
    samelst = samelst->next;
    order_list_t *prev = opplst;
    opplst = opplst->next;
    bool rm_pend = false;
    if( check_transaction (opplst->order,order)){
        order_t *upd_order = update_order_shares (opplst->order, order, 
            pendshares, &rm_pend);
        if(rm_pend){ 
            prev->next = opplst->next;
            free(opplst);     
        }
        return upd_order;
    } else{
        samebook->pending = add_to_order_list(head, order);
        *pendshares = false;
        return order;
    }
}