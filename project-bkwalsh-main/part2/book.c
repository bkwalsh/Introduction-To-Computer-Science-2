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
 

struct book {
    enum book_type type; 
    int num_slots;
    int num_occupied;
    order_t **array;
};

#define INIT_SLOTS 10
#define SLOTS_MULTIPLIER 2

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
            return b1->price < b2->price; 
        } 
    }
    return b1->time < b2->time;
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
    out->num_slots = 5;
    out->num_occupied = 0;
    out->array= (order_t**)malloc(sizeof(order_t*) * INIT_SLOTS);
    if (out->array == NULL) {
        fprintf(stderr, "book_t: Unable to allocate\n");
        exit(1);
    }
    return out;
}


/* 
 * free_array: Frees an array of order_lists
 *
 * array: Array of orders
 * size: length of array
 * 
 * Returns: Nothing
 */
void free_array(order_t **array, int size){
    for(int i=0;i<size;i++){
        free_order(array[i]);
    }
}

/* 
 * free_book_lst: Frees all values in a book
 *
 * value: book to be freed
 * 
 * Returns: Nothing
 */
void free_book_lst(book_t *value){
    free_array(value->array, value->num_occupied);
    free(value->array);
    free (value);
}


/* 
 * print_order_lst: Prints all values in an array order list
 *
 * lst: list to be printed
 */
void print_array(order_t *array[], int len) {
    for(int i = 0;i < len;i++){
        print_order(array[i]);
    }
}


/* 
 * print_contents_of_book: Prints all the contents in a book list
 *
 * book: book to be printed
 */
void print_contents_of_book(book_t *book){
    if (book->type == BUY_BOOK) {
        printf("Buy book: \n");
    } else {
        printf("Sell book: \n");
    }
    print_array(book->array, book->num_occupied);
}



/* 
 * update_order_shares: Updates the amount of shares when a transaction is
 * being made. Returns an order to be reported to the action report
 * as well as modifying the shares on either pending list
 *
 * Returns: Updated order
 */
order_t *update_order_shares(order_t *pend_order, order_t *new_order,
    bool *pendshares, bool *rm_pend){
    if (pend_order->shares == new_order->shares){
        *pendshares = false;
        *rm_pend = true;
        new_order = copy_order(pend_order); 
        return new_order;
    } else if (pend_order->shares > new_order->shares ){
        *pendshares = false;
        *rm_pend = false;
        pend_order->shares = pend_order->shares - new_order->shares;
        int hold= new_order->shares;
        new_order = copy_order(pend_order); 
        new_order->shares = hold; 
        return new_order;
    } else{
        *pendshares = true;
        *rm_pend = true;
        new_order->shares = new_order->shares - pend_order->shares;
        order_t *out_order = copy_order(pend_order); 
        return out_order;
    }
}


/* 
 * swap: swaps to orders
 * a, b= orders to be swapped
 *
 * Returns: nothing
 */
void swap(order_t *a, order_t *b) {
    order_t temp = *a;
    *a = *b;
    *b = temp;
}

/* 
 * parent: returns parent index of a node for a BST. If parent DNE, returns -1
 *
 * Returns: Int (parent index or -1)
 */
int parent(int val){
    val=(val-1)/2;
    if (val>=0){
        return val;
    } else{
        return -1;
    }
}

/* 
 * left_child: returns left child index of a node for a BST
 *
 * Returns: Int, left child index
 */
int left_child(int val){
    val=(val*2)+1;
    return val;
}

/* 
 * right_child: returns right child index of a node for a BST
 *
 * Returns: Int, right child index
 */
int right_child(int val){
    val=(val*2)+2;
    return val;
}


/* 
 * sift_up: swaps values from the bottom up. Adds a node to the 
 *  bottom and then swaps it into place
 * array: array of orders (in BST ordering)
 * index: order to be added
 * size: length of array
 *
 * Returns: Nothing, modifies BST
 */
void sift_up(order_t *array[], int index, int size) {
    int parent_index;
    while(true){
        if (index==0){
            return;
        }
        parent_index=parent(index);
        if (order_cmp(array[index],array[parent_index])){
            swap(array[index], array[parent_index]);
            index=parent_index;
        } else{
            return;
        }
    }
}


/* 
 * sift_down: swaps values from the top down to reorginze the order at given
 * index
 * array: array of orders (in BST ordering)
 * index: index of order to be reorginzed
 * size: length of array
 *
 * Returns: Nothing, modifies BST
 */
void sift_down(order_t *array[],int size,int index){
    while(true){
        int min_index=index;
        int left = left_child(index), right = right_child(index);
        if(left < size) {
            if(order_cmp(array[left], array[min_index])) min_index = left;
        }
        if(right < size) {
            if(order_cmp(array[right], array[min_index])) min_index = right;
        }
        if(min_index != index){
            swap(array[index], array[min_index]);
            index=min_index;
        } else{
            return;
        }
    }
}

/* 
 * remove_min: Removes the last value of a BST by swapping it with the root
 * and calling sift down
 * array: array of orders (in BST ordering)
 * size: length of array
 *
 * Returns: Nothing, modifies BST
 */
void remove_min(order_t *array[], int *size) {
    order_t *last = array[*size - 1];
    array[0] = last;
    *size = *size - 1;
    sift_down(array, *size, 0);
}


/* 
 * rm_val: Swaps the root val with desired removal index. Sets index to null
 * if only val is removed
 * 
 * book: Where the value is to be removed from
 * rm_index: index of the desired removal val
 *
 * Returns: Nothing, modifies BST, modifes book->num_occupied up to date
 */
void rm_val(book_t *book, int rm_index){
    order_t **array=book->array;
    int size=book->num_occupied--;
    swap(array[0],array[rm_index]);
    remove_min(array, &size);
    if (book->num_occupied==0){
        book->array[0]=NULL;
    } 
}

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
void insert(book_t *book, order_t *inc_order) {
    int nt = book->num_occupied;
    int ns = book->num_slots;

    if (nt == ns) {
        int new_num_slots = (int) (ns * SLOTS_MULTIPLIER);
        book->array = (order_t **) ck_realloc(book->array, 
					      sizeof(order_t*) * new_num_slots, 
					      "insert");
        book->num_slots = new_num_slots;
        ns = new_num_slots;
    }    
    assert(nt < ns);
    book->array[nt]=inc_order;
    book->num_occupied++;
    sift_up(book->array, book->num_occupied-1, book->num_occupied);
}


/* 
 * best_order: Returns the "best order" for a book. Will be the first order
 * Best order is first order in priority lists. If book is empty, returns NULL
 * 
 * book: Book where the order is to be drawn from
 *
 * Returns: Desired order if book is not empty, otherwise NULL
 */
order_t *best_order(book_t *book){
    if (book->num_occupied==0){
        return NULL;
    }
    return book->array[0];
}



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
 * sv: flag to not free shares of order if there are remaining shares
 * 
 * Returns: None. Modifes the book if appropriate and sets out parameter
 */
void compute_cancel(book_t *book, order_t *order, order_t **out, bool *sv){
    int search_oref=order->oref;
    for (int i=0; i<book->num_occupied; i++){ 
        if (book->array[i]->oref==search_oref){
            if(order->shares == book->array[i]->shares){
                *out=book->array[i]; 
                rm_val(book,i);
                *sv=false;
                return;
            } else if (order->shares > book->array[i]->shares){
                *out=book->array[i]; 
                rm_val(book, i);
                *sv=false;                
                return;
            } else{
                *out=order;
                book->array[i]->shares=book->array[i]->shares-order->shares;
                *sv=true;                
                return;
            }
        }
    }
    return;
}
