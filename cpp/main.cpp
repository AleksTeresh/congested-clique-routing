#include <emscripten/bind.h>
#include "test.cpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_module) {
        value_object<Message>("Message")
                .field("src", &Message::src)
                .field("final_dest", &Message::final_dest);

        value_object<MessageCount>("MessageCount")
                .field("msg_src", &MessageCount::msg_src)
                .field("msg_dest", &MessageCount::msg_dest)
                .field("msg_count", &MessageCount::msg_count)
                .field("info_dest", &MessageCount::info_dest);

        value_object<NodeData>("NodeData")
                .field("id", &NodeData::id)
                .field("messages", &NodeData::messages)
                .field("message_counts", &NodeData::message_counts);

        value_object<TimePoint>("TimePoint")
                .field("nodes", &TimePoint::nodes);

        register_vector<TimePoint>("vector<TimePoint>");
        register_vector<NodeData>("vector<NodeData>");
        register_vector<MessageCount>("vector<MessageCount>");
        register_vector<Message>("vector<Message>");

        emscripten::function("random_test", &random_test);
}