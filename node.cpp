#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;

struct Message {
    int dest;
    int src;

    Message(int src, int dest) {
        this->src = src;
        this->dest = dest;
    }
};

class Node {
private:
    int global_idx;
    vector<Message*> messages;
    vector<Node*> nodes;
    // vector<vector<int>> src_dest_pairs;

    int get_set_from_node_id(int node_id) {
        int node_count = nodes.size();
        int set_count = sqrt(node_count);
        return node_id / set_count;
    }

    int get_nth_node_in_set(int set_idx, int node_idx) {
        return set_idx * sqrt(nodes.size()) + node_idx;
    }

    void send_message(Message* message, int intermediate_dest) {
        nodes[intermediate_dest]->add_message(message);
    }

    void announce_messages_in_w() {

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
        messages.push_back(m);
    }

    int get_node_idx() {
        return this->global_idx;
    }

    int get_set_idx() {
        return get_set_from_node_id(this->global_idx);
    }

    void init(vector<Node*>& nodes_to_init) {
        nodes = vector<Node*>();
        for (auto node : nodes_to_init) {
            nodes.push_back(node);
        }
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
            int dest_set_idx = get_set_from_node_id((*i)->dest);
            if (dest_set_idx != src_set_idx) {
                int intermediate_dest = get_nth_node_in_set(
                        dest_set_idx,
                        last_idx_per_set[dest_set_idx]
                );

                last_idx_per_set[dest_set_idx]++;
                send_message((*i), intermediate_dest);
                i = messages.erase(i);
            } else {
                i++;
            }
        }
    }

    void send_within_set() {
        // TODO: Algorithm 1 step 5
    }
};