#include <vector>
#include <unordered_map>
#include <cmath>
#include "node.h"

using namespace std;

void Node::start_message_count() {
    message_sent_count = 0;
}

void Node::check_message_count() {
    check_message_count(nodes.size() - 1);
}

void Node::check_message_count(int max_size) {
    // make sure a nodes does not send more than N-1 messages per round
    // (a nodes does not send a message to itself => N-1
    assert(message_sent_count <= max_size);
}

int Node::get_node_idx_in_set(int node_id) {
    int node_count = nodes.size();
    int set_count = sqrt(node_count);
    return node_id % set_count;
}

int Node::get_nth_node_in_set(int set_idx, int node_idx) {
    return set_idx * sqrt(nodes.size()) + node_idx;
}

void Node::send_message(Message* message, int intermediate_dest, int step_to_be_sent) {
    assert(message->src == global_idx); // a node can call send_message only on itself
    message->step_to_be_sent = step_to_be_sent;
    nodes[intermediate_dest]->add_message(message);
    message_sent_count++;
}

void Node::send_neighbour_message_count(MessageCount* mc, int intermediate_dest) {
    nodes[intermediate_dest]->add_neighbour_message_count(mc);
    message_sent_count++;
}

Message* Node::get_message(const function<bool(Message*)> prerequisite) {
    for (auto m : messages) {
        if (prerequisite(m)) {
            return m;
        }
    }

    return nullptr;
}

vector<Message*>::iterator Node::get_message_position(const function<bool(Message*)> prerequisite) {
    for (auto it = messages.begin(); it != messages.end();) {
        if (prerequisite(*it)) {
            return it;
        } else {
            it++;
        }
    }

    return messages.end();
};

Message* Node::get_message_by_dest_set(int dest) {
    return get_message([this, dest](auto m) {
        return m->next_dest == -1 &&
                this->get_set_from_node_id(nodes, m->dest) == dest;
    });
}

// current assumption is that number of nodes in a set is always 2
// Due to this, the coloring algorithm is trivial
vector<vector<vector<int>>> Node::get_graph_coloring(vector<vector<int>>& all_messages) {
    vector<vector<vector<int>>> colors(SET_SIZE, vector<vector<int>>(SET_SIZE, vector<int>()));

    // color the graph
    for (int src_i = 0; src_i < SET_SIZE; src_i++) {
        int c = src_i % 2 == 0 ? 0 : (nodes.size() - 1);
        for (int dest_i = 0; dest_i < SET_SIZE; dest_i++) {
            int message_count = all_messages[src_i][dest_i];
            for (int message_i = 0; message_i < message_count; message_i++) {
                colors[src_i][dest_i].push_back(c);
                c = src_i % 2 == 0 ? (c+1) : (c-1);
            }
        }
    }

    return colors;
}

void Node::corollary34_round1(vector<int>& message_counts, const function<int(int)>& dest_from_inset_node_idx) {
        vector<vector<int>> edge_counts(SET_SIZE, vector<int>(SET_SIZE, SET_SIZE));
        vector<vector<vector<int>>> coloring = get_graph_coloring(edge_counts);

        int src_idx_in_set = get_node_idx_in_set(global_idx);
        int set_id = get_set_idx();

        start_message_count();
        // step 1 of Corollary 3.4
        for (int dest_inset_i = 0; dest_inset_i < SET_SIZE; dest_inset_i++) { // destination node of the message
            for (int about_node_inset_i = 0; about_node_inset_i < SET_SIZE; about_node_inset_i++) { // the message contains info about this node
                int c = coloring[src_idx_in_set][dest_inset_i][about_node_inset_i]; // color of the edge corresponding to the message

                int global_dest_idx = get_nth_node_in_set(set_id, dest_inset_i);
                int dest_idx = dest_from_inset_node_idx(about_node_inset_i);// get_nth_node_in_set(set_id, about_node_inset_i);

                int message_count_to_destination = message_counts[dest_idx];


                auto mc = new MessageCount(
                        global_idx,
                        dest_idx,
                        message_count_to_destination,
                        global_dest_idx
                );
                send_neighbour_message_count(mc, c);
            }
        }
        check_message_count(nodes.size());
};

void Node::corollary_34_round2() {
    start_message_count();
    for (auto i = neighbour_message_count.begin(); i != neighbour_message_count.end();) {
        auto mc = *i;
        if (mc->info_dest != global_idx) {
            send_neighbour_message_count(mc, mc->info_dest);
            i = neighbour_message_count.erase(i);
        } else {
            i++;
        }
    }
    check_message_count();
}

void Node::send_message_to_color(int color, int dest_of_message, int curr_algo_step) {
    auto pos = get_message_position(
            [dest_of_message, curr_algo_step]
            (Message* m) {
                return m->next_dest == dest_of_message &&
                       m->step_to_be_sent != curr_algo_step;
            });

    if (pos != messages.end()) {
        auto message = *pos;
        send_message(message, color, curr_algo_step);
        pos = messages.erase(pos);
    }
}

void Node::send_message_to_itself(int dest_of_message, int curr_algo_step) {
    auto message = get_message(
            [dest_of_message, curr_algo_step]
            (Message* m) {
             return m->next_dest == dest_of_message &&
                    m->step_to_be_sent != curr_algo_step;
            });

    if (message != nullptr) {
        message->step_to_be_sent = curr_algo_step;
    }
}

void Node::corollary_34_round3(vector<vector<int>>& edge_counts, int curr_algo_step) {
    vector<vector<vector<int>>> coloring = get_graph_coloring(edge_counts);

    int src_idx_in_set = get_node_idx_in_set(global_idx);
    int set_id = get_set_idx();

    start_message_count();
    for (int dest_inset_idx = 0; dest_inset_idx < SET_SIZE; dest_inset_idx++) { // destination node of the message
        int global_dest_idx = get_nth_node_in_set(set_id, dest_inset_idx);
        if (global_dest_idx == global_idx) continue; // skip sending messages to itself

        for (int c : coloring[src_idx_in_set][dest_inset_idx]) { // color of the edge corresponding to the message
            if (c == global_idx) { // skip sending messages to itself
                send_message_to_itself(global_dest_idx, curr_algo_step);
            } else {
                send_message_to_color(c, global_dest_idx, curr_algo_step);
            }
        }
    }
    check_message_count();
}

void Node::corollary_34_round4(int current_algo_step) {
    start_message_count();
    for (auto i = messages.begin(); i != messages.end();) {
        auto message = *i;
        if (message->next_dest != global_idx &&
            message->step_to_be_sent == current_algo_step
        ) {
            send_message(message, message->next_dest, 0);
            i = messages.erase(i);
        } else {
            i++;
        }
    }
    check_message_count();
}

vector<Message*>& Node::get_messages() {
    return messages;
}

void Node::add_messages(vector<int>& new_messages) {
    for (int message : new_messages) {
        add_message(message);
    }
}

void Node::add_message(int message_dest) {
    auto m = new Message(get_node_idx(), message_dest);
    messages.push_back(m);
}

void Node::add_message(Message* m) {
    m->src = get_node_idx();
    messages.push_back(m);
}

int Node::get_node_idx() {
    return this->global_idx;
}

int Node::get_set_idx() {
    return get_set_from_node_id(nodes, this->global_idx);
}

void Node::add_neighbour_message_count(MessageCount* mc) {
    this->neighbour_message_count.push_back(mc);
}

void Node::init(vector<Node*>& nodes_to_init) {
    nodes = vector<Node*>();
    for (auto node : nodes_to_init) {
        nodes.push_back(node);
    }

    neighbour_message_count = vector<MessageCount*>();
}

void Node::step2() {
    // TODO: Algorithm 2
}

void Node::step3_round1() {
    vector<int> message_counts(SET_SIZE, 0);
    // at this point all messages are destined within W only
    for (auto message : messages) {
        message_counts[get_set_from_node_id(nodes, message->dest)]++;
    }

    corollary34_round1(
            message_counts,
            [](int inset_node_idx) { return inset_node_idx; }
    );
}

void Node::step3_round2() {
    corollary_34_round2();
}

vector<vector<int>> Node::step3_round3_create_graph(
        vector<vector<int>>& message_count_per_src_node
) {
    vector<vector<int>> edge_counts(SET_SIZE, vector<int>(SET_SIZE, 0));
    for (int dest_set_idx = 0; dest_set_idx < SET_SIZE; dest_set_idx++) { // for each destination set W
        for (int node_local_idx = 0; node_local_idx < SET_SIZE; node_local_idx++) {
            int curr_messages_to_send = message_count_per_src_node[node_local_idx][dest_set_idx];

            int src_idx = 0;
            while (curr_messages_to_send < SET_SIZE) {
                if (src_idx != node_local_idx && message_count_per_src_node[src_idx][dest_set_idx] > SET_SIZE) {
                    int global_src_idx = get_nth_node_in_set(get_set_idx(), src_idx);
                    auto m = nodes[global_src_idx]->get_message_by_dest_set(dest_set_idx);
                    m->next_dest = get_nth_node_in_set(get_set_idx(), node_local_idx);

                    edge_counts[src_idx][node_local_idx]++;
                    curr_messages_to_send++;
                    message_count_per_src_node[src_idx][dest_set_idx]--;
                }

                src_idx = (src_idx + 1) % SET_SIZE;
            }
        }
    }
    return edge_counts;
}

void Node::step3_round3() {
    vector<vector<int>> message_count_per_src_node(
        SET_SIZE,
        vector<int>(SET_SIZE, 0)
    );

    for (auto mc : neighbour_message_count) {
        int src_local_idx = get_node_idx_in_set(mc->msg_src);
        int dest_set_idx = mc->msg_dest;
        message_count_per_src_node[src_local_idx][dest_set_idx] = mc->msg_count;
    }

    vector<vector<int>> edge_counts = step3_round3_create_graph(
         message_count_per_src_node
    );
    corollary_34_round3(edge_counts, 3);
}

void Node::step3_round4() {
    corollary_34_round4(3);
}

void Node::clear_neighbour_mcs() {
    neighbour_message_count = vector<MessageCount*>();
}

void Node::send_cross_set() {
    int src_set_idx = get_set_idx();

    vector<int> last_idx_per_set;
    last_idx_per_set.resize(sqrt(nodes.size()));

    start_message_count();
    for (auto i = messages.begin(); i != messages.end();) {
        auto message = *i;
        int dest_set_idx = get_set_from_node_id(nodes, message->dest);
        if (dest_set_idx != src_set_idx) {
            int intermediate_dest = get_nth_node_in_set(
                    dest_set_idx,
                    last_idx_per_set[dest_set_idx]
            );

            last_idx_per_set[dest_set_idx]++;
            send_message(message, intermediate_dest, 0);
            i = messages.erase(i);
        } else {
            i++;
        }
    }
    check_message_count();
}

void Node::send_within_set_round1() {
    vector<int> message_counts(nodes.size(), 0);
    message_counts.resize(nodes.size());
    // at this point all messages are destined within W only
    for (auto message : messages) {
        message_counts[message->dest]++;
    }

    corollary34_round1(
        message_counts,
        [this](int inset_node_idx) {
            return this->get_nth_node_in_set(
                get_set_idx(),
                inset_node_idx
            );
        }
    );
}

// step 2 of Corollary 3.4
void Node::send_within_set_round2() {
    corollary_34_round2();
}

void Node::prepare_message_for_final_transfer() {
    for (auto i = messages.begin(); i != messages.end();) {
        auto message = *i;
        message->next_dest = message->dest;
        message->step_to_be_sent = 5;
        i++;
    }
}

void Node::send_within_set_round3() {
    vector<vector<int>> edge_counts(SET_SIZE, vector<int>(SET_SIZE, 0));

    for (auto mc : neighbour_message_count) {
        int src_local_idx = get_node_idx_in_set(mc->msg_src);
        int dest_local_idx = get_node_idx_in_set(mc->msg_dest);
        edge_counts[src_local_idx][dest_local_idx] = mc->msg_count;
    }

    prepare_message_for_final_transfer();
    corollary_34_round3(edge_counts, 5);
}

void Node::send_within_set_round4() {
    corollary_34_round4(5);
}