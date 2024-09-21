// ** long long vs int

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> 
#include <unordered_map>

using namespace std;

struct Rabbit
{
    int dist;
    int r = 1;
    int c = 1;
    int cnt = 0;
    long long score = 0;
};

int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

int N, M, P, Q, order, K, S, L;
unordered_map<int, Rabbit> rb;
vector<int> moved(10000001, 0);
vector<int> diffScore(10000001, 0);
long long totalScore = 0;
vector<int> pidList;

struct moveCmp
{
    bool operator()(int& pid1, int& pid2)
    {
        Rabbit& a = rb[pid1];
        Rabbit& b = rb[pid2];
        if (a.cnt == b.cnt)
        {
            if (a.r + a.c == b.r + b.c)
            {
                if (a.r == b.r)
                {
                    if (a.c == b.c)
                    {
                        return pid1 > pid2;
                    }
                    return a.c > b.c;
                }
                return a.r > b.r;
            }
            return a.r + a.c > b.r + b.c;
        }
        return a.cnt > b.cnt;
    }
};

struct Spot
{
    int r, c;
};

struct moveSpotCmp
{
    bool operator()(Spot& a, Spot& b) {
        if (a.r + a.c == b.r + b.c)
        {
            if (a.r == b.r)
            {
                return a.c < b.c;
            }
            return a.r < b.r;
        }
        return a.r + a.c < b.r + b.c;
    }
};

struct addCmp
{
    bool operator()(int& pid1, int& pid2) {
        Rabbit& a = rb[pid1];
        Rabbit& b = rb[pid2];
        if (a.r + a.c == b.r + b.c)
        {
            if (a.r == b.r)
            {
                if (a.c == b.c)
                {
                    return pid1 < pid2;
                }
                return a.c < b.c;
            }
            return a.r < b.r;
        }
        return a.r + a.c < b.r + b.c;
    }
};

inline int calculate(int r, int N) {
    r = ((r - 1) % (2 * (N - 1)) + 2 * (N - 1)) % (2 * (N - 1)) + 1;
    return r <= N ? r : 2 * N - r;
}

priority_queue<Spot, vector<Spot>, moveSpotCmp> ms_pq;
void move(Rabbit& rb) {
    int r = rb.r;
    int c = rb.c;
    int dist = rb.dist;
    ms_pq = priority_queue<Spot, vector<Spot>, moveSpotCmp>();
    for (int d = 0; d < 4; d++)
    {
        int nr = calculate(r + dr[d] * dist, N);
        int nc = calculate(c + dc[d] * dist, M);
        ms_pq.push({ nr, nc });
    }
    rb.r = ms_pq.top().r;
    rb.c = ms_pq.top().c;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> Q;

    priority_queue<int, vector<int>, moveCmp> m_pq;
    priority_queue<int, vector<int>, addCmp> s_pq;

    for (int n = 1; n <= Q; n++)
    {
        cin >> order;
        switch (order)
        {
        case 100: {
            cin >> N >> M >> P;
            int pid, dist;
            for (int i = 0; i < P; i++)
            {
                cin >> pid >> dist;
                pidList.push_back(pid);
                rb[pid].dist = dist;
                rb[pid].score = 0;
                m_pq.push(pid);
            }
            break;
        }
        case 200: {
            cin >> K >> S;

            s_pq = priority_queue<int, vector<int>, addCmp>();

            for (int t = 1; t <= K; t++)
            {
                int pid = m_pq.top();
                m_pq.pop();
                move(rb[pid]);
                rb[pid].cnt++;
                int addScore = rb[pid].r + rb[pid].c;

                if (rb[pid].r <= 0 || rb[pid].c <= 0) {
                    cout << rb[pid].r << ", " << rb[pid].c << endl;
                    return 0;
                }

                totalScore += addScore;
                diffScore[pid] += addScore;

                m_pq.push(pid);
                s_pq.push(pid);
                moved[pid] = n;
            }

            int pid = s_pq.top();
            rb[pid].score += S;

            break;
        }
        case 300: {
            int id;
            cin >> id >> L;
            rb[id].dist *= L;
            break;
        }
        case 400: {
            long long maxScore = 0;
            for (int i = 0; i < P; i++)
            {
                //cout << "========\n";
                //cout << "rb[pidList[i]].score: " << rb[pidList[i]].score << endl;
                //cout << "diffScore[pidList[i]: " << diffScore[pidList[i]] << endl;
                //cout << "totalScore: " << totalScore << endl;
                rb[pidList[i]].score += (totalScore - diffScore[pidList[i]]);
            }
            for (int i = 0; i < P; i++)
            {
                maxScore = max(maxScore, rb[pidList[i]].score);
            }
            cout << maxScore << '\n';
            break;
        }
        default:
            break;
        }
    }
    return 0;
}