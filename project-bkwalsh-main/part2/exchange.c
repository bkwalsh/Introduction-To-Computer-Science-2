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

/* book_and_ar: Adds an order to its respective book, and adds the action
 * of booking to the action report. Used when no matches are suitable.
 * ar: action report
 * order: desired order to add
 * exchange: exchange to add the order to
 * returns: modified action report (books are also modified by this function)
 */
action_report_t *book_and_ar(action_report_t *ar, order_t *order, 
    exchange_t *exchange){
    if (is_buy_order(order)){
        add_action(ar,BOOKED_BUY,order->oref,order->price, order->shares);
        insert(exchange->buy, order);
    } else {
        add_action(ar,BOOKED_SELL,order->oref,order->price, order->shares);
        insert(exchange->sell, order);
    }
    return ar;    
}



/* 
 * process_order: process an order. Returns a action_report for the
 *   actions completed in the process.
 *
 * exchange: an exchange
 * ord_str: a string describing the order (in the expected format)
 * time: the time the order was placed.
 * 
 * Returns: An action report detailing what actions took place if any
 */
action_report_t  *process_order(exchange_t *exchange, char *ord_str, int time){
    assert(exchange != NULL);
    assert(ord_str != NULL);
    action_report_t *out = mk_action_report(exchange->ticker);
    order_t *order = mk_order_from_line(ord_str,time);
    order_t *cancel_var = NULL;
    bool is_buy = is_buy_order(order);
    bool sv=false;
    if (is_c_buy_order (order)) {
       compute_cancel(exchange->buy, order, &cancel_var, &sv);
        if (cancel_var != NULL){
            add_action(out,CANCEL_BUY,cancel_var->oref,cancel_var->price,
                cancel_var->shares);
            free_order(cancel_var);
        }
    } else if (is_c_sell_order (order)) {
        compute_cancel(exchange->sell, order, &cancel_var, &sv);
        if (cancel_var != NULL){
            add_action(out,CANCEL_SELL,cancel_var->oref,cancel_var->price,
                cancel_var->shares);
            free_order(cancel_var);
        }
    } else {
        while (true) {
            order_t* best_fit;
            if (is_buy) {
                best_fit=best_order(exchange->sell);
            } else {
                best_fit=best_order(exchange->buy);
            }
            if (best_fit==NULL) {
                return book_and_ar(out,order,exchange);
            } else {
                bool pendshares=true;
                bool rm_pend=false;
                if (check_transaction(best_fit, order)){
                    order_t *transaction=update_order_shares(best_fit, order,
                        &pendshares, &rm_pend);
                    add_action(out,EXECUTE,best_fit->oref,best_fit->price,
                        transaction->shares);               
                    if(rm_pend) {
                        if (is_buy) {                                                    
                            rm_val(exchange->sell,0);
                            free_order(best_fit);
                        } else {                                                        
                            rm_val(exchange->buy,0);    
                            free_order(best_fit);                    
                        }
                    }
                    free_order(transaction);
                    if(!pendshares){
                        free_order(order);
                        return out;
                    }
                } else {
                    return book_and_ar(out,order,exchange);
                }
            } 
        }
    }
    if (!sv){
        free_order(order);
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