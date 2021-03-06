#ifndef CONGESTED_CLIQUE_ROUTING_NODE_H
#define CONGESTED_CLIQUE_ROUTING_NODE_H

#include "helper-data-structs.h"
#include <vector>
#include <functional>
#include <memory>
#include <cmath>

class Node {
private:
    int global_idx;
    Vec<std::unique_ptr<Message>> messages;
    Vec<std::unique_ptr<MessageCount>> received_message_counts;
    Vec<std::unique_ptr<MessageCount>> algo2_step1_message_counts;
    Vec<std::weak_ptr<Node>> nodes; // list of all nodes in the clique
    Vec3<int> step2_coloring;

    int message_sent_count = 0; // number of messages sent during a single round
    int set_size = 0; // size of set W

    void start_message_count();

    void check_message_count();

    void check_message_count(int max_size);

    int get_local_id_from_global(int node_id);

    int get_global_id_from_local(int set_idx, int node_idx);

    void send_message(std::unique_ptr<Message> message, int intermediate_dest, int step_to_be_sent);

    void send_message_counts_algo2_step1(std::unique_ptr<MessageCount> mc, int intermediate_dest);

    void send_message_counts(std::unique_ptr<MessageCount> mc, int intermediate_dest);

    Message& get_message(const std::function<bool(Message&)>& prerequisite);

    Vec<std::unique_ptr<Message>>::iterator get_message_pos(const std::function<bool(Message&)>& prerequisite);

    void add_missing_edges(Vec2<int>& all_messages, int degree);

    Message& get_message_by_dest_set(int dest);
    Message& get_message_by_next_set(int next_set_idx);

    Vec3<int> get_graph_coloring(Vec2<int>& all_messages);
    Vec3<int> get_graph_coloring(Vec2<int> all_messages, int degree);

    std::unique_ptr<MessageCount> corollary34_create_message_count(
            Vec<int>& message_counts,
            int dest_id, // the MessageCount object will be sent to this node
            int about_node_id // the MessageCount object will have info about number of messages to be sent from global_idx to about_node_id
    );

    void corollary34_round1(Vec<int>& message_counts, const std::function<int(int)>& dest_from_inset_node_idx);

    void corollary_34_round2();

    void corollary_34_round3(Vec2<int>& edge_counts, int current_algo_step);

    void corollary_34_round4(int current_algo_step);

    bool node_has_extra_messages_for_set(
            Vec2<int>& message_counts,
            int src_node_idx,
            int dest_node_idx
    );

    void set_next_dest_to_message(int message_dest_set_idx, int local_src_idx, int local_dest_idx);
    void set_next_set_to_message(int final_dest_set, int local_src_idx, int intermediate_dest_set);
    void set_next_dest_to_message_by_next_set(
            int dest_set_idx, // how to find
            int local_src_idx, // from where
            int local_dest_idx // next dest
    );

    void compute_set_to_set_graph();
    void broadcast_message_counts_locally();

    void update_counts(
            Vec2<int>& message_counts, // message counts from a node in W to a set W'
            Vec2<int>& edge_counts,
            int node_local_idx,
            int local_src_idx,
            int dest_set_idx
    );
    void find_missing_messages(
            Vec2<int>& message_counts, // message counts from a node in W to a set W'
            Vec2<int>& edge_counts,
            int node_local_idx,
            int dest_set_idx
    );
    Vec2<int> step3_round3_create_graph(
            Vec2<int>& message_count_per_src_node
    );

    void send_message_to_color(int color, int dest_of_message, int curr_algo_step);

    void send_message_to_itself(int dest_of_message, int curr_algo_step);

    Vec<int> count_messages_for_each_subset();
    void send_counts_for_ith_subset_locally(Vec<int> message_counts);

    Vec2<int> count_messages_inter_set();
    Vec2<int> build_inter_set_graph(Vec2<int> final_message_count);
    Vec2<int> build_inset_graph(Vec3<int> coloring);

    Vec<int> sum_up_message_counts();
    void broadcast_message_counts(Vec<int> message_counts);

public:
    explicit Node(int global_idx) {
        this->global_idx = global_idx;
    }

    static int get_set_from_node_id(Vec<std::weak_ptr<Node>>& nodes, int node_id) {
        return get_set_from_node_id(nodes.size(), node_id);
    }
    static int get_set_from_node_id(Vec<std::shared_ptr<Node>>& nodes, int node_id) {
        return get_set_from_node_id(nodes.size(), node_id);
    }
    static int get_set_from_node_id(int node_count, int node_id) {
        int set_count = sqrt(node_count);
        return node_id / set_count;
    }

    Vec<std::unique_ptr<Message>>& get_messages();
    Vec<std::unique_ptr<MessageCount>>& get_message_counts();
    Vec<std::unique_ptr<MessageCount>>& get_algo2_message_counts();

    void add_messages(Vec<int>& new_messages);
    void add_message(int message_dest);
    void add_message(std::unique_ptr<Message> m);
    int get_node_idx();
    int get_set_idx();
    void add_also2_step1_message_count(std::unique_ptr<MessageCount> mc);
    void add_received_message_count(std::unique_ptr<MessageCount> mc);
    void init(Vec<std::shared_ptr<Node>>& nodes_to_init);

    void step2_round1();
    void step2_round2();
    void step2_round3();
    void step2_round4();
    void step2_round5();
    void step2_round6();
    void step2_round7();

    void step3_round1();
    void step3_round2();
    void step3_round3();
    void step3_round4();

    void clear_received_mcs();
    void reset_message_next_dest();
    void prepare_message_for_final_transfer();

    void send_cross_set();

    void send_within_set_round1();
    void send_within_set_round2();
    void send_within_set_round3();
    void send_within_set_round4();
};

#endif //CONGESTED_CLIQUE_ROUTING_NODE_H
