#ifndef CONGESTED_CLIQUE_ROUTING_COLOR_H
#define CONGESTED_CLIQUE_ROUTING_COLOR_H

#include "helper-data-structs.h"

using namespace std;

bool bpm(Vec2<int>& bpGraph, int u, Vec<bool>& seen, Vec<int>& matchR);
Vec<int> reverse(Vec<int> matchR);
Vec<int> max_BPM(Vec2<int>& bp_graph);

#endif //CONGESTED_CLIQUE_ROUTING_NODE_H
