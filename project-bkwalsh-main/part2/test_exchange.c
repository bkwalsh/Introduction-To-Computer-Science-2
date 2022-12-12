/*
 * CS 152, Spring 2022
 * Project Automated Tests 
 *
 * Run make test_exchange to compile these tests and ./test_exchange
 * to run these tests.
 *
 * Please note: the test below does not represent the full automated
 * test suite that will be used for grading the project
 * 
 * Do not modify this file.
 */

#include <stdbool.h>
#include <criterion/criterion.h>
#include <string.h>
#include <stdio.h>
#include "action_report.h"
#include "exchange.h"

// The type for representing actions in the report
typedef struct action_s {
    enum action action;
    long long oref;
    long long price;
    int shares;
} action_t;

// structure for represeting a action report
struct action_report {
    char *ticker;
    int num_actions;   // the number of actions recorded
    int num_slots;    // the number of slots in the array
    action_t *actions;  // an array of struct actions
};


void verify_action_report(action_report_t *expected, action_report_t *actual) {
  cr_assert(actual != NULL);
  cr_assert(actual->num_actions == expected->num_actions);
  cr_assert(actual->ticker != NULL);
  cr_assert(strcmp(actual->ticker, expected->ticker) == 0);

  cr_assert(actual->actions != NULL);
  for (int i=0; i < actual->num_actions; i++) {
    cr_assert(actual->actions[i].action == expected->actions[i].action);
    cr_assert(actual->actions[i].oref == expected->actions[i].oref); 
    cr_assert(actual->actions[i].price == expected->actions[i].price);   
    cr_assert(actual->actions[i].shares == expected->actions[i].shares);       
  }
}

Test(exchange, test0) {
  char *ticker = "UOCCS";
  char *order_strs[] = 
    { "I,UOCCS,A,S,100,550000,4000",
      "I,UOCCS,A,B,70,558000,1070"
    };

  int times[] = {10, 20};

  exchange_t *exch = mk_exchange(ticker);
  cr_assert(exch != NULL);

  action_report_t *expected_ar1 = mk_action_report(ticker);
  add_action(expected_ar1, BOOKED_SELL, 4000, 550000, 100);
  action_report_t *actual_ar1 = process_order(exch, order_strs[0], times[0]);
  verify_action_report(expected_ar1, actual_ar1);
  free_action_report(expected_ar1);
  free_action_report(actual_ar1);

  action_report_t *expected_ar2 = mk_action_report(ticker);
  add_action(expected_ar2, EXECUTE, 4000, 550000, 70);
  action_report_t *actual_ar2 = process_order(exch, order_strs[1], times[1]);
  verify_action_report(expected_ar2, actual_ar2);
  free_action_report(expected_ar2);
  free_action_report(actual_ar2);

  free_exchange(exch);  
}


Test(cancel, test0) {
  char *ticker = "UOCCS";
  char *order_strs[] = 
    { "I,UOCCS,A,S,100,550000,4000",
      "I,UOCCS,A,B,70,558000,1070",
      "I,UOCCS,C,S,50,550000,4000"
    };

  int times[] = {10, 20, 30};

  exchange_t *exch = mk_exchange(ticker);
  cr_assert(exch != NULL);

  action_report_t *expected_ar1 = mk_action_report(ticker);
  add_action(expected_ar1, BOOKED_SELL, 4000, 550000, 100);
  action_report_t *actual_ar1 = process_order(exch, order_strs[0], times[0]);
  verify_action_report(expected_ar1, actual_ar1);
  free_action_report(expected_ar1);
  free_action_report(actual_ar1);

  action_report_t *expected_ar2 = mk_action_report(ticker);
  add_action(expected_ar2, EXECUTE, 4000, 550000, 70);
  action_report_t *actual_ar2 = process_order(exch, order_strs[1], times[1]);
  verify_action_report(expected_ar2, actual_ar2);
  free_action_report(expected_ar2);
  free_action_report(actual_ar2);

  action_report_t *expected_ar3 = mk_action_report(ticker);
  add_action(expected_ar3, CANCEL_SELL, 4000, 550000, 30);
  action_report_t *actual_ar3 = process_order(exch, order_strs[2], times[2]);
  verify_action_report(expected_ar3, actual_ar3);
  free_action_report(expected_ar3);
  free_action_report(actual_ar3);

  free_exchange(exch);
}

