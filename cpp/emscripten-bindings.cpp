#include "helper-data-structs.h"
#include "test.h"
#include "time-point.h"
#include <emscripten/bind.h>

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
        register_vector<int>("Vec1");
        register_vector<std::vector<int>>("Vec2");

        emscripten::function("random_test", &random_test);
        emscripten::function("test1", &test1);
        emscripten::function("test2", &test2);
        emscripten::function("test3", &test3);
        emscripten::function("test4", &test4);
        emscripten::function("test5", &test5);
        emscripten::function("test6", &test6);
        emscripten::function("test7", &test7);
        emscripten::function("uniform_subset_test", &uniform_subset_test);
        emscripten::function("custom_test", &custom_test);
}
