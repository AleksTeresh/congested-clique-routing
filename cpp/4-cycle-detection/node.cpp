#include "node.h"
#include <cmath>

using namespace std;

namespace four_cycle_detection {

    void Node::add_message(unique_ptr<Message> m) {
        messages.push_back(move(m));
    }

    void Node::set_nodes(Vec<std::shared_ptr<Node>> &nodes_to_set) {
        nodes = Vec<weak_ptr<Node>>();
        for (const auto& node : nodes_to_set) {
            nodes.push_back(node);
        }
    }

    void Node::messages_to_neighbours() {
        node_degrees.resize(nodes.size());
        for (auto i = messages.begin(); i != messages.end();) {
            auto& message = *i;
            node_degrees[message->src] = message->value;
            i = messages.erase(i);
        }
    }

    void Node::broadcast_degree() {
        for (int i = 0; i < nodes.size(); i++) {
            auto message = make_unique<Message>(this->id, neighbours.size());
            nodes[i].lock()->add_message(move(message));
        }
    }

    void Node::compute_tiles() {
        Vec<int> fs;
        for (int i = 0; i < node_degrees.size(); i++) {
            fs.push_back(highestPowerOf2(node_degrees[i] / 4));
        }
        int k = highestPowerOf2(nodes.size());

        int i = 0;
        setsA.resize(fs.size(), Vec<int>());
        setsB.resize(fs.size(), Vec<int>());
        Vec<Tile> tiles = {{ 0, k, 0, k }};

        while (true) {
            Vec<int> fsOfRightSizeCount;
            int tileSize = k / pow(2, i);
            for (int j = 0; j < fs.size(); j++) {
                if (fs[j] == tileSize) {
                    fsOfRightSizeCount.push_back(j);
                }
            }

            int tileCount = tiles.size();
            // divide each empty tile to 4 smaller tiles
            for (int j = 0; j < tileCount; j++) {
                if (!tiles[j].taken) {
                    int new_a_end = (tiles[j].a_start + tiles[j].a_end) / 2;
                    int new_b_end = (tiles[j].b_start + tiles[j].b_end) / 2;
                    int old_a_end = tiles[j].a_end;
                    int old_b_end = tiles[j].b_end;
                    tiles.push_back({ tiles[j].a_start, new_a_end, new_b_end, old_b_end });
                    tiles.push_back({ new_a_end, old_a_end, tiles[j].b_start, new_b_end });
                    tiles.push_back({ new_a_end, old_a_end, new_b_end, old_b_end });
                    tiles[j].a_end = new_a_end;
                    tiles[j].b_end = new_b_end;
                }
            }

            for (int fIdx = 0; fIdx < fsOfRightSizeCount.size(); fIdx++) {
                int y = fsOfRightSizeCount[fIdx];
                for (int j = 0; j < tiles.size(); j++) {
                    if ((tiles[j].a_start - tiles[j].a_end) == tileSize && !tiles[j].taken) {
                        tiles[j].taken = true;
                        for (int e = tiles[j].a_start; e < tiles[j].a_end; e++) {
                            setsA[y].push_back(e);
                        }
                        for (int e = tiles[j].b_start; e < tiles[j].b_end; e++) {
                            setsB[y].push_back(e);
                        }
                    }
                }
            }

            if (tileSize == 0) break;
            i++;
        }
    }

    int Node::highestPowerOf2(int n)
    {
        int p = (int)log2(n);
        return (int)pow(2, p);
    }
}
