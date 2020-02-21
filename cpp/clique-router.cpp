#include "node.h"

using namespace std;

struct NodeData {
    int id;
    vector<Message> messages;
    vector<MessageCount> message_counts;

    NodeData() {}
    NodeData(int id, vector<unique_ptr<Message>>& messages, vector<unique_ptr<MessageCount>>& message_counts) {
        this->id = id;
        for (auto& m : messages) {
            this->messages.push_back(*m);
        }
        for (auto& mc : message_counts) {
            this->message_counts.push_back(*mc);
        }
    }
};

struct TimePoint {
    vector<NodeData> nodes;
};

class CliqueRouter {
private:
    int set_size = 0;
    vector<TimePoint> history;

    void init(Vec<shared_ptr<Node>>& nodes) {
        set_size = sqrt(nodes.size());
        for (auto& node : nodes) {
            node->init(nodes);
        }
    }

    void init_history(Vec<shared_ptr<Node>>& nodes) {
        history = {};
        update_history(nodes);
    }

    void update_history(Vec<shared_ptr<Node>>& nodes) {
        TimePoint tp;
        for (auto& node : nodes) {
            auto raw = node.get();
            NodeData nd(raw->get_node_idx(), raw->get_messages(), raw->get_message_counts());
            tp.nodes.push_back(nd);
        }
        history.push_back(tp);
    }

    // Algorithm 2
    // move all messages s.t. each subset W holds exactly set_size * set_size messages for each W'
    void step2(Vec<shared_ptr<Node>>& nodes) {
        check_step_2_precondition(nodes);

        for (auto& node : nodes) {
            node->clear_received_mcs();
            node->reset_message_next_dest();
        }

        for (auto& node : nodes) {
            node->step2_round1();
        }
        update_history(nodes);
        for (auto& node : nodes) {
            node->step2_round2();
        }
        update_history(nodes);
        for (auto& node : nodes) {
            node->step2_round3();
        }
        update_history(nodes);
        for (auto& node : nodes) {
            node->step2_round4();
        }
        update_history(nodes);
        for (auto& node : nodes) {
            node->step2_round5();
        }
        update_history(nodes);
        for (auto& node : nodes) {
            node->step2_round6();
        }
        update_history(nodes);
        for (auto& node : nodes) {
            node->step2_round7();
        }
        update_history(nodes);
    }

    // for each pair of subsets W, W' move messages destined
    // to W' within W s.t. each node holds exactly set_size messages
    // with destination in W'
    void step3(Vec<shared_ptr<Node>>& nodes) {
        check_step_3_precondition(nodes);

        for (auto& node : nodes) {
            node->clear_received_mcs();
            node->reset_message_next_dest();
        }

        for (auto& node : nodes) {
            node->step3_round1();
        }
        update_history(nodes);
        for (auto& node : nodes) {
            node->step3_round2();
        }
        update_history(nodes);
        check_step3_round2_result(nodes);
        for (auto& node : nodes) {
            node->step3_round3();
        }
        update_history(nodes);
        for (auto& node : nodes) {
            node->step3_round4();
        }
        update_history(nodes);
    }

    void check_step3_round2_result(Vec<shared_ptr<Node>>& nodes) {
        for (auto& node : nodes) {
            Vec2<int> mcounts(set_size, Vec<int>(set_size, 0));
            auto& mcs = node->get_message_counts();
            assert(mcs.size() == set_size * set_size);

            for (auto& mc : mcs) {
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

    // for each pair W and W'. Move all messages destined to nodes in W' from W to W'
    void move_messages_between_sets(Vec<shared_ptr<Node>>& nodes) {
        check_step_4_precondition(nodes);
        for (auto& node : nodes) {
            node->send_cross_set();
        }
        update_history(nodes);
    }

    // for each subset W move messages within it to their final destinations
    void move_messages_within_sets(Vec<shared_ptr<Node>>& nodes) {
        check_step_5_precondition(nodes);
        for (auto& node : nodes) {
            node->clear_received_mcs();
            node->prepare_message_for_final_transfer();
        }
        for (auto& node : nodes) {
            node->send_within_set_round1();
        }
        update_history(nodes);
        for (auto& node : nodes) {
            node->send_within_set_round2();
        }
        update_history(nodes);
        for (auto& node : nodes) {
            node->send_within_set_round3();
        }
        update_history(nodes);
        for (auto& node : nodes) {
            node->send_within_set_round4();
        }
        update_history(nodes);
    }

    void check_step_2_precondition(Vec<shared_ptr<Node>>& nodes) {
        Vec<int> sent(set_size * set_size, 0);
        Vec<int> received(set_size * set_size, 0);

        for (auto& node : nodes) {
            int src_idx = node->get_node_idx();
            for (auto& message : node->get_messages()) {
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

    void check_step_3_precondition(Vec<shared_ptr<Node>>& nodes) {
        Vec2<int> message_counts(set_size, Vec<int>(set_size, 0));

        for (auto& node : nodes) {
            int src_set_idx = node->get_set_idx();
            assert(node->get_messages().size() == set_size * set_size);
            for (auto& message : node->get_messages()) {
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

    void check_step_4_precondition(Vec<shared_ptr<Node>>& nodes) {
        for (auto& node : nodes) {
            Vec<int> message_per_set_count(set_size, 0);
            for (auto& message : node->get_messages()) {
                int dest_set_idx = Node::get_set_from_node_id(nodes, message->final_dest);
                message_per_set_count[dest_set_idx]++;
            }

            for (int count : message_per_set_count) {
                assert(count == set_size);
            }
        }
    }

    void check_step_5_precondition(Vec<shared_ptr<Node>>& nodes) {
        for (auto& node : nodes) {
            int set_idx = node->get_set_idx();
            for (auto& message : node->get_messages()) {
                int dest_set_idx = Node::get_set_from_node_id(nodes, message->final_dest);
                assert(dest_set_idx == set_idx);
            }
        }
    }

public:
    void route(Vec<shared_ptr<Node>>& nodes) {
        init(nodes);
        init_history(nodes);
        step2(nodes);
        step3(nodes);
        move_messages_between_sets(nodes);
        move_messages_within_sets(nodes);
    }

    vector<TimePoint>& get_history() {
        return history;
    }
};