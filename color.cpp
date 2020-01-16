#include <vector>

using namespace std;

bool bpm(vector<vector<int>>& bpGraph, int u, vector<bool>& seen, vector<int>& matchR)
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

vector<int> max_BPM(vector<vector<int>>& bp_graph)
{
    vector<int> matchR(bp_graph.size(), -1);

    for (int src = 0; src < bp_graph.size(); src++)
    {
        vector<bool> seen(bp_graph.size(), false);

        bpm(bp_graph, src, seen, matchR);
    }
    return matchR;
}

