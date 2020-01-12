#include "clique-router.cpp"

int test_step_4_and_5() {
    vector<Node*> nodes;

    auto n1 = new Node(0);
    vector<int> v = {1, 1, 3, 3};
    n1->add_messages(v);
    nodes.push_back(n1);

    auto n2 = new Node(1);
    v = {0, 0, 2, 3};
    n2->add_messages(v);
    nodes.push_back(n2);

    auto n3 = new Node(2);
    v = {2, 3, 0, 1};
    n3->add_messages(v);
    nodes.push_back(n3);

    auto n4 = new Node(3);
    v = {2, 2, 1, 0};
    n4->add_messages(v);
    nodes.push_back(n4);

    CliqueRouter cr;
    cr.route(nodes);

    for (auto node : nodes) {
        int global_id = node->get_node_idx();
        auto messages = node->get_messages();

        for (auto m : messages) {
            assert(m->dest == global_id);
        }
    }

    return 0;
}

int main() {
    test_step_4_and_5();
}