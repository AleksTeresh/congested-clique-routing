#ifndef CONGESTED_CLIQUE_ROUTING_TEST_H
#define CONGESTED_CLIQUE_ROUTING_TEST_H

#include "node.h"
#include "time-point.h"

void check_arrived_messages(Vec<std::shared_ptr<Node>>& nodes);

std::vector<TimePoint> test1();
std::vector<TimePoint> test2();
std::vector<TimePoint> test3();
std::vector<TimePoint> test4();
std::vector<TimePoint> test5();
std::vector<TimePoint> test6();
std::vector<TimePoint> test7();
std::vector<TimePoint> uniform_subset_test(int n);

std::vector<TimePoint> random_test(int subset_size);

int run_tests ();

#endif //CONGESTED_CLIQUE_ROUTING_TEST_H
