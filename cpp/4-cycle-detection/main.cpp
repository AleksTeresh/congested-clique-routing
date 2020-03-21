#include "node.h"

using namespace std;
using namespace four_cycle_detection;

int main() {
    auto n0 = make_shared<Node>(0, Vec<int>({1, 3, 5}));
    auto n1 = make_shared<Node>(1, Vec<int>({0, 2, 3}));
    auto n2 = make_shared<Node>(2, Vec<int>({1, 3, 4}));
    auto n3 = make_shared<Node>(3, Vec<int>({0, 1, 2, 4, 5}));
    auto n4 = make_shared<Node>(4, Vec<int>({2, 3, 5}));
    auto n5 = make_shared<Node>(5, Vec<int>({0, 3, 4}));

    Vec<shared_ptr<Node>> nodes;

    nodes.push_back(n0);
    nodes.push_back(n1);
    nodes.push_back(n2);
    nodes.push_back(n3);
    nodes.push_back(n4);
    nodes.push_back(n5);

    for (auto& node : nodes) {
        node->set_nodes(nodes);
    }

    for (auto& node : nodes) {
        node->broadcast_degree();
    }

    for (auto& node : nodes) {
        node->messages_to_neighbours();
    }

    for (auto& node : nodes) {
        node->compute_tiles();
    }

    return 0;
}
