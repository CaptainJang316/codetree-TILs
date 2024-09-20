// 3:05 ~ 5:25, 7:40 ~ 

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> 

using namespace std;

struct Rabbit
{
    int id;
    int dist;
    int r = 1;
    int c = 1;
    int cnt = 0;
    int score = 0;
};

int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

int N, M, P, Q, order, K, S, L;
vector< Rabbit> rb(2001);

// 1. 현재까지의 총 점프 횟수가 적은 토끼, 
// 2. 현재 서있는 행 번호 + 열 번호가 작은 토끼, 
// 3. 행 번호가 작은 토끼, 
// 4. 열 번호가 작은 토끼, 
// 5. 고유번호가 작은 토끼 
struct moveCmp
{
    bool operator()(int &a, int &b) {
        if (rb[a].cnt == rb[b].cnt)
        {
            if (rb[a].r + rb[a].c == rb[b].r + rb[b].c)
            {
                if (rb[a].r == rb[b].r)
                {
                    if (rb[a].c == rb[b].c)
                    {
                        return rb[a].id > rb[b].id;
                    }
                    return rb[a].c > rb[b].c;
                }
                return rb[a].r > rb[b].r;
            }
            return rb[a].r + rb[a].c > rb[b].r + rb[b].c;
        }
        return rb[a].cnt > rb[b].cnt;
    }
};

struct Spot
{
    int r, c;
};

// 1. 행 번호 + 열 번호가 큰 칸, 
// 2. 행 번호가 큰 칸, 
// 3. 열 번호가 큰 칸 
struct moveSpotCmp
{
    bool operator()(Spot &a, Spot &b) {
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

// 1. 현재 서있는 행 번호 + 열 번호가 큰 토끼, 
// 2. 행 번호가 큰 토끼, 
// 3. 열 번호가 큰 토끼, 
// 4. 고유번호가 큰 토끼 
struct addCmp
{
    bool operator()(int &id1, int &id2) {
        Rabbit a = rb[id1];
        Rabbit b = rb[id2];
        if (a.r + a.c == b.r + b.c)
        {
            if(a.r == b.r)
            {
                if (a.c == b.c)
                {
                    return a.id < b.id;
                }
                return a.c < b.c;
            }
            return a.r < b.r;
        }
        return a.r + a.c < b.r + b.c;
    }
};

bool isOutOfBound_R(int r) {
    return r < 1 || N < r;
}

bool isOutOfBound_C(int c) {
    return c < 1 || M < c;
}

int calculate(int r, int N) {
    int v;
    if (r > N)
    {
        v = r - N;
        if (v / (N - 1) % 2 == 0)
        {
            int diffV = v % (N - 1);
            if (diffV == 0) diffV = N - 1;

            return N - diffV;
        }
        else
        {
            int addV = v % (N - 1);
            if (addV == 0) addV = N - 1;

            return 1 + addV;
        }
    }
    else if (r < 1) {
        if (( - 1 * r / (N - 1)) % 2 == 0)
        {
            int addV = -1 * r % (N - 1);
            return addV + 2;
        }
        else
        {
            int addV = r % (N - 1);
            return N - 1 + addV;
        }
    }
    else return r;
}

void move(Rabbit& rb) {
    int r = rb.r;
    int c = rb.c;
    int dist = rb.dist;
    priority_queue< Spot, vector<Spot>, moveSpotCmp> ms_pq;
    //cout << "** id: " << rb.id << endl;
    for (int d = 0; d < 4; d++)
    {
        int nr = r + (dr[d] * dist);
        int nc = c + (dc[d] * dist);

        nr = calculate(nr, N);
        nc = calculate(nc, M);
        ms_pq.push({ nr, nc });
    }
    int br = ms_pq.top().r;
    int bc = ms_pq.top().c;
    rb.r = br;
    rb.c = bc;
}

int main() {
    cin >> Q;
    
    for (int n = 0; n < Q; n++)
    {
        cin >> order;
        switch (order)
        {
        case 100:
            cin >> N >> M >> P;
            for (int i = 1; i <= P; i++)
            {
                cin >> rb[i].id >> rb[i].dist;
            }

            break;
        case 200: {
            cin >> K >> S;
            for (int i = 1; i <= P; i++)
            {
                rb[i].cnt = 0;
            }

            for (int t = 1; t <= K; t++)
            {
                priority_queue<int, vector<int>, moveCmp> m_pq;
                for (int i = 1; i <= P; i++)
                {
                    m_pq.push(i);
                }
                int id = m_pq.top();
                move(rb[id]);
                rb[id].cnt += 1;
                int addScore = rb[id].r + rb[id].c;
                for (int i = 1; i <= P; i++)
                {
                    if (i == id) continue;

                    rb[i].score += addScore;
                }
            }
            priority_queue<int, vector<int>, addCmp> s_pq;
            for (int i = 1; i <= P; i++)
            {
                s_pq.push(i);
            }
            while (true)
            {
                int id = s_pq.top();
                if (rb[id].cnt != 0)
                {
                    rb[id].score += S;
                    break;
                }
                s_pq.pop();
            }

            break;
        }
        case 300:
            int id;
            cin >> id >> L;
            rb[id].dist *= L;
            break;

        case 400: {
            int maxScore = 0;
            for (int i = 1; i <= P; i++)
            {
                maxScore = max(maxScore, rb[i].score);
            }

            cout << maxScore << '\n';
            break;
        }        default:
            break;
        }
    }
    // 여기에 코드를 작성해주세요.
    return 0;
}