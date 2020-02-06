#include <unordered_map>
#include <cmath>
#include "color.cpp"
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

Vec<Message*>::iterator Node::get_message_position(const function<bool(Message*)> prerequisite) {
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
        return this->get_set_from_node_id(nodes, m->dest) == dest && m->next_dest == -1 && m->next_set == -1;
    });
}

Message* Node::get_message_by_next_set(int next_set_idx) {
    return get_message([this, next_set_idx](auto m) {
        return m->next_set == next_set_idx && m->next_dest == -1;
    });
}

void assert_no_edges(Vec2<int>& all_messages, int src_idx) {
    for (int edges_remaining : all_messages[src_idx]) {
        assert(edges_remaining == 0);
    }
}

void Node::add_missing_edges(Vec2<int>& all_messages, int degree) {
    for (int i = 0; i < all_messages.size(); i++) {
        int count = 0;
        for (int j = 0; j < all_messages.size(); j++) {
            count += all_messages[i][j];
        }

        if (count < degree) {
            all_messages[i][i] = degree - count;
        }
    }
}

Vec3<int> Node::get_graph_coloring(Vec2<int> all_messages) {
    return get_graph_coloring(all_messages, set_size * set_size);
}

// the parameter is modified => the vector is copied instead of reference being used
Vec3<int> Node::get_graph_coloring(Vec2<int> all_messages, int degree) {
    add_missing_edges(all_messages, degree);
    Vec3<int> colors(set_size, Vec2<int>(set_size, Vec<int>()));

    int color = -1;
    bool empty = false;
    while (!empty) {
        color++;
        Vec<int> matching = max_BPM(all_messages);
        empty = true;

        for (int i = 0; i < matching.size(); i++) {
            int matched_pair = matching[i];
            if (matched_pair == -1) {
                assert_no_edges(all_messages, i);
            } else {
                empty = false;
                all_messages[i][matched_pair]--;
                colors[i][matched_pair].push_back(color);
            }
        }
    }
    assert(color <= degree);

    return colors;
}

MessageCount* Node::corollary34_create_message_count(
        Vec<int>& message_counts,
        int dest_id, // the MessageCount object will be sent to this node
        int about_node_id // the MessageCount object will have info about number of messages to be sent from global_idx to about_node_id
) {
    int set_id = get_set_idx();
    int global_dest_idx = get_nth_node_in_set(set_id, dest_id);
    int message_count_to_destination = message_counts[about_node_id];
    auto mc = new MessageCount(
            global_idx,
            about_node_id,
            message_count_to_destination,
            global_dest_idx
    );
    return mc;
}

void Node::corollary34_round1(Vec<int>& message_counts, const function<int(int)>& dest_from_inset_node_idx) {
        Vec2<int> edge_counts(set_size, Vec<int>(set_size, set_size));
        auto coloring = get_graph_coloring(edge_counts);

        int src_idx_in_set = get_node_idx_in_set(global_idx);


        start_message_count();
        // step 1 of Corollary 3.4
        for (int dest_inset_i = 0; dest_inset_i < set_size; dest_inset_i++) { // destination node of the message
            for (int about_node_inset_i = 0; about_node_inset_i < set_size; about_node_inset_i++) { // the message contains info about this node
                int c = coloring[src_idx_in_set][dest_inset_i][about_node_inset_i]; // color of the edge corresponding to the message
                int dest_idx = dest_from_inset_node_idx(about_node_inset_i);// get_nth_node_in_set(set_id, about_node_inset_i);
                auto mc = corollary34_create_message_count(message_counts, dest_inset_i, dest_idx);

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
    } else {
        assert(1 == 0);
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
    } else {
        assert(1 == 0);
    }
}

void Node::corollary_34_round3(Vec2<int>& edge_counts, int curr_algo_step) {
    auto coloring = get_graph_coloring(edge_counts);

    int src_idx_in_set = get_node_idx_in_set(global_idx);
    int set_id = get_set_idx();

    start_message_count();
    for (int dest_inset_idx = 0; dest_inset_idx < set_size; dest_inset_idx++) { // destination node of the message
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

Vec<Message*>& Node::get_messages() {
    return messages;
}

Vec<MessageCount*>& Node::get_message_counts() {
    return neighbour_message_count;
}

void Node::add_messages(Vec<int>& new_messages) {
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

void Node::init(Vec<Node*>& nodes_to_init) {
    set_size = sqrt(nodes_to_init.size());
    nodes = Vec<Node*>();
    for (auto node : nodes_to_init) {
        nodes.push_back(node);
    }

    neighbour_message_count = Vec<MessageCount*>();
}

void Node::step2_round1() {
    Vec<int> message_counts(set_size, 0);
    for (auto message : messages) {
        int dest_set_idx = get_set_from_node_id(nodes, message->dest);
        message_counts[dest_set_idx]++;
    }

    for (int i = 0; i < set_size; i++) {
        int local_dest_idx = i;
        int global_dest_idx = get_nth_node_in_set(get_set_idx(), local_dest_idx);
        int set_idx = i;

        auto mc = new MessageCount(
                get_set_idx(),
                set_idx,
                message_counts[set_idx],
                -1
        );
        send_neighbour_message_count(mc, global_dest_idx);
    }
}

void Node::step2_round2() {
    Vec<int> message_counts(set_size, 0);
    for (auto it = neighbour_message_count.begin(); it != neighbour_message_count.end();) {
        auto mc = *it;
        if (mc->msg_src == get_set_idx() && mc->info_dest == -1) {
            int dest_set_idx = mc->msg_dest;
            message_counts[dest_set_idx] += mc->msg_count;
            it = neighbour_message_count.erase(it);
        } else {
            it++;
        }
    }

    for (auto node : nodes) {
        int i = get_node_idx_in_set(global_idx); // stands for destination set idx
        auto mc = new MessageCount(
                get_set_idx(),
                i,
                message_counts[i],
                node->get_node_idx()
        );
        send_neighbour_message_count(mc, node->get_node_idx());
    }
}

void Node::step2_round3() {
    Vec2<int> message_count( // from set W to set W'
            set_size,
            Vec<int>(set_size, 0)
    );

    for (auto it = neighbour_message_count.begin(); it != neighbour_message_count.end();) {
        auto mc = *it;
        int src_set_idx = mc->msg_src;
        int dest_set_idx = mc->msg_dest;
        message_count[src_set_idx][dest_set_idx] = mc->msg_count;
        it = neighbour_message_count.erase(it);
    }

    auto coloring = get_graph_coloring(message_count, set_size * set_size * set_size);
    this->step2_coloring = coloring;
}

void Node::step2_round4() {
    Vec<int> message_counts(set_size, 0);

    for (auto message : messages) {
        message_counts[get_set_from_node_id(nodes, message->dest)]++;
    }

    corollary34_round1(
            message_counts,
            [](int inset_node_idx) { return inset_node_idx; }
    );
}

void Node::step2_round5() {
    corollary_34_round2();
}

void Node::step2_round6() {
    Vec2<int> final_message_count( // from node i in W to set W'
            set_size,
            Vec<int>(set_size, 0)
    );
    for (auto it = neighbour_message_count.begin(); it != neighbour_message_count.end();) {
        auto mc = *it;
        int src_idx = get_node_idx_in_set(mc->msg_src);
        int dest_set_idx = mc->msg_dest;
        final_message_count[src_idx][dest_set_idx] = mc->msg_count;
        it = neighbour_message_count.erase(it);
    }

    Vec2<int> message_count( // from node i in W to set W'
            set_size,
            Vec<int>(set_size, 0)
    );
    for (int final_dest_set = 0; final_dest_set < set_size; final_dest_set++) {
        for (auto c : step2_coloring[get_set_idx()][final_dest_set]) {
            int intermediate_dest_set = c % set_size;

            int local_src_idx = 0;
            while (true) {
                if (final_message_count[local_src_idx][final_dest_set] > 0) { // we found a source for this particular color
                    set_next_set_to_message(final_dest_set, local_src_idx, intermediate_dest_set);
                    message_count[local_src_idx][intermediate_dest_set]++;
                    final_message_count[local_src_idx][final_dest_set]--;
                    break;
                }
                local_src_idx++;
            }
        }
    }
    Vec3<int> coloring = get_graph_coloring(message_count); // graph of degree set_size * set_size => n colors used

    Vec2<int> edge_counts(set_size, Vec<int>(set_size, 0));
    for (int local_src_idx = 0; local_src_idx < set_size; local_src_idx++) {
        for (int dest_set_idx = 0; dest_set_idx < set_size; dest_set_idx++) {
            for (auto c : coloring[local_src_idx][dest_set_idx]) {
                int local_dest_idx = c % set_size;
                set_next_dest_to_message_by_next_set(dest_set_idx, local_src_idx, local_dest_idx);
                edge_counts[local_src_idx][local_dest_idx]++;
            }
        }
    }

    corollary_34_round3(edge_counts, 2);
}

void Node::step2_round7() { // send messages within W only as needed
    corollary_34_round4(2);
}

void Node::step2_round8() { // each node in in W sends one message to each node
    Vec<int> next_dest_idx_for_sets(set_size, 0);

    start_message_count();
    for (auto i = messages.begin(); i != messages.end();) {
        auto message = *i;
        if (message->next_set == -1) { // already sent
            i++;
            continue;
        }

        int next_set = message->next_set;
        int inset_dest_idx = next_dest_idx_for_sets[next_set];
        int global_dest_idx = get_nth_node_in_set(next_set, inset_dest_idx);

        message->next_set = -1;
        if (global_dest_idx != global_idx) {
            send_message(message, global_dest_idx, 0);
            i = messages.erase(i);
        } else {
            i++;
        }

        next_dest_idx_for_sets[next_set]++;
    }
    check_message_count();
}

void Node::step3_round1() {
    Vec<int> message_counts(set_size, 0);
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

bool Node::node_has_extra_messages_for_set(
        Vec2<int>& message_counts,
        int local_src_idx,
        int dest_node_idx
) {
    return message_counts[local_src_idx][dest_node_idx] > set_size;
}

void Node::set_next_dest_to_message(
        int message_dest_set_idx, // how to find
        int local_src_idx, // from where
        int local_dest_idx // next dest
) {
    int global_src_idx = get_nth_node_in_set(get_set_idx(), local_src_idx);

    if (global_src_idx == global_idx) {
        auto m = get_message_by_dest_set(message_dest_set_idx);
        m->next_dest = get_nth_node_in_set(get_set_idx(), local_dest_idx);
    }
}

void Node::set_next_set_to_message(int final_dest_set, int local_src_idx, int intermediate_dest_set) {
    int global_src_idx = get_nth_node_in_set(get_set_idx(), local_src_idx);

    if (global_src_idx == global_idx) {
        auto m = get_message_by_dest_set(final_dest_set);
        m->next_set = intermediate_dest_set;
    }
}

void Node::set_next_dest_to_message_by_next_set(
        int dest_set_idx, // how to find
        int local_src_idx, // from where
        int local_dest_idx // next dest
) {
    int global_src_idx = get_nth_node_in_set(get_set_idx(), local_src_idx);

    if (global_src_idx == global_idx) {
        auto m = get_message_by_next_set(dest_set_idx);
        m->next_dest = get_nth_node_in_set(get_set_idx(), local_dest_idx);
    }
}

Vec2<int> Node::step3_round3_create_graph(
        Vec2<int>& message_counts // message counts from a node in W to a set W'
) {
    Vec2<int> edge_counts(set_size, Vec<int>(set_size, 0));
    for (int dest_set_idx = 0; dest_set_idx < set_size; dest_set_idx++) { // for each destination set W'
        // for each node in current set W, make sure that it has set_size of messages with destination in W'
        for (int node_local_idx = 0; node_local_idx < set_size; node_local_idx++) {
            int curr_messages_to_send = message_counts[node_local_idx][dest_set_idx];

            int local_src_idx = 0;
            // while the node still needs more message with destination in set W'
            // find the message from other nodes in set W and send them to the node
            while (curr_messages_to_send < set_size) {
                if (
                    local_src_idx != node_local_idx && // src is not dest
                    node_has_extra_messages_for_set(message_counts, local_src_idx, dest_set_idx)
                ) {
                    set_next_dest_to_message(dest_set_idx, local_src_idx, node_local_idx);
                    edge_counts[local_src_idx][node_local_idx]++;
                    message_counts[node_local_idx][dest_set_idx]++;
                    message_counts[local_src_idx][dest_set_idx]--;
                    curr_messages_to_send = message_counts[node_local_idx][dest_set_idx];
                }

                local_src_idx = (local_src_idx + 1) % set_size;
            }
        }
    }
    return edge_counts;
}

void Node::step3_round3() {
    Vec2<int> message_count_per_src_node(
        set_size,
        Vec<int>(set_size, 0)
    );

    for (auto mc : neighbour_message_count) {
        int src_local_idx = get_node_idx_in_set(mc->msg_src);
        int dest_set_idx = mc->msg_dest;
        message_count_per_src_node[src_local_idx][dest_set_idx] = mc->msg_count;
    }

    Vec2<int> edge_counts = step3_round3_create_graph(
         message_count_per_src_node
    );
    corollary_34_round3(edge_counts, 3);
}

void Node::step3_round4() {
    corollary_34_round4(3);
}

void Node::clear_neighbour_mcs() {
    neighbour_message_count = Vec<MessageCount*>();
}

void Node::reset_message_next_dest() {
    for (auto m : messages) {
        m->next_set = -1;
        m->next_dest = -1;
    }
}

void Node::send_cross_set() {
    int src_set_idx = get_set_idx();

    Vec<int> last_idx_per_set;
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
    Vec<int> message_counts(nodes.size(), 0);
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

// prepare messages for the last 2 rounds of Step 5 Algorithm 1
void Node::prepare_message_for_final_transfer() {
    for (auto i = messages.begin(); i != messages.end();) {
        auto message = *i;
        message->next_dest = message->dest;
        message->step_to_be_sent = -1;
        i++;
    }
}

void Node::send_within_set_round3() {
    Vec2<int> edge_counts(set_size, Vec<int>(set_size, 0));

    for (auto mc : neighbour_message_count) {
        int src_local_idx = get_node_idx_in_set(mc->msg_src);
        int dest_local_idx = get_node_idx_in_set(mc->msg_dest);
        edge_counts[src_local_idx][dest_local_idx] = mc->msg_count;
    }

    corollary_34_round3(edge_counts, 5);
}

void Node::send_within_set_round4() {
    corollary_34_round4(5);
}