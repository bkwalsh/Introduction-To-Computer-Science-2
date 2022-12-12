CS 152, Spring 2022
Stock Exchange Project - Part 2
Test Files

This directory contains the orders, times, and expected results for
the autograder tests for Part 1.

Each test has three files (TN is the test number).

     testTN_orders.csv: -- the order strings for the test, one order string per line.

     testTN_times.csv -- the times used for the orders.  With the
       exception of test 7, the times are in increasing order.  Test 7
       contains a delayed order.  It should not be used to test
       simulate.c, since we specified increasing time for
       simulate.c.  You are not expected to use this file. It is
       included only for informational purposes.

       The first line in the file is the number of times.  The rest of
       the lines are the actual times used.

      testTN_actions_expected.csv -- the expected action report results
       for the orders.  Each line contains an action. The first value
       in each line is the line number of the corresponding order. For
       example, this action from test0_actions_expected.csv
       corresponds to the first order (order 0) in test0_orders.csv.

           0,BOOKED_SELL,4000,550000,100

Test 0 through 11 are the tests we used for the autograder.  Test 12
is the example from the writeup from Part 2.

       



