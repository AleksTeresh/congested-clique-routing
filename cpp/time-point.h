#ifndef CONGESTED_CLIQUE_ROUTING_TIME_POINT_H
#define CONGESTED_CLIQUE_ROUTING_TIME_POINT_H

#include "node.h"
#include <vector>

struct NodeData {
    int id;
    std::vector<Message> messages;
    std::vector<MessageCount> message_counts;

    NodeData() {}
    NodeData(
            int id,
            std::vector<std::unique_ptr<Message>>& messages,
            std::vector<std::unique_ptr<MessageCount>>& message_counts,
            std::vector<std::unique_ptr<MessageCount>>& algo2_message_counts
    ) {
        this->id = id;
        for (auto& m : messages) {
            this->messages.push_back(*m);
        }
        for (auto& mc : message_counts) {
            this->message_counts.push_back(*mc);
        }
        for (auto& mc : algo2_message_counts) {
            this->message_counts.push_back(*mc);
        }
    }
};

struct TimePoint {
    std::vector<NodeData> nodes;
};

#endif //CONGESTED_CLIQUE_ROUTING_TIME_POINT_H
