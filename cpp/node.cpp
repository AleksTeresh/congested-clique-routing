#include "color.h"
#include "util.h"
#include <unordered_map>
#include <cmath>
#include <numeric>

using namespace std;

void Node::start_message_count() {
    message_sent_count = 0;
}

void Node::check_message_count() {
    // make sure a nodes does not send more than N-1 messages per round
    // (a node does not send a message to itself => N-1
    check_message_count(nodes.size() - 1);
}

void Node::check_message_count(int max_size) {
    assert(message_sent_count <= max_size);
}

int Node::get_local_id_from_global(int node_id) {
    int node_count = nodes.size();
    int set_count = sqrt(node_count);
    return node_id % set_count;
}

int Node::get_global_id_from_local(int set_idx, int node_idx) {
    return set_idx * sqrt(nodes.size()) + node_idx;
}

void Node::send_message(unique_ptr<Message> message, int intermediate_dest, int step_to_be_sent) {
    assert(message->src == global_idx); // a node can call send_message only on itself
    message->step_to_be_sent = step_to_be_sent;
    nodes[intermediate_dest].lock()->add_message(move(message));
    message_sent_count++;
}

void Node::send_message_counts(unique_ptr<MessageCount> mc, int intermediate_dest) {
    nodes[intermediate_dest].lock()->add_received_message_count(move(mc));
    message_sent_count++;
}

// Special version of send_message_counts() for the first step of Algorithm2
// the main reason is that message counts for this step and others need to be stored separately
void Node::send_message_counts_algo2_step1(unique_ptr<MessageCount> mc, int intermediate_dest) {
    nodes[intermediate_dest].lock()->add_also2_step1_message_count(move(mc));
    message_sent_count++;
}

Message& Node::get_message(const function<bool(Message&)>& prerequisite) {
    for (auto& m : messages) {
        if (prerequisite(*m)) {
            return *m;
        }
    }

    throw logic_error("No message was found with required prerequisite.");
}

Vec<unique_ptr<Message>>::iterator Node::get_message_pos(const function<bool(Message&)>& prerequisite) {
    for (auto it = messages.begin(); it != messages.end();) {
        if (prerequisite(**it)) {
            return it;
        } else {
            it++;
        }
    }
    return messages.end();
};

Message& Node::get_message_by_dest_set(int dest) {
    return get_message([this, dest](auto& m) {
        return this->get_set_from_node_id(nodes, m.final_dest) == dest &&
               m.next_dest == -1 &&
               m.next_set == -1;
    });
}

Message& Node::get_message_by_next_set(int next_set_idx) {
    return get_message([this, next_set_idx](auto& m) {
        return m.next_set == next_set_idx &&
               m.next_dest == -1;
    });
}

void assert_no_edges(Vec2<int>& all_messages, int src_idx) {
    for (int edges_remaining : all_messages[src_idx]) {
        assert(edges_remaining == 0);
    }
}

// if some nodes do not have enough edges for needed degree, add loops as needed
void Node::add_missing_edges(Vec2<int>& all_messages, int degree) {
    loop(all_messages.size(), [&](int i) {
        int count = 0;
        loop(all_messages.size(), [&](int j) {
            count += all_messages[i][j];
        });

        if (count < degree) { // if not enough edges for the graph to be regular, add the difference
            all_messages[i][i] = degree - count;
        }
    });
}

Vec3<int> Node::get_graph_coloring(Vec2<int>& all_messages) {
    return get_graph_coloring(all_messages, set_size * set_size);
}

// since the graph is regular bipartite, we can color it by repeatedly finding perfect matching
// the vector is copied because in-place mutation is used
Vec3<int> Node::get_graph_coloring(Vec2<int> all_messages, int degree) {
    add_missing_edges(all_messages, degree);
    Vec3<int> colors(set_size, Vec2<int>(set_size, Vec<int>()));

    int color = -1;
    bool empty = false;
    while (!empty) {
        color++;
        Vec<int> matching = max_BPM(all_messages);
        empty = true;

        loop(matching.size(), [&](int i) {
            int matched_pair = matching[i];
            if (matched_pair == -1) {
                assert_no_edges(all_messages, i);
            } else {
                empty = false;
                all_messages[i][matched_pair]--;
                colors[i][matched_pair].push_back(color);
            }
        });
    }
    assert(color <= degree);

    return colors;
}

unique_ptr<MessageCount> Node::corollary34_create_message_count(
        Vec<int>& message_counts,
        int dest_id, // the MessageCount object will be sent to this node
        int about_node_id // the MessageCount object will have info about number of messages to be sent from global_idx to about_node_id
) {
    int set_id = get_set_idx();
    int global_dest_idx = get_global_id_from_local(set_id, dest_id);
    int message_count_to_destination = message_counts[about_node_id];
    auto mc = make_unique<MessageCount>(
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

        int src_idx_in_set = get_local_id_from_global(global_idx);

        start_message_count();
        // step 1 of Corollary 3.4
        loop(set_size, [&](int dest_local_idx) { // destination node of the message
            loop(set_size, [&](int about_node_local_idx) { // the message contains info about this node
                int c = coloring[src_idx_in_set][dest_local_idx][about_node_local_idx]; // color of the edge corresponding to the message
                int dest_idx = dest_from_inset_node_idx(about_node_local_idx);// get_nth_node_in_set(set_id, about_node_inset_i);
                auto mc = corollary34_create_message_count(message_counts, dest_local_idx, dest_idx);

                send_message_counts(move(mc), c);
            });
        });
        check_message_count(nodes.size());
};

void Node::corollary_34_round2() {
    start_message_count();
    for (auto i = received_message_counts.begin(); i != received_message_counts.end();) {
        auto& mc = *i;
        if (mc->info_dest != global_idx) {
            int info_dest = mc->info_dest;
            send_message_counts(move(*i), info_dest);
            i = received_message_counts.erase(i);
        } else {
            i++;
        }
    }
    check_message_count();
}

void Node::send_message_to_color(int color, int dest_of_message, int curr_algo_step) {
    auto pos = get_message_pos(
            [dest_of_message, curr_algo_step]
                    (Message& m) {
                return m.next_dest == dest_of_message &&
                       m.step_to_be_sent != curr_algo_step;
            });

    if (pos != messages.end()) {
        send_message(move(*pos), color, curr_algo_step);
        pos = messages.erase(pos);
    } else {
        throw logic_error("Message has not been found");
    }
}

void Node::send_message_to_itself(int dest_of_message, int curr_algo_step) {
    auto& message = get_message(
            [dest_of_message, curr_algo_step]
            (Message& m) {
             return m.next_dest == dest_of_message &&
                    m.step_to_be_sent != curr_algo_step;
            });
    message.step_to_be_sent = curr_algo_step;
}

void Node::corollary_34_round3(Vec2<int>& edge_counts, int curr_algo_step) {
    auto coloring = get_graph_coloring(edge_counts);

    int src_idx_in_set = get_local_id_from_global(global_idx);
    int set_id = get_set_idx();

    start_message_count();

    loop(set_size, [&](int dest_inset_idx) { // destination node of the message
        int global_dest_idx = get_global_id_from_local(set_id, dest_inset_idx);
        if (global_dest_idx != global_idx) { // skip sending messages to itself
            for (int c : coloring[src_idx_in_set][dest_inset_idx]) { // color of the edge corresponding to the message
                if (c == global_idx) { // skip sending messages to itself
                    send_message_to_itself(global_dest_idx, curr_algo_step);
                } else {
                    send_message_to_color(c, global_dest_idx, curr_algo_step);
                }
            }
        }
    });
    check_message_count();
}

void Node::corollary_34_round4(int current_algo_step) {
    start_message_count();
    for (auto i = messages.begin(); i != messages.end();) {
        auto& message = *i;
        if (message->next_dest != global_idx &&
            message->step_to_be_sent == current_algo_step
        ) {
            int next_dest = message->next_dest;
            send_message(move(*i), next_dest, 0);
            i = messages.erase(i);
        } else {
            i++;
        }
    }
    check_message_count();
}

Vec<unique_ptr<Message>>& Node::get_messages() {
    return messages;
}

Vec<unique_ptr<MessageCount>>& Node::get_message_counts() {
    return received_message_counts;
}

Vec<unique_ptr<MessageCount>>& Node::get_algo2_message_counts() {
    return algo2_step1_message_counts;
}

void Node::add_messages(Vec<int>& new_messages) {
    for (int message : new_messages) {
        add_message(message);
    }
}

void Node::add_message(int message_dest) {
    auto m = make_unique<Message>(get_node_idx(), message_dest);
    messages.push_back(move(m));
}

void Node::add_message(unique_ptr<Message> m) {
    m->src = get_node_idx();
    messages.push_back(move(m));
}

int Node::get_node_idx() {
    return this->global_idx;
}

int Node::get_set_idx() {
    return get_set_from_node_id(nodes, this->global_idx);
}

void Node::add_also2_step1_message_count(unique_ptr<MessageCount> mc) {
    this->algo2_step1_message_counts.push_back(move(mc));
}

void Node::add_received_message_count(unique_ptr<MessageCount> mc) {
    this->received_message_counts.push_back(move(mc));
}

void Node::init(Vec<shared_ptr<Node>>& nodes_to_init) {
    set_size = sqrt(nodes_to_init.size());
    nodes = Vec<weak_ptr<Node>>();
    for (const auto& node : nodes_to_init) {
        nodes.push_back(node);
    }

    received_message_counts = Vec<unique_ptr<MessageCount>>();
}

Vec<int> Node::count_messages_for_each_subset() {
    Vec<int> message_counts(set_size, 0);
    // count messages destined to each subset W'
    for (auto& message : messages) {
        int dest_set_idx = get_set_from_node_id(nodes, message->final_dest);
        message_counts[dest_set_idx]++;
    }
    return message_counts;
}

// send count figures for i'th subset to i'th node in the current subset W (with local index i)
void Node::send_counts_for_ith_subset_locally(Vec<int> message_counts) {
    loop(set_size, [&](int i) {
        int local_dest_idx = i;
        int global_dest_idx = get_global_id_from_local(get_set_idx(), local_dest_idx);
        int set_idx = i;

        auto mc = make_unique<MessageCount>(
                get_set_idx(),
                set_idx,
                message_counts[set_idx],
                -1
        );
        send_message_counts_algo2_step1(move(mc), global_dest_idx);
    });
}

// send the number of messages destined to i'th subset that the
// current node holds to i'th node of the current subset W
void Node::step2_round1() {
    auto message_counts = count_messages_for_each_subset();
    send_counts_for_ith_subset_locally(message_counts);
}

Vec<int> Node::sum_up_message_counts() {
    Vec<int> message_counts(set_size, 0);
    // sum up the number of messages W holds for i'th subset W' (where i is local idx of the curent node)
    for (auto it = algo2_step1_message_counts.begin(); it != algo2_step1_message_counts.end();) {
        auto& mc = *it;
        if (mc->msg_src == get_set_idx() && mc->info_dest == -1) {
            int dest_set_idx = mc->msg_dest;
            message_counts[dest_set_idx] += mc->msg_count;
            it = algo2_step1_message_counts.erase(it);
        } else {
            it++;
        }
    }
    return message_counts;
}

void Node::broadcast_message_counts(Vec<int> message_counts) {
    // announce the number of messages W holds for i'th subset W' to all nodes
    for (auto& node : nodes) {
        int i = get_local_id_from_global(global_idx); // stands for destination set idx
        auto locked_node = node.lock();
        auto mc = make_unique<MessageCount>(
                get_set_idx(),
                i,
                message_counts[i],
                locked_node->get_node_idx()
        );
        send_message_counts_algo2_step1(move(mc), locked_node->get_node_idx());
    }
}

// the node announces message counts (from this subset W to i'th subset W'
// (where i is local index of the node)) to all the nodes in the clique
void Node::step2_round2() {
    auto message_counts = sum_up_message_counts();
    broadcast_message_counts(message_counts);
}

void Node::compute_set_to_set_graph() {
    Vec2<int> message_count( // from set W to set W'
            set_size,
            Vec<int>(set_size, 0)
    );

    // count total number of messages from each subset to each subset
    for (auto it = algo2_step1_message_counts.begin(); it != algo2_step1_message_counts.end();) {
        auto& mc = *it;
        int src_set_idx = mc->msg_src;
        int dest_set_idx = mc->msg_dest;
        message_count[src_set_idx][dest_set_idx] = mc->msg_count;
        // delete message_counts objects
        it = algo2_step1_message_counts.erase(it);
    }

    // graph's degree is set_size ^ 3, since each node needs to send and receive set_size^2 messages,
    // and there are set_size nodes in each subset. => can be edge-colored with set_size ^ 3 colors
    auto coloring = get_graph_coloring(message_count, set_size * set_size * set_size);
    this->step2_coloring = coloring; // save the coloring of the graph for future rounds
}

// announce to all other nodes in current subset the number of message this node has destined for each subset W'
// i.e. just compute sum of message counts for each subset W' and send set_size pieces of info to each of the nodes in the subset W
// corollary 3.4 is used => the operation is done in 2 rounds
void Node::broadcast_message_counts_locally() {
    Vec<int> message_counts(set_size, 0);

    for (auto& message : messages) {
        message_counts[get_set_from_node_id(nodes, message->final_dest)]++;
    }

    corollary34_round1(
            message_counts,
            [](int inset_node_idx) { return inset_node_idx; }
    );
}

// construct bipartite graph G as follows.
// set_size senders, set_size receivers. For each message W needs to send to W', add an edge from S to R
void Node::step2_round3() {
    compute_set_to_set_graph();
    broadcast_message_counts_locally();
}

// 2nd round of corollary 3.4, see comments for step2_round4.
void Node::step2_round4() {
    corollary_34_round2();
}


Vec2<int> Node::count_messages_inter_set() {
    Vec2<int> final_message_count( // from node i in W to set W'
            set_size,
            Vec<int>(set_size, 0)
    );
    // count messages from each node i in W to each of the sets W',
    // and delete message_counts objects that has been used to collect the data
    // This info is available due to rounds 4 and 5.
    for (auto it = received_message_counts.begin(); it != received_message_counts.end();) {
        auto& mc = *it;
        int src_idx = get_local_id_from_global(mc->msg_src);
        int dest_set_idx = mc->msg_dest;
        final_message_count[src_idx][dest_set_idx] = mc->msg_count;
        it = received_message_counts.erase(it);
    }

    return final_message_count;
}

Vec2<int> Node::build_inter_set_graph(Vec2<int> final_message_count) {
    Vec2<int> message_count( // from node i in W to set W'
            set_size,
            Vec<int>(set_size, 0)
    );

    // This for loop construct another bipartite graph G. s.t. nodes in the current subset are on one side
    // and each of the sets W' are on the other.
    // The idea is to use previously computed coloring and construct a new graph s.t. a message of color c is sent
    // (as an intermediate destination) to set (c % set_size).
    loop(set_size, [&](int final_dest_set) { // for each destination subset W'
        for (auto c : step2_coloring[get_set_idx()][final_dest_set]) { // get next colored edge from W to W'
            int intermediate_dest_set = c % set_size; // idx of the set the message should first be sent to

            // Now we know that a message with color c should be sent from W
            // to intermediate_dest_set (as intermediate destination).
            // In the for loop, we're looking for a node that has a message that suits the characteristics
            // (i.e. final destinations should match)
            int local_src_idx = 0; // candidate local node
            while (true) {
                if (final_message_count[local_src_idx][final_dest_set] > 0) { // we found a source node for this particular color
                    set_next_set_to_message(final_dest_set, local_src_idx, intermediate_dest_set);
                    message_count[local_src_idx][intermediate_dest_set]++;
                    final_message_count[local_src_idx][final_dest_set]--;
                    break;
                }
                local_src_idx++;
            }
        }
    });

    return message_count;
}

// now based on the second bipartite graph coloring, construct a graph (i.e. plan for sending)
// s.t. message of color i is sent to (c % set_size) node in current W (i.e. node that has local idx c % set_size)
Vec2<int> Node::build_inset_graph(Vec3<int> coloring) {
    Vec2<int> edge_counts(set_size, Vec<int>(set_size, 0));

    loop(set_size, [&](int local_src_idx) {
        loop(set_size, [&](int dest_set_idx) {
            for (auto c : coloring[local_src_idx][dest_set_idx]) {
                int local_dest_idx = c % set_size;
                set_next_dest_to_message_by_next_set(dest_set_idx, local_src_idx, local_dest_idx);
                edge_counts[local_src_idx][local_dest_idx]++;
            }
        });
    });

    return edge_counts;
}

void Node::step2_round5() {
    auto inter_set_message_count = count_messages_inter_set();
    auto message_count = build_inter_set_graph(inter_set_message_count);
    // the we color the newly constructed bipartite graph (from each node of W to intermediate destination set R)
    auto coloring = get_graph_coloring(message_count); // graph of degree set_size * set_size => n colors used
    auto edge_counts = build_inset_graph(coloring);
    // the computed plan will be next executed in 2 rounds (using Corollary 3.4). i.e. the messages are moved only
    // within W for now.
    // In round8, the messages will finally be moved to other W' subset (as their intertmediate destinations)
    corollary_34_round3(edge_counts, 2);
}

// 2nd round of Corollary 3.4. See comments for round 6.
void Node::step2_round6() { // send messages within W only as needed
    corollary_34_round4(2);
}

// By now we've achieved that each node holds exactly set_size messages with destination of W' (for each W')
// Thus, current, node can send one message to each of the set_size nodes of W' (for each W')
void Node::step2_round7() { // each node in in W sends one message to each node
    Vec<int> next_dest_idx_for_sets(set_size, 0);

    start_message_count();
    for (auto i = messages.begin(); i != messages.end();) { // for each message held
        auto& message = *i;
        if (message->next_set == -1) { // message is in the right intermediate destination set already
            i++;
            continue;
        }

        int next_set = message->next_set;
        int local_dest_idx = next_dest_idx_for_sets[next_set];
        int global_dest_idx = get_global_id_from_local(next_set, local_dest_idx);

        message->next_set = -1; // mark the message s.t. it is already in its intermediate destination set
        if (global_dest_idx != global_idx) { // if dest is not the current node
            send_message(move(*i), global_dest_idx, 0);
            i = messages.erase(i);
        } else {
            i++;
        }

        next_dest_idx_for_sets[next_set]++;
    }
    check_message_count();
}

// the node announces to each other node in W, how many messages
// it holds for each subset W'
void Node::step3_round1() {
    Vec<int> message_counts(set_size, 0);
    // count how many messages this node has for each of the subsets
    for (auto& message : messages) {
        message_counts[get_set_from_node_id(nodes, message->final_dest)]++;
    }

    // announce to other nodes in W how many messages this node has for other subsets
    corollary34_round1(
            message_counts,
            [](int inset_node_idx) { return inset_node_idx; }
    );
}

// second rounds. This completes previous round
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
    int global_src_idx = get_global_id_from_local(get_set_idx(), local_src_idx);

    if (global_src_idx == global_idx) {
        Message& m = get_message_by_dest_set(message_dest_set_idx);
        m.next_dest = get_global_id_from_local(get_set_idx(), local_dest_idx);
    }
}

void Node::set_next_set_to_message(int final_dest_set, int local_src_idx, int intermediate_dest_set) {
    int global_src_idx = get_global_id_from_local(get_set_idx(), local_src_idx);

    if (global_src_idx == global_idx) {
        Message& m = get_message_by_dest_set(final_dest_set);
        m.next_set = intermediate_dest_set;
    }
}

void Node::set_next_dest_to_message_by_next_set(
        int dest_set_idx, // how to find
        int local_src_idx, // from where
        int local_dest_idx // next dest
) {
    int global_src_idx = get_global_id_from_local(get_set_idx(), local_src_idx);

    if (global_src_idx == global_idx) {
        auto& m = get_message_by_next_set(dest_set_idx);
        m.next_dest = get_global_id_from_local(get_set_idx(), local_dest_idx);
    }
}

void Node::update_counts(
    Vec2<int>& message_counts, // message counts from a node in W to a set W'
    Vec2<int>& edge_counts,
    int node_local_idx,
    int local_src_idx,
    int dest_set_idx
) {
    edge_counts[local_src_idx][node_local_idx]++;
    message_counts[node_local_idx][dest_set_idx]++;
    message_counts[local_src_idx][dest_set_idx]--;
}

void Node::find_missing_messages(
    Vec2<int>& message_counts, // message counts from a node in W to a set W'
    Vec2<int>& edge_counts,
    int node_local_idx,
    int dest_set_idx
) {
    int& curr_messages_to_send = message_counts[node_local_idx].at(dest_set_idx);
    int local_src_idx = 0;
    // while the node still needs more message with destination in set W'
    // find the message from other nodes in set W and send them to the node
    while (curr_messages_to_send < set_size) {
        if (
            local_src_idx != node_local_idx && // src is not dest
            node_has_extra_messages_for_set(message_counts, local_src_idx, dest_set_idx)
        ) {
            set_next_dest_to_message(dest_set_idx, local_src_idx, node_local_idx);
            update_counts(
                    message_counts,
                    edge_counts,
                    node_local_idx,
                    local_src_idx,
                    dest_set_idx);
        }

        local_src_idx = (local_src_idx + 1) % set_size;
    }
}

// build a bipartite graph where vertices are nodes of the current subset in their
// sending and receiving roles. The idea is to find nodes that have too many
// messages for some subset W' and nodes that have too few, and add edges from the former
// to the latter ones.
Vec2<int> Node::step3_round3_create_graph(
        Vec2<int>& message_counts // message counts from a node in W to a set W'
) {
    Vec2<int> edge_counts(set_size, Vec<int>(set_size, 0));

    loop(set_size, [&](int dest_set_idx) { // for each destination set W'
        loop(set_size, [&](int node_local_idx) {
            // for each node in current set W, make sure that it has set_size messages with destination in W'
            find_missing_messages(
                    message_counts,
                    edge_counts,
                    node_local_idx,
                    dest_set_idx);
        });
    });
    return edge_counts;
}

// calculate how many messages each node in the subset W has for each subset W'. If the number if <= set_size
// find messages with the destination in the same subset W' among other nodes of the current subset W.
// Finally send messages within nodes of the current subset s.t. in the end each node has exactly set_size
// messages with destination in each subset W'
void Node::step3_round3() {
    Vec2<int> message_count_per_src_node(
        set_size,
        Vec<int>(set_size, 0)
    );

    // calculate message count for each node in the subset W to each of the subsets W'
    for (auto& mc : received_message_counts) {
        int src_local_idx = get_local_id_from_global(mc->msg_src);
        int dest_set_idx = mc->msg_dest;
        message_count_per_src_node[src_local_idx][dest_set_idx] = mc->msg_count;
    }

    // build the graph according to which the "missing" message
    // will be sent from nodes that have too many messages destined to some W'
    // to nodes that have too few
    Vec2<int> edge_counts = step3_round3_create_graph(
         message_count_per_src_node
    );
    corollary_34_round3(edge_counts, 3);
}

void Node::step3_round4() {
    corollary_34_round4(3);
}

void Node::clear_received_mcs() {
    received_message_counts = Vec<unique_ptr<MessageCount>>();
}

void Node::reset_message_next_dest() {
    for (auto& m : messages) {
        m->next_set = -1;
        m->next_dest = -1;
    }
}

// step 4
void Node::send_cross_set() {
    int src_set_idx = get_set_idx();

    Vec<int> last_idx_per_set;
    last_idx_per_set.resize(sqrt(nodes.size()));

    start_message_count();
    for (auto i = messages.begin(); i != messages.end();) {
        auto& message = *i;
        int dest_set_idx = get_set_from_node_id(nodes, message->final_dest);
        if (dest_set_idx != src_set_idx) {
            int intermediate_dest = get_global_id_from_local(
                    dest_set_idx,
                    last_idx_per_set[dest_set_idx]
            );

            last_idx_per_set[dest_set_idx]++;
            send_message(move(*i), intermediate_dest, 0);
            i = messages.erase(i);
        } else {
            i++;
        }
    }
    check_message_count();
}

// prepare messages for the last 2 rounds of Step 5 Algorithm 1
// sets next_dest to equal final_dest
void Node::prepare_message_for_final_transfer() {
    for (auto i = messages.begin(); i != messages.end();) {
        auto& message = *i;
        message->next_dest = message->final_dest;
        message->step_to_be_sent = -1;
        i++;
    }
}

// step 5 round 1
//
void Node::send_within_set_round1() {
    Vec<int> message_counts(nodes.size(), 0);
    message_counts.resize(nodes.size());
    // at this point all messages are destined within W only
    for (auto& message : messages) {
        message_counts[message->final_dest]++;
    }

    corollary34_round1(
        message_counts,
        [this](int inset_node_idx) {
            return this->get_global_id_from_local(
                    get_set_idx(),
                    inset_node_idx
            );
        }
    );
}

// step 5 round 2
void Node::send_within_set_round2() {
    corollary_34_round2();
}

// step 5 round 3
void Node::send_within_set_round3() {
    Vec2<int> edge_counts(set_size, Vec<int>(set_size, 0));

    for (const auto& mc : received_message_counts) {
        int src_local_idx = get_local_id_from_global(mc->msg_src);
        int dest_local_idx = get_local_id_from_global(mc->msg_dest);
        edge_counts[src_local_idx][dest_local_idx] = mc->msg_count;
    }

    corollary_34_round3(edge_counts, 5);
}

// step 5 round 4
void Node::send_within_set_round4() {
    corollary_34_round4(5);
}
