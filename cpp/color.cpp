#include "node.h"

using namespace std;

bool bpm(Vec2<int>& bpGraph, int u, Vec<bool>& seen, Vec<int>& matchR)
{
    for (int v = 0; v < bpGraph.size(); v++)
    {
        if (bpGraph[u][v] > 0 && !seen[v])
        {
            seen[v] = true;

            if (matchR[v] < 0 || bpm(bpGraph, matchR[v],
                                     seen, matchR))
            {
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}

Vec<int> reverse(Vec<int> matchR) {
    Vec<int> matchL(matchR.size(), -1);

    for (int i = 0; i < matchR.size(); i++) {
        if (matchR[i] != -1) {
            matchL[matchR[i]] = i;
        }
    }
    return matchL;
}

Vec<int> max_BPM(Vec2<int>& bp_graph)
{
    Vec<int> matchR(bp_graph.size(), -1);

    for (int src = 0; src < bp_graph.size(); src++)
    {
        Vec<bool> seen(bp_graph.size(), false);

        bpm(bp_graph, src, seen, matchR);
    }
    return reverse(matchR);
}

