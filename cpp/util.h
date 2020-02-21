#ifndef CONGESTED_CLIQUE_ROUTING_UTIL_H
#define CONGESTED_CLIQUE_ROUTING_UTIL_H

template<typename FUNCTION>
inline void loop(int n, FUNCTION f) {
    for (int i = 0; i < n; ++i) {
        f(i);
    }
}

#endif //CONGESTED_CLIQUE_ROUTING_UTIL_H
