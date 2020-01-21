#include <random>
#include "node.cpp"
#include "clique-router.cpp"

void check_arrived_messages(vector<Node*>& nodes) {
    for (auto node : nodes) {
        int global_id = node->get_node_idx();
        auto messages = node->get_messages();

        for (auto m : messages) {
            assert(m->dest == global_id);
        }
    }
}

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

    check_arrived_messages(nodes);

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

    check_arrived_messages(nodes);

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

    check_arrived_messages(nodes);

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

    check_arrived_messages(nodes);

    return 0;
}

int test5() {
    vector<Node*> nodes;

    auto n1 = new Node(0);
    vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    n1->add_messages(v);
    nodes.push_back(n1);

    auto n2 = new Node(1);
    v = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    n2->add_messages(v);
    nodes.push_back(n2);

    auto n3 = new Node(2);
    v = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    n3->add_messages(v);
    nodes.push_back(n3);

    auto n4 = new Node(3);
    v = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    n4->add_messages(v);
    nodes.push_back(n4);

    auto n5 = new Node(4);
    v = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    n5->add_messages(v);
    nodes.push_back(n5);

    auto n6 = new Node(5);
    v = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    n6->add_messages(v);
    nodes.push_back(n6);

    auto n7 = new Node(6);
    v = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    n7->add_messages(v);
    nodes.push_back(n7);

    auto n8 = new Node(7);
    v = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    n8->add_messages(v);
    nodes.push_back(n8);

    auto n9 = new Node(8);
    v = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    n9->add_messages(v);
    nodes.push_back(n9);

    CliqueRouter cr;
    cr.route(nodes);

    check_arrived_messages(nodes);

    return 0;
}

int test6() {
    vector<Node*> nodes;

    auto n1 = new Node(0);
    vector<int> v = {1, 1, 2, 2, 4, 4, 8, 8, 8};
    n1->add_messages(v);
    nodes.push_back(n1);

    auto n2 = new Node(1);
    v = {7, 7, 7, 5, 5, 4, 0, 0, 1};
    n2->add_messages(v);
    nodes.push_back(n2);

    auto n3 = new Node(2);
    v = {1, 1, 5, 5, 3, 3, 6, 6, 6};
    n3->add_messages(v);
    nodes.push_back(n3);

    auto n4 = new Node(3);
    v = {2, 2, 2, 2, 6, 6, 6, 6, 6};
    n4->add_messages(v);
    nodes.push_back(n4);

    auto n5 = new Node(4);
    v = {3, 3, 3, 3, 0, 0, 8, 8, 8};
    n5->add_messages(v);
    nodes.push_back(n5);

    auto n6 = new Node(5);
    v = {0, 0, 4, 4, 4, 4, 0, 5, 8};
    n6->add_messages(v);
    nodes.push_back(n6);

    auto n7 = new Node(6);
    v = {0, 1, 2, 3, 4, 5, 5, 3, 4};
    n7->add_messages(v);
    nodes.push_back(n7);

    auto n8 = new Node(7);
    v = {6, 7, 7, 0, 1, 3, 5, 7, 7};
    n8->add_messages(v);
    nodes.push_back(n8);

    auto n9 = new Node(8);
    v = {7, 7, 8, 8, 5, 1, 1, 2, 2};
    n9->add_messages(v);
    nodes.push_back(n9);

    CliqueRouter cr;
    cr.route(nodes);

    check_arrived_messages(nodes);

    return 0;
}

int test7() {
    vector<Node*> nodes;

    auto n1 = new Node(0);
    vector<int> v = {1, 1, 2, 6, 4, 4, 8, 8, 8};
    n1->add_messages(v);
    nodes.push_back(n1);

    auto n2 = new Node(1);
    v = {7, 7, 7, 5, 5, 4, 8, 0, 1};
    n2->add_messages(v);
    nodes.push_back(n2);

    auto n3 = new Node(2);
    v = {1, 1, 5, 5, 3, 3, 6, 6, 6};
    n3->add_messages(v);
    nodes.push_back(n3);

    auto n4 = new Node(3);
    v = {2, 2, 2, 2, 2, 6, 6, 6, 6};
    n4->add_messages(v);
    nodes.push_back(n4);

    auto n5 = new Node(4);
    v = {3, 3, 3, 3, 0, 0, 0, 8, 8};
    n5->add_messages(v);
    nodes.push_back(n5);

    auto n6 = new Node(5);
    v = {0, 0, 4, 4, 4, 4, 0, 5, 8};
    n6->add_messages(v);
    nodes.push_back(n6);

    auto n7 = new Node(6);
    v = {0, 1, 2, 3, 4, 5, 5, 3, 4};
    n7->add_messages(v);
    nodes.push_back(n7);

    auto n8 = new Node(7);
    v = {6, 7, 7, 0, 1, 3, 5, 7, 7};
    n8->add_messages(v);
    nodes.push_back(n8);

    auto n9 = new Node(8);
    v = {7, 7, 8, 8, 5, 1, 1, 2, 2};
    n9->add_messages(v);
    nodes.push_back(n9);

    CliqueRouter cr;
    cr.route(nodes);

    check_arrived_messages(nodes);

    return 0;
}

void random_test(int subset_size) {
    int set_size = subset_size * subset_size;
    vector<vector<int>> message_destinations(set_size, vector<int>());

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(0,set_size - 1); // distribution in range [0, set_size - 1]

    for (int i = 0; i < set_size; i++) {
        for (int dest = 0; dest < set_size; dest++) {
            int node_idx = dist(rng);
            while (message_destinations[node_idx].size() >= set_size) {
                node_idx = dist(rng);
            }
            message_destinations[node_idx].push_back(dest);
        }
    }

    vector<Node*> nodes;
    for (int i = 0; i < set_size; i++) {
        auto n = new Node(i);
        n->add_messages(message_destinations[i]);
        nodes.push_back(n);
    }

    CliqueRouter cr;
    cr.route(nodes);

    check_arrived_messages(nodes);
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();

    setbuf(stdout, nullptr);
    int test_count_per_size = 4;
    int max_size = 11;
    for (int i = 1; i <= max_size; i++) { // subset size
        printf("Started %d tests with %d nodes\n", test_count_per_size, i*i);
        for (int j = 0; j < test_count_per_size; j++) {
            random_test(i);
        }
    }

    return 0;
}