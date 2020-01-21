#ifndef CONGESTED_CLIQUE_ROUTING_NODE_H
#define CONGESTED_CLIQUE_ROUTING_NODE_H

#include <vector>
#include <cmath>

struct Message {
    int dest;
    int src;

    int step_to_be_sent = 0;
    int next_dest = -1;
    int next_set = -1;

    Message(int src, int dest) {
        this->src = src;
        this->dest = dest;
    }
};

struct MessageCount {
    int msg_src;
    int msg_dest;
    int msg_count = 0;
    int info_dest;

    MessageCount(int msg_src, int msg_dest, int msg_count, int info_dest) {
        this->msg_src = msg_src;
        this->msg_dest = msg_dest;
        this->msg_count = msg_count;
        this->info_dest = info_dest;
    }
};

class Node {
private:
    int global_idx;
    std::vector<Message*> messages;
    std::vector<MessageCount*> neighbour_message_count;
    std::vector<Node*> nodes;
    std::vector<std::vector<std::vector<int>>> step2_coloring;

    int message_sent_count = 0;
    int set_size = 0;

    void start_message_count();

    void check_message_count();

    void check_message_count(int max_size);

    int get_node_idx_in_set(int node_id);

    int get_nth_node_in_set(int set_idx, int node_idx);

    void send_message(Message* message, int intermediate_dest, int step_to_be_sent);

    void send_neighbour_message_count(MessageCount* mc, int intermediate_dest);

    Message* get_message(std::function<bool(Message*)> prerequisite);

    std::vector<Message*>::iterator get_message_position(const std::function<bool(Message*)> prerequisite);

    void add_missing_edges(std::vector<std::vector<int>>& all_messages, int degree);

    Message* get_message_by_dest_set(int dest);
    Message* get_message_by_next_set(int next_set_idx);

    std::vector<std::vector<std::vector<int>>> get_graph_coloring(std::vector<std::vector<int>> all_messages);
    std::vector<std::vector<std::vector<int>>> get_graph_coloring(std::vector<std::vector<int>> all_messages, int degree);

    MessageCount* corollary34_create_message_count(
            std::vector<int>& message_counts,
            int dest_id, // the MessageCount object will be sent to this node
            int about_node_id // the MessageCount object will have info about number of messages to be sent from global_idx to about_node_id
    );

    void corollary34_round1(std::vector<int>& message_counts, const std::function<int(int)>& dest_from_inset_node_idx);

    void corollary_34_round2();

    void corollary_34_round3(std::vector<std::vector<int>>& edge_counts, int current_algo_step);

    void corollary_34_round4(int current_algo_step);

    bool node_has_extra_messages_for_set(
            std::vector<std::vector<int>>& message_counts,
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

    std::vector<std::vector<int>> step3_round3_create_graph(
            std::vector<std::vector<int>>& message_count_per_src_node
    );

    void send_message_to_color(int color, int dest_of_message, int curr_algo_step);

    void send_message_to_itself(int dest_of_message, int curr_algo_step);

public:
    explicit Node(int global_idx) {
        this->global_idx = global_idx;
    }

    std::vector<Message*>& get_messages();

    std::vector<MessageCount*>& get_message_counts();

    static int get_set_from_node_id(std::vector<Node*>& nodes, int node_id) {
        int node_count = nodes.size();
        int set_count = sqrt(node_count);
        return node_id / set_count;
    }


    void add_messages(std::vector<int>& new_messages);

    void add_message(int message_dest);

    void add_message(Message* m);

    int get_node_idx();

    int get_set_idx();

    void add_neighbour_message_count(MessageCount* mc);

    void init(std::vector<Node*>& nodes_to_init);

    void step2_round1();
    void step2_round2();
    void step2_round3();
    void step2_round4();
    void step2_round5();
    void step2_round6();
    void step2_round7();
    void step2_round8();

    void step3_round1();
    void step3_round2();
    void step3_round3();
    void step3_round4();

    void clear_neighbour_mcs();
    void reset_message_next_dest();
    void prepare_message_for_final_transfer();

    void send_cross_set();

    void send_within_set_round1();
    void send_within_set_round2();
    void send_within_set_round3();
    void send_within_set_round4();
};

#endif //CONGESTED_CLIQUE_ROUTING_NODE_H
