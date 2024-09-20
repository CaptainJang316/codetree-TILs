// 3:05 ~ 5:25, 7:40 ~ 

// (1) 경주 시작 준비

// P 마리의 토끼가 N×M 크기의 격자 위에서 경주를 진행할 준비를 합니다. (1,1) ~ 

// 각 토끼에게는 '고유한 번호'가 붙어있으며, '한번 움직일 시 꼭 이동해야 하는 거리'도 정해져 있습니다.

// 처음 토끼들은 전부 (1행, 1열)에 있습니다.


// (2) 경주 진행

// 가장 우선순위가 높은 토끼를 뽑아 멀리 보내주는 것을 K번 반복합니다.

// 우선순위는 순서대로 
// 1. 현재까지의 총 점프 횟수가 적은 토끼, 
// 2. 현재 서있는 행 번호 + 열 번호가 작은 토끼, 
// 3. 행 번호가 작은 토끼, 
// 4. 열 번호가 작은 토끼, 
// 5. 고유번호가 작은 토끼 
// 순입니다. 
// 첫 번째 우선순위가 높은 토끼가 한마리 뿐이라면 바로 결정되는 것이고, 동률이라면 두 번째 우선순위를 보고, .. 이러한 규칙에 의해 가장 우선순위가 높은 토끼가 결정됩니다.

// 우선순위가 가장 높은 토끼가 결정이 되면 이 토끼를 i번 토끼라 했을 때, 상하좌우 네 방향으로 각각 d만큼 이동했을 때의 위치를 구합니다.
// 이때 이동하는 도중 '그 다음 칸이' 격자를 벗어나게 된다면, (현재 위치에서) '방향을 반대로 바꿔' 한 칸 이동하게 됩니다.
// 
// 이렇게 구해진 4개의 위치 중
// 
// 1. 행 번호 + 열 번호가 큰 칸, 
// 2. 행 번호가 큰 칸, 
// 3. 열 번호가 큰 칸 
// 순으로 우선순위를 두었을 때 가장 우선순위가 높은 칸을 골라 그 위치로 해당 토끼를 이동시킵니다.
// 이 칸의 위치를 (r, c)라 했을 때 i번 토끼를 제외한 나머지 P−1마리의 토끼들은 전부 r+ c​ 만큼의 점수를 동시에 얻게 됩니다.

// 이렇게 K번의 턴 동안 가장 우선순위가 높은 토끼를 뽑아 멀리 보내주는 것을 반복하게 되며, 이 과정에서 '동일한 토끼가 여러번 선택'되는 것 역시 '가능합니다'.

// K번의 턴이 모두 진행된 직후에는 
// 1. 현재 서있는 행 번호 + 열 번호가 큰 토끼, 
// 2. 행 번호가 큰 토끼, 
// 3. 열 번호가 큰 토끼, 
// 4. 고유번호가 큰 토끼 
// 순으로 우선순위를 두었을 때 가장 우선순위가 높은 토끼를 골라 '점수 S를 더해주게 됩니다'. 
// 
// 단, 이 경우에는 K번의 턴 동안 한번이라도 뽑혔던 적이 있던 토끼 중 가장 우선순위가 높은 토끼를 골라야만 함에 꼭 유의합니다.


// (3) 이동거리 변경
// 고유번호가 pid인 토끼의 이동거리를 L배 해줍니다. 단, 계산 도중 특정 토끼의 이동거리가 10억을 넘어가는 일은 발생하지 않음을 가정해도 좋습니다.

// (4) 최고의 토끼 선정
// 각 토끼가 모든 경주를 진행하며 얻은 점수 중 가장 높은 점수를 출력합니다.

// Q번에 걸쳐 명령을 순서대로 진행하며 최고의 토끼를 선정해주는 프로그램을 작성해보세요.


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

// 우선순위가 가장 높은 토끼가 결정이 되면 이 토끼를 i번 토끼라 했을 때, 상하좌우 네 방향으로 각각 d만큼 이동했을 때의 위치를 구합니다.
// 이때 이동하는 도중 '그 다음 칸이' 격자를 벗어나게 된다면, (현재 위치에서) '방향을 반대로 바꿔' 한 칸 이동하게 됩니다.
// 
// 이렇게 구해진 4개의 위치 중
// 
// 1. 행 번호 + 열 번호가 큰 칸, 
// 2. 행 번호가 큰 칸, 
// 3. 열 번호가 큰 칸 
// 순으로 우선순위를 두었을 때 가장 우선순위가 높은 칸을 골라 그 위치로 해당 토끼를 이동시킵니다.
// 이 칸의 위치를 (r, c)라 했을 때 i번 토끼를 제외한 나머지 P−1마리의 토끼들은 전부 r+ c​ 만큼의 점수를 동시에 얻게 됩니다.
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

        //int rv = nr % (N - 1);
        //int cv = nc % (M - 1);
        //cout << "1 nr: " << nr << ", nc: " << nc << endl;
        nr = calculate(nr, N);
        nc = calculate(nc, M);

        
        //if ((nr / (N - 1)) % 2 == 0) nr = N - rv;
        //else nr = 1 + rv;

        //if ((nc / (M - 1)) % 2 == 0) nc = M - cv;
        //else nc = 1 + cv;

        //if (isOutOfBound_R(nr))
        //{
        //    if (nr < 1) nr = 2;
        //    else nr = N - 1;
        //}
        //if (isOutOfBound_C(nc))
        //{
        //    if (nc < 1) nc = 2;
        //    else nc = M - 1;
        //}
        //cout << "2 nr: " << nr << ", nc: " << nc << endl;
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
                //cout << "rb[id].r: " << rb[id].r << ", rb[id].c: " << rb[id].c << endl;
                for (int i = 1; i <= P; i++)
                {
                    if (i == id) continue;

                    rb[i].score += addScore;
                }

                //cout << "====" << t << "====" << endl;
                //for (int r = 1; r <= 3; r++)
                //{
                //    for (int c = 1; c <= 5; c++)
                //    {
                //        cout << map[r][c] << ' ';
                //    }
                //    cout << endl;
                //}
                //for (int i = 1; i <= P; i++)
                //{
                //    cout << i << ".score: " << rb[i].score << endl;
                //}
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
            // K번의 턴이 모두 진행된 직후에는 
            // 1. 현재 서있는 행 번호 + 열 번호가 큰 토끼, 
            // 2. 행 번호가 큰 토끼, 
            // 3. 열 번호가 큰 토끼, 
            // 4. 고유번호가 큰 토끼 
            // 순으로 우선순위를 두었을 때 가장 우선순위가 높은 토끼를 골라 '점수 S를 더해주게 됩니다'. 
            // 
            // 단, 이 경우에는 K번의 턴 동안 한번이라도 뽑혔던 적이 있던 토끼 중 가장 우선순위가 높은 토끼를 골라야만 함에 꼭 유의합니다.

            break;
        }
        // (3) 이동거리 변경
        // 고유번호가 pid인 토끼의 이동거리를 L배 해줍니다. 단, 계산 도중 특정 토끼의 이동거리가 10억을 넘어가는 일은 발생하지 않음을 가정해도 좋습니다.

        // (4) 최고의 토끼 선정
        // 각 토끼가 모든 경주를 진행하며 얻은 점수 중 가장 높은 점수를 출력합니다.
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