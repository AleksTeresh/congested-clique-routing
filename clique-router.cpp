#include "node.h"

using namespace std;

class CliqueRouter {
private:
    int set_size = 0;

    void init(Vec<Node*>& nodes) {
        set_size = sqrt(nodes.size());
        for (auto node : nodes) {
            node->init(nodes);
        }
    }

    void step2(Vec<Node*>& nodes) {
        check_step_2_precondition(nodes);

        for (auto node : nodes) {
            node->clear_neighbour_mcs();
            node->reset_message_next_dest();
        }

        for (auto node : nodes) {
            node->step2_round1();
        }
        for (auto node : nodes) {
            node->step2_round2();
        }
        for (auto node : nodes) {
            node->step2_round3();
        }
        for (auto node : nodes) {
            node->step2_round4();
        }
        for (auto node : nodes) {
            node->step2_round5();
        }
        for (auto node : nodes) {
            node->step2_round6();
        }
        for (auto node : nodes) {
            node->step2_round7();
        }
        for (auto node : nodes) {
            node->step2_round8();
        }
    }

    void step3(Vec<Node*>& nodes) {
        check_step_3_precondition(nodes);

        for (auto node : nodes) {
            node->clear_neighbour_mcs();
            node->reset_message_next_dest();
        }

        for (auto node : nodes) {
            node->step3_round1();
        }
        for (auto node : nodes) {
            node->step3_round2();
        }
        check_step3_round2_result(nodes);
        for (auto node : nodes) {
            node->step3_round3();
        }
        for (auto node : nodes) {
            node->step3_round4();
        }
    }

    void check_step3_round2_result(Vec<Node*>& nodes) {
        for (auto node : nodes) {
            Vec2<int> mcounts(set_size, Vec<int>(set_size, 0));
            auto mcs = node->get_message_counts();
            assert(mcs.size() == set_size * set_size);

            for (auto mc : mcs) {
                assert(mc->info_dest == node->get_node_idx()); // the message count is at the destination
                mcounts[mc->msg_src - node->get_set_idx() * set_size][mc->msg_dest]++;
            }

            for (int i = 0; i < set_size; i++) {
                for (int j = 0; j < set_size; j++) {
                    assert(mcounts[i][j] == 1); // one message count from each node in W about each set W'
                }
            }
        }
    }

    void move_messages_between_sets(Vec<Node*>& nodes) {
        check_step_4_precondition(nodes);
        for (auto node : nodes) {
            node->send_cross_set();
        }
    }

    void move_messages_within_sets(Vec<Node*>& nodes) {
        check_step_5_precondition(nodes);
        for (auto node : nodes) {
            node->clear_neighbour_mcs();
            node->prepare_message_for_final_transfer();
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

    void check_step_2_precondition(Vec<Node*>& nodes) {
        Vec<int> sent(set_size * set_size, 0);
        Vec<int> received(set_size * set_size, 0);

        for (auto node : nodes) {
            int src_idx = node->get_node_idx();
            for (auto message : node->get_messages()) {
                int dest_idx = message->final_dest;
                sent[src_idx]++;
                received[dest_idx]++;
            }
        }

        for (int i = 0; i < set_size * set_size; i++) {
            assert(sent[i] == set_size * set_size);
            assert(received[i] == set_size * set_size);
        }
    }

    void check_step_3_precondition(Vec<Node*>& nodes) {
        Vec2<int> message_counts(set_size, Vec<int>(set_size, 0));

        for (auto node : nodes) {
            int src_set_idx = node->get_set_idx();
            for (auto message : node->get_messages()) {
                int dest_set_idx = Node::get_set_from_node_id(nodes, message->final_dest);
                message_counts[src_set_idx][dest_set_idx]++;
            }
        }

        for (int i = 0; i < set_size; i++) {
            for (int j = 0; j < set_size; j++) {
                assert(message_counts[i][j] == set_size * set_size);
            }
        }
    }

    void check_step_4_precondition(Vec<Node*>& nodes) {
        for (auto node : nodes) {
            Vec<int> message_per_set_count(set_size, 0);
            for (auto message : node->get_messages()) {
                int dest_set_idx = Node::get_set_from_node_id(nodes, message->final_dest);
                message_per_set_count[dest_set_idx]++;
            }

            for (int count : message_per_set_count) {
                assert(count == set_size);
            }
        }
    }

    void check_step_5_precondition(Vec<Node*>& nodes) {
        for (auto node : nodes) {
            int set_idx = node->get_set_idx();
            for (auto message : node->get_messages()) {
                int dest_set_idx = Node::get_set_from_node_id(nodes, message->final_dest);
                assert(dest_set_idx == set_idx);
            }
        }
    }

public:
    void route(Vec<Node*>& nodes) {
        init(nodes);
        step2(nodes);
        step3(nodes);
        move_messages_between_sets(nodes);
        move_messages_within_sets(nodes);
    }
};