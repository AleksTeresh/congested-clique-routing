#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;

// TODO: current assumption is that |W| is always 2
const int SET_SIZE = 2;

struct Message {
    int dest;
    int src;

    Message(int src, int dest) {
        this->src = src;
        this->dest = dest;
    }
};

struct MessageCount {
    int msg_src;
    int msg_dest;
    int msg_count;
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
    vector<Message*> messages;
    vector<MessageCount*> neighbour_message_count;
    vector<Node*> nodes;

    int get_set_from_node_id(int node_id) {
        int node_count = nodes.size();
        int set_count = sqrt(node_count);
        return node_id / set_count;
    }

    int get_node_idx_in_set(int node_id) {
        int node_count = nodes.size();
        int set_count = sqrt(node_count);
        return node_id % set_count;
    }

    int get_nth_node_in_set(int set_idx, int node_idx) {
        return set_idx * sqrt(nodes.size()) + node_idx;
    }

    void send_message(Message* message, int intermediate_dest) {
        nodes[intermediate_dest]->add_message(message);
    }

    Message* get_message_by_dest(int dest) {
        for (auto m : messages) {
            if (m->dest == dest) {
                return m;
            }
        }
    }

    // current assumption is that number of nodes in a set is always 2
    // Due to this, the coloring algorithm is trivial
    vector<vector<vector<int>>> get_graph_coloring(vector<vector<int>>& all_messages) {
        vector<vector<vector<int>>> colors(SET_SIZE, vector<vector<int>>(SET_SIZE, vector<int>()));

        int set_idx = get_set_idx();

        // color the graph
        for (int src_i = 0; src_i < SET_SIZE; src_i++) {
            int c = 0;
            for (int dest_i = 0; dest_i < SET_SIZE; dest_i++) {
                int message_count = all_messages[src_i][dest_i];
                for (int message_i = 0; message_i < message_count; message_i++) {
                    colors[src_i][dest_i].push_back(c);
                    c++;
                }
            }
        }

        return colors;
    }

public:
    explicit Node(int global_idx) {
        this->global_idx = global_idx;
    }

    void add_messages(vector<int>& new_messages) {
        for (int message : new_messages) {
            add_message(message);
        }
    }

    void add_message(int message_dest) {
        auto m = new Message(get_node_idx(), message_dest);
        messages.push_back(m);
    }

    void add_message(Message* m) {
        m->src = get_node_idx();
        messages.push_back(m);
    }

    int get_node_idx() {
        return this->global_idx;
    }

    int get_set_idx() {
        return get_set_from_node_id(this->global_idx);
    }

    void announce_neighbour_message_count(MessageCount* mc) {
        this->neighbour_message_count.push_back(mc);
    }

    void init(vector<Node*>& nodes_to_init) {
        nodes = vector<Node*>();
        for (auto node : nodes_to_init) {
            nodes.push_back(node);
        }

        neighbour_message_count = vector<MessageCount*>();
    }

    void step2() {
        // TODO: Algorithm 2
    }

    void step3() {
        // TODO: Algorithm1 step 3
    }

    void send_cross_set() {
        int src_set_idx = get_set_idx();

        vector<int> last_idx_per_set;
        last_idx_per_set.resize(sqrt(nodes.size()));

        for (auto i = messages.begin(); i != messages.end();) {
            auto message = *i;
            int dest_set_idx = get_set_from_node_id(message->dest);
            if (dest_set_idx != src_set_idx) {
                int intermediate_dest = get_nth_node_in_set(
                        dest_set_idx,
                        last_idx_per_set[dest_set_idx]
                );

                last_idx_per_set[dest_set_idx]++;
                send_message(message, intermediate_dest);
                i = messages.erase(i);
            } else {
                i++;
            }
        }
    }

    void send_within_set_round1() {
        vector<int> message_counts(nodes.size(), 0);
        // at this point all messages are destined within W only
        for (auto message : messages) {
            message_counts[message->dest]++;
        }

        vector<vector<int>> edge_counts(SET_SIZE, vector<int>(SET_SIZE, SET_SIZE));
        vector<vector<vector<int>>> coloring = get_graph_coloring(edge_counts);

        int src_idx_in_set = get_node_idx_in_set(global_idx);
        int set_id = get_set_idx();

        // step 1 of Corollary 3.4
        for (int dest_inset_i = 0; dest_inset_i < SET_SIZE; dest_inset_i++) { // destination node of the message
            for (int about_node_inset_i = 0; about_node_inset_i < SET_SIZE; about_node_inset_i++) { // the message contains info about this node
                int c = coloring[src_idx_in_set][dest_inset_i][about_node_inset_i]; // color of the edge corresponding to the message

                int global_dest_idx = get_nth_node_in_set(set_id, dest_inset_i);
                int global_about_node_idx = get_nth_node_in_set(set_id, about_node_inset_i);

                int message_count_to_destination = message_counts[global_about_node_idx];


                auto mc = new MessageCount(
                        global_idx,
                        global_about_node_idx,
                        message_count_to_destination,
                        global_dest_idx
                );
                nodes[c]->announce_neighbour_message_count(mc);
            }
        }
    }

    // step 2 of Corollary 3.4
    void send_within_set_round2() {
        for (auto i = neighbour_message_count.begin(); i != neighbour_message_count.end();) {
            auto mc = *i;
            if (mc->info_dest != global_idx) {
                nodes[mc->info_dest]->announce_neighbour_message_count(mc);
                i = neighbour_message_count.erase(i);
            } else {
                i++;
            }
        }
    }

    void send_within_set_round3() {
        vector<vector<int>> edge_counts(SET_SIZE, vector<int>(SET_SIZE, 0));

        for (auto mc : neighbour_message_count) {
            int src_local_idx = get_node_idx_in_set(mc->msg_src);
            int dest_local_idx = get_node_idx_in_set(mc->msg_dest);
            edge_counts[src_local_idx][dest_local_idx] = mc->msg_count;
        }

        vector<vector<vector<int>>> coloring = get_graph_coloring(edge_counts);

        int src_idx_in_set = get_node_idx_in_set(global_idx);
        int set_id = get_set_idx();

        for (int dest_inset_idx = 0; dest_inset_idx < SET_SIZE; dest_inset_idx++) { // destination node of the message
            int global_dest_idx = get_nth_node_in_set(set_id, dest_inset_idx);
            if (global_dest_idx == global_idx) continue; // skip sending messages to itself

            for (int c : coloring[src_idx_in_set][dest_inset_idx]) { // color of the edge corresponding to the message
                if (c == global_idx) continue; // skip sending messages to itself

                for (auto i = messages.begin(); i != messages.end();) {
                    auto message = *i;
                    if (message->dest == global_dest_idx) {
                        send_message(message, c);
                        i = messages.erase(i);
                        break;
                    } else {
                        i++;
                    }
                }
            }
        }
    }

    void send_within_set_round4() {
        for (auto i = messages.begin(); i != messages.end();) {
            auto message = *i;
            if (message->dest != global_idx) {
                send_message(message, message->dest);
                i = messages.erase(i);
            } else {
                i++;
            }
        }
    }
};