#ifndef INC_4_CYCLE_DETECTION_NODE_H
#define INC_4_CYCLE_DETECTION_NODE_H

#include <vector>

template<class T>
using Vec = std::vector<T>;
template<class T>
using Vec2 = std::vector<std::vector<T>>;

namespace four_cycle_detection {
    struct Tile {
        int a_start;
        int a_end;
        int b_start;
        int b_end;
        bool taken;

        Tile(int a_start, int a_end, int b_start, int b_end) {
            this->a_start = a_start;
            this->a_end = a_end;
            this->b_start = b_start;
            this->b_end = b_end;
            this->taken = false;
        }
    };

    struct Message {
        int src;
        int value;

        Message(int src, int value) {
            this->src = src;
            this->value = value;
        }
    };

    class Node {
        public:
            explicit Node(int id, Vec<int> neighbours) {
                this->id = id;
                this->neighbours = neighbours;
            }

            void set_nodes(Vec<std::shared_ptr<Node>> &nodes_to_set);

            void broadcast_degree();
            void send_A_partition_to_a();
            void send_A_partition_to_b();
            void check_local_4cycles(); // the last step of the 2nd part of the algorithm

            int get_2walk_count();
            int get_id();

            void messages_to_neighbours();
            void compute_tiles();
            void compute_W_y_b();
            void compute_W_b();

            void add_message(std::unique_ptr<Message> m);
        private:
            int id;
            Vec<std::weak_ptr<Node>> nodes;
            Vec<int> node_degrees;
            Vec<std::unique_ptr<Message>> messages;

            Vec<int> neighbours;

            Vec2<int> setsA;
            Vec2<int> setsB;

            int highestPowerOf2(int n);
    };
}

#endif //INC_4_CYCLE_DETECTION_NODE_H
