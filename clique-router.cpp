#include "node.cpp"
#include <vector>

using namespace std;

class CliqueRouter {
private:
    void init(vector<Node*>& nodes) {
        for (auto node : nodes) {
            node->init(nodes);
        }
    }

    void move_messages1(vector<Node*>& nodes) {
        for (auto node : nodes) {
            node->step2();
        }
    }

    void move_messages2(vector<Node*>& nodes) {
        for (auto node : nodes) {
            node->step3();
        }
    }

    void move_messages_between_sets(vector<Node*>& nodes) {
        for (auto node : nodes) {
            node->send_cross_set();
        }
    }

    void move_messages_within_sets(vector<Node*>& nodes) {
        for (auto node : nodes) {
            node->send_within_set_round1();
        }
        for (auto node : nodes) {
            node->send_within_set_round2();
        }
        for (auto node : nodes) {
            node->send_within_set_round3();
        }
        for (auto node : nodes) {
            node->send_within_set_round4();
        }
    }

public:
    void route(vector<Node*>& nodes) {
        init(nodes);
        move_messages1(nodes);
        move_messages2(nodes);
        move_messages_between_sets(nodes);
        move_messages_within_sets(nodes);
    }
};