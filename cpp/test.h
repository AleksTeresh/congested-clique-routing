#ifndef CONGESTED_CLIQUE_ROUTING_TEST_H
#define CONGESTED_CLIQUE_ROUTING_TEST_H

#include "node.h"
#include "time-point.h"

void check_arrived_messages(Vec<std::shared_ptr<Node>>& nodes);

int test1();
int test2();
int test3();
int test4();
int test5();
int test6();
int test7();

std::vector<TimePoint> random_test(int subset_size);

int run_tests ();

#endif //CONGESTED_CLIQUE_ROUTING_TEST_H
