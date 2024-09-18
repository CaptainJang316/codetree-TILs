#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#define INF (~0U>>2)

using namespace std;

struct W1
{
    int c, f;
};

struct W2
{
    int c, t;
};

struct Package
{
    int r, dest;
    bool isDeleted = false;
};

int Q;
vector<int> dist(2000, INF); // 0 ~ n-1
vector<vector<W2>> node(10001);
vector<int> idList;
vector< Package> p(30001);
int n, m, v, u, w, order, id, r, d, s, maxV;

struct cmp
{
    bool operator()(W1& a, W1& b) {
        return a.c > b.c;
    }
};

priority_queue<W1, vector<W1>, cmp> pq;

void setDist(int s) {
    //bool isExist = false;
    //for (int i = 0; i < node[s].size(); i++)
    //{
    //    if (node[s][i].t == s)
    //    {
    //        dist[s] = node[s][i].c;
    //        pq.push({ node[s][i].c, s });
    //        isExist = true;
    //        break;
    //    }
    //}
    //if (!isExist) {
    //    dist[s] = 0;
    //    pq.push({ 0, s });
    //}
    dist[s] = 0;
    pq.push({ 0, s });

    while (!pq.empty())
    {
        int f = pq.top().f;
        int c = pq.top().c;
        pq.pop();

        if (dist[f] < c) continue;

        for (int i = 0; i < node[f].size(); i++)
        {
            int to = node[f][i].t;
            int nc = c + node[f][i].c;

            if (dist[to] > nc)
            {
                dist[to] = nc;
                pq.push({ nc, to });
            }
        }
    }
}

int main() {
    cin >> Q;
    for (int i = 0; i < Q; i++)
    {
        cin >> order;
        
        switch (order)
        {
        case 100:
            cin >> n >> m;
            for (int i = 0; i < m; i++)
            {
                cin >> v >> u >> w;
                node[v].push_back({ w, u });
                node[u].push_back({ w, v });
            }

            setDist(0);

            break;

        case 200:
            cin >> id;
            cin >> p[id].r >> p[id].dest;
            idList.push_back(id);
            p[id].isDeleted = false;

            break;

        case 300:
            cin >> id;
            p[id].isDeleted = true;

            break;

        case 400:
            maxV = -1;
            int id, v, best_id;
            for (int i = 0; i < idList.size(); i++)
            {
                if (p[idList[i]].isDeleted) continue;

                id = idList[i];
                r = p[id].r;
                d = p[id].dest;

                v = r - dist[d];

                if (maxV <= v)
                {
                    if (maxV == v)
                    {
                        best_id = min(best_id, id);
                    }
                    else
                    {
                        maxV = v;
                        best_id = id;
                    }
                }
            }

            if (maxV != -1)
            {
                cout << best_id << '\n';
                p[best_id].isDeleted = true;
            }
            else cout << maxV << '\n';

            break;

        case 500:
            cin >> s;

            fill(dist.begin(), dist.end(), INF);
            setDist(s);

            break;
        default:
            break;
        }
    }
    return 0;
}