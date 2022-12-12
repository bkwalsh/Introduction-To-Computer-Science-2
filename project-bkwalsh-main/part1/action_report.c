/*
 * CS 152, Spring 2022
 * Action Report Data Structure
 * 
 * Do NOT modify this file
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "action_report.h"
#include "util.h"

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

#define INIT_SLOTS 2
#define SLOTS_MULTIPLIER 1.5

/*
 * mk_action_report: make an empty action report
 *
 * ticker: the ticker symbol for the report
 *
 * Returns: an empty action report
 */
action_report_t *mk_action_report(char *ticker) {
    char *fn_name = "mk_action_report";
    action_report_t *ar =
      (action_report_t *) ck_malloc(sizeof(action_report_t), 
				    fn_name);
    ar->ticker = ck_strdup(ticker, fn_name);
    ar->num_actions = 0;
    ar->num_slots = INIT_SLOTS;
    ar->actions = (action_t *) ck_malloc(sizeof(action_t) * INIT_SLOTS, 
                                       fn_name);
    return ar;
}

/* 
 * free_action_report: frees up the space associated with a action
 * report.
 *
 * ar: the action report
 */
void free_action_report(action_report_t *ar) {
    assert(ar != NULL);

    ck_free(ar->ticker);
    ck_free(ar->actions);
    ck_free(ar);
}

/*
 * add_action: add an action to the report
 *
 * ar: the action report
 * oref: the order's oref
 * price; the price of the action
 * num_shares: the number of shares actions
 */
void add_action(action_report_t *ar, enum action action,
		long long oref, long long price, int num_shares) {
    assert(ar != NULL);
    assert(num_shares > 0);

    int nt = ar->num_actions;
    int ns = ar->num_slots;

    if (nt == ns) {
        // out of room
        int new_num_slots = (int) (ns * SLOTS_MULTIPLIER);
        ar->actions = (action_t *) ck_realloc(ar->actions, 
					      sizeof(action_t) * new_num_slots, 
					      "add_action");
        ar->num_slots = new_num_slots;
        ns = new_num_slots;
    }

    // now we are sure there is a free slot
    assert(nt < ns);
    ar->actions[nt].action = action;
    ar->actions[nt].oref = oref;
    ar->actions[nt].price = price;
    ar->actions[nt].shares = num_shares;
    ar->num_actions++;
}

/*
 * print_action_report: print the contents of the action report
 *
 * ar: the action report
 */
void print_action_report(action_report_t *ar) {
    assert(ar != NULL);

    int nt = ar->num_actions;
    if (nt == 0) {
        printf("No actions recorded for %s\n", ar->ticker);
        return;
    }

    for (int i = 0; i < nt; i++) {
        char *act;
        if (ar->actions[i].action == BOOKED_BUY) {
  	    act = "BOOKED (BUY)";
	} else if (ar->actions[i].action == BOOKED_SELL) {
  	    act = "BOOKED (SELL)";
	} else {
	    act = "TRADED";
	}

        printf("  %d shares %s at price %lld (%lld)\n",
               ar->actions[i].shares,
	       act,
               ar->actions[i].price,
               ar->actions[i].oref);	   
    }
}


