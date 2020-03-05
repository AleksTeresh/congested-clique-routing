#ifndef CONGESTED_CLIQUE_ROUTING_CLIQUE_ROUTER_H
#define CONGESTED_CLIQUE_ROUTING_CLIQUE_ROUTER_H

#include "node.h"
#include "time-point.h"

class CliqueRouter {
private:
    int set_size = 0;
    std::vector<TimePoint> history;

    void init(Vec<std::shared_ptr<Node>>& nodes);
    void init_history(Vec<std::shared_ptr<Node>>& nodes);
    void update_history(Vec<std::shared_ptr<Node>>& nodes);
    void step2(Vec<std::shared_ptr<Node>>& nodes);
    void step3(Vec<std::shared_ptr<Node>>& nodes);
    void check_step3_round2_result(Vec<std::shared_ptr<Node>>& nodes);
    void move_messages_between_sets(Vec<std::shared_ptr<Node>>& nodes);
    void move_messages_within_sets(Vec<std::shared_ptr<Node>>& nodes);
    void check_step_2_precondition(Vec<std::shared_ptr<Node>>& nodes);
    void check_step_3_precondition(Vec<std::shared_ptr<Node>>& nodes);
    void check_step_4_precondition(Vec<std::shared_ptr<Node>>& nodes);
    void check_step_5_precondition(Vec<std::shared_ptr<Node>>& nodes);

public:
    void route(Vec<std::shared_ptr<Node>>& nodes);
    std::vector<TimePoint>& get_history();
};


#endif //CONGESTED_CLIQUE_ROUTING_CLIQUE_ROUTER_H
