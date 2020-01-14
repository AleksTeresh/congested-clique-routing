#ifndef CONGESTED_CLIQUE_ROUTING_NODE_H
#define CONGESTED_CLIQUE_ROUTING_NODE_H

#include <vector>
#include <cmath>

// TODO: current assumption is that |W| is always 2
const int SET_SIZE = 2;

struct Message {
    int dest;
    int src;

    int step_to_be_sent = 0;
    int next_dest = -1;

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

    int message_sent_count = 0;

    void start_message_count();

    void check_message_count();

    void check_message_count(int max_size);

    int get_node_idx_in_set(int node_id);

    int get_nth_node_in_set(int set_idx, int node_idx);

    void send_message(Message* message, int intermediate_dest, int step_to_be_sent);

    void send_neighbour_message_count(MessageCount* mc, int intermediate_dest);

    Message* get_message_by_dest(int dest);

    Message* get_message_by_dest_set(int dest);
    // current assumption is that number of nodes in a set is always 2
    // Due to this, the coloring algorithm is trivial
    std::vector<std::vector<std::vector<int>>> get_graph_coloring(std::vector<std::vector<int>>& all_messages);

    void corollary34_step1(std::vector<int>& message_counts);

    void corollary_34_step2();


public:
    explicit Node(int global_idx) {
        this->global_idx = global_idx;
    }

    std::vector<Message*>& get_messages();

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

    void step2();

    void step3_round1();

    void step3_round2();

    void step3_round3();

    void step3_round4();

    void clear_neighbour_mcs();

    void send_cross_set();

    void send_within_set_round1();

    // step 2 of Corollary 3.4
    void send_within_set_round2();

    void send_within_set_round3();

    void send_within_set_round4();
};

#endif //CONGESTED_CLIQUE_ROUTING_NODE_H
