// 12:40 ~ 2:43, 11:30 ~ 12:50 <-- 약 3시간 반 소요...;;


// ** 출발지가 계속 바뀔 수 있는 경우, 그 때마다 새로 계산하는 건 굉장히 시간 소모가 커질 수 있다.
//    => 이미 구한 적 있는 방향의 경우엔 그대로 사용할 수 있도록 해당 출발지 기준으로의 거리들 저장..!!!(2차원 dist!!)

// ** 각 명령어마다 반복하는 횟수가 정해져 있는 경우, 각 명령어의 시간복잡도 보다 해당 명령 수가 월등히 차이가 나지는 않는지 확인하고,
//    만약 그렇다면, 많이 호출되는 명령에 대해 최대한 시간 복잡도를 줄이는 게 올바른 최적화 방향이다!
//    (이 문제의 경우, 우선순위 큐를 비우고 새로 업데이트하는 게 오히려 좋은 선택이었다. 반복 횟수 자체가 비교 불가 수준으로 차이가 남)
//    (우선순위 큐 업데이트 15번 vs 탐색 30000번ㅇㅇ..)

// ** 본인 자신에게 돌아오는 간선이 있다고 해도, 결국 dist는 최단거리를 구하는 것인 만큼, 특별한 조건이 없다면 걍 출발지 cost는 0으로 설정할 것!

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

// ** 1≤w≤100 & 1≤m≤10,000,
//    => 100 * 10000 = 1000000
//#define INF (~0U>>2)
#define INF 1000000

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

struct PackageInfo
{
    int id, r, dest;
};

int Q;
//vector<int> dist(2000, INF); // 0 ~ n-1
vector<vector<int>> dist(2000, vector<int>(2000, INF)); // 0 ~ n-1
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

struct cmp2
{
    bool operator()(PackageInfo& a, PackageInfo& b) {
        int av = a.r - dist[s][a.dest];
        int bv = b.r - dist[s][b.dest];
        if (av == bv) return a.id > b.id;
        return av < bv;
    }
};

priority_queue<PackageInfo, vector<PackageInfo>, cmp2> p_pq;

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

    if (dist[s][s] != 0) 
    {
        dist[s][s] = 0;
        pq.push({ 0, s });

        while (!pq.empty())
        {
            int f = pq.top().f;
            int c = pq.top().c;
            pq.pop();

            if (dist[s][f] < c) continue;

            for (int i = 0; i < node[f].size(); i++)
            {
                int to = node[f][i].t;
                int nc = c + node[f][i].c;

                if (dist[s][to] > nc)
                {
                    dist[s][to] = nc;
                    pq.push({ nc, to });
                }
            }
        }
    }

    while (!p_pq.empty())
    {
        p_pq.pop();
    }

    for (int i = 0; i < idList.size(); i++)
    {
        int id = idList[i];
        if (p[id].isDeleted) continue;

        p_pq.push({ id, p[id].r, p[id].dest });
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
            p_pq.push({id, p[id].r, p[id].dest});

            break;

        case 300:
            cin >> id;
            p[id].isDeleted = true;

            break;

        case 400: {
            maxV = -1;
            bool isExist = false;
            while (!p_pq.empty())
            {
                int id = p_pq.top().id;
                int r = p_pq.top().r;
                int d = p_pq.top().dest;
                p_pq.pop();

                //cout << "id: " << id << ", r - dist[s][d]:" << r - dist[s][d] << '\n';

                if (p[id].isDeleted || p[id].r != r || p[id].dest != d) continue;

                isExist = true;
                maxV = r - dist[s][d];

                if (maxV >= 0)
                {
                    cout << id << '\n';
                    p[id].isDeleted = true;
                }
                else {
                    maxV = -1;
                    cout << maxV << '\n';
                }

                break;
            }
            if (!isExist)
            {
                maxV = -1;
                cout << maxV << '\n';
            }

            break;
        }
        case 500:
            cin >> s;
            setDist(s);

            break;
        default:
            break;
        }
    }
    return 0;
}