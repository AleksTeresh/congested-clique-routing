#include "node.h"
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
        check_precondition_for_step_3(nodes);

        for (auto node : nodes) {
            node->clear_neighbour_mcs();
        }

        for (auto node : nodes) {
            node->step3_round1();
        }
        for (auto node : nodes) {
            node->step3_round2();
        }
        for (auto node : nodes) {
            node->step3_round3();
        }
        for (auto node : nodes) {
            node->step3_round4();
        }
    }

    void move_messages_between_sets(vector<Node*>& nodes) {
        check_precondition_for_step_4(nodes);
        for (auto node : nodes) {
            node->send_cross_set();
        }
    }

    void move_messages_within_sets(vector<Node*>& nodes) {
        check_precondition_for_step_5(nodes);
        for (auto node : nodes) {
            node->clear_neighbour_mcs();
        }
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

    void check_precondition_for_step_3(vector<Node*>& nodes) {
        vector<vector<int>> message_counts(SET_SIZE, vector<int>(SET_SIZE, 0));

        for (auto node : nodes) {
            int src_set_idx = node->get_set_idx();
            for (auto message : node->get_messages()) {
                int dest_set_idx = Node::get_set_from_node_id(nodes, message->dest);
                message_counts[src_set_idx][dest_set_idx]++;
            }
        }

        for (int i = 0; i < SET_SIZE; i++) {
            for (int j = 0; j < SET_SIZE; j++) {
                assert(message_counts[i][j] == SET_SIZE * SET_SIZE);
            }
        }
    }

    void check_precondition_for_step_4(vector<Node*>& nodes) {
        for (auto node : nodes) {
            vector<int> message_per_set_count(SET_SIZE, 0);
            for (auto message : node->get_messages()) {
                int dest_set_idx = Node::get_set_from_node_id(nodes, message->dest);
                message_per_set_count[dest_set_idx]++;
            }

            for (int count : message_per_set_count) {
                assert(count == SET_SIZE);
            }
        }
    }

    void check_precondition_for_step_5(vector<Node*>& nodes) {
        for (auto node : nodes) {
            int set_idx = node->get_set_idx();
            for (auto message : node->get_messages()) {
                int dest_set_idx = Node::get_set_from_node_id(nodes, message->dest);
                assert(dest_set_idx == set_idx);
            }
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