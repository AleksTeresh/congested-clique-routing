#include "clique-router.cpp"

int test1() {
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

int test2() {
    vector<Node*> nodes;

    auto n1 = new Node(0);
    vector<int> v = {1, 2, 3, 0};
    n1->add_messages(v);
    nodes.push_back(n1);

    auto n2 = new Node(1);
    v = {0, 1, 2, 3};
    n2->add_messages(v);
    nodes.push_back(n2);

    auto n3 = new Node(2);
    v = {2, 3, 0, 1};
    n3->add_messages(v);
    nodes.push_back(n3);

    auto n4 = new Node(3);
    v = {3, 2, 1, 0};
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

int test3() {
    vector<Node*> nodes;

    auto n1 = new Node(0);
    vector<int> v = {1, 2, 3, 3};
    n1->add_messages(v);
    nodes.push_back(n1);

    auto n2 = new Node(1);
    v = {0, 0, 1, 3};
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

int test4() {
    vector<Node*> nodes;

    auto n1 = new Node(0);
    vector<int> v = {2, 2, 3, 0};
    n1->add_messages(v);
    nodes.push_back(n1);

    auto n2 = new Node(1);
    v = {1, 1, 1, 3};
    n2->add_messages(v);
    nodes.push_back(n2);

    auto n3 = new Node(2);
    v = {1, 3, 3, 2};
    n3->add_messages(v);
    nodes.push_back(n3);

    auto n4 = new Node(3);
    v = {0, 2, 0, 0};
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
    test1();
    test2();
    test3();
    test4();
}