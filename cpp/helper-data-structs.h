//
// Created by Aleksandr Tereshchenko on 19.3.2020.
//

#ifndef CPP_HELPER_DATA_STRUCTS_H
#define CPP_HELPER_DATA_STRUCTS_H

#include <vector>

template<class T>
using Vec = std::vector<T>;
template<class T>
using Vec2 = std::vector<std::vector<T>>;
template<class T>
using Vec3 = std::vector<std::vector<std::vector<T>>>;

// struct representing an message
struct Message {
    int final_dest; // index of destination node
    int src; // index of src node
    int value;

    // NOTE: the following properties are only used in some algorithm steps

    // the message is to be sent during this step
    // (e.g. some message of node X are to be moved during step 4 and some are not)
    int step_to_be_sent = 0;
    int next_dest = -1; // node idx to which the message should be sent next (i.e. intermediate destination)
    int next_set = -1; // set idx to which the message should be sent next (i.e. intermediate destination)

    Message() {}
    Message(int src, int dest) {
        this->src = src;
        this->final_dest = dest;
    }
};

// struct used to send info related to message counts between nodes
struct MessageCount {
    int msg_src; // the struct reports number of messages from this src (node or set idx)
    int msg_dest; // the struct reports number of messages with destination to this (node or set idx)
    int msg_count = 0; // number of messages from msg_src to msg_dest
    int info_dest; // MessageCount struct is to be delivered to this dest (node idx)

    MessageCount(){}
    MessageCount(int msg_src, int msg_dest, int msg_count, int info_dest) {
        this->msg_src = msg_src;
        this->msg_dest = msg_dest;
        this->msg_count = msg_count;
        this->info_dest = info_dest;
    }
};

#endif //CPP_HELPER_DATA_STRUCTS_H
