// 12:40 ~ 

// 고객의 만족을 위해 여행사는 다양한 여행 상품을 관리하여 최적의 여행 경험을 제공하려고 합니다.

// 코드트리 랜드는 아래와 같이 n 개의 도시와 각 도시 사이를 연결하는 m 개의 간선으로 이루어져 있습니다. 
// 각 도시는 0 번부터 n−1 번까지 번호가 붙여져 있으며, 각 간선은 '방향성을 갖지 않습니다'. 
// 또, 두 도시 사이를 연결하는 간선은 '여러 개가 존재할 수 있으며,' 자기 자신을 향하는 간선 또한 '존재할 수 있습니다'.

// 코드트리 여행사는 항상 여행 상품의 '출발지를 하나로 통일'하여 관리합니다. 처음 코드트리 여행 상품의 출발지는 0번 도시입니다.

// 코드트리 투어는 5가지 명령에 따라 진행됩니다.

// (1) 코드트리 랜드 건설

// 코드트리 랜드 관련 정보가 주어집니다. 
// 도시의 수 n 과 간선의 수 m, 그리고 m 개의 간선에 해당하는 정보 v, u, w가 주어집니다. 
// 도시 v와 도시 u는 가중치가 w인 간선으로 직접 연결되어 있음을 의미합니다
// 
// (2) 여행 상품 생성


// 같은 값을 가지는 상품이 여러 개 있을 경우 id 가 가장 작은 상품을 선택합니다.

// 코드트리는 판매 가능한 상품 중 가장 우선순위가 높은 상품을 1개를 판매하게 되며, 이 상품의 id 를 출력한 뒤 이 상품을 관리 목록에서 제거합니다. 

// 여행 상품의 출발지를 전부 s로 변경하는 명령입니다. 출발지가 변경됨에 따라 각 상품의 cost가 변경될 수 있음에 유의하세요.

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
    bool isExist = false;
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
            cin >> id >> r >> d;
            idList.push_back(id);
            p[id].r = r;
            p[id].dest = d;
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
                id = idList[i];
                if (p[id].isDeleted) continue;

                r = p[id].r;
                d = p[id].dest;

                v = r - dist[d];

                if (maxV < v)
                {
                    maxV = v;
                    best_id = id;
                }
                else if(maxV == v)
                {
                    best_id = min(best_id, id);
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