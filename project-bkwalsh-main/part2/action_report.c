/*
 * CS 152, Spring 2022
 * Trade Report Data Structure
 * 
 * Do NOT modify this file
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include "action_report.h"
#include "util.h"

// The type for representing trades in the report
typedef struct action_s {
    enum action action;
    long long oref;
    long long price;
    int shares;
} action_t;

// structure for represeting a trade report
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
	switch (ar->actions[i].action) {
	case BOOKED_BUY:
  	    act = "BOOKED (BUY)";
	    break;
	case BOOKED_SELL:	    	
  	    act = "BOOKED (SELL)";
	    break;
	case EXECUTE:
	    act = "TRADED";
	    break;
	case CANCEL_BUY:
	    act = "CANCELED (BUY)";
	    break;
	case CANCEL_SELL:
	    act = "CANCELED (SELL)";
	    break;
	default:
	  fprintf(stderr, "Should not get here: %d  %d\n", i, ar->actions[i].action);
	    exit(0);

	}
	
        printf("  %d shares %s at price %lld (%lld)\n",
               ar->actions[i].shares,
	       act,
               ar->actions[i].price,
               ar->actions[i].oref);	   
    }
}


/* 
 * write_action_report_to_file: write the action report to a file, one action
 *   per line.
 *
 * ar: the action report
 * fp: a file pointer
 * index: a prefix to add to every line to identify action report
 */
void write_action_report_to_file(action_report_t *ar, FILE *fp, int index) {
    assert(ar != NULL);

    if (ar->num_actions == 0) {
        return;
    }

    for (int i = 0; i < ar->num_actions; i++) {
        char *act;
	switch (ar->actions[i].action) {
	case BOOKED_BUY:
  	    act = "BOOKED_BUY";
	    break;
	case BOOKED_SELL:	    	
  	    act = "BOOKED_SELL";
	    break;
	case EXECUTE:
	    act = "EXECUTE";
	    break;
	case CANCEL_BUY:
	    act = "CANCEL_BUY";
	    break;
	case CANCEL_SELL:
	    act = "CANCEL_SELL";
	    break;
	default:
	    fprintf(stderr, "Should not get here: %d  %d\n", i, ar->actions[i].action);
  	    exit(0);
	}
	
        fprintf(fp,"%d,%s,%lld,%lld,%d\n",
		index,
		act,
		ar->actions[i].oref,
		ar->actions[i].price,
		ar->actions[i].shares);
    }
}
