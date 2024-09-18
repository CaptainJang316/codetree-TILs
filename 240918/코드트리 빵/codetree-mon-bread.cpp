// 12:57 ~ 1:02, 1:31 ~ 3:13, 11:50 ~ 12:05, 12:43 ~ 1:56 <-- 3시간 20분 정도 소요.. 그마저도 틀린 테케가 뭔지 알 수 있었던 덕분;;
// => 다시 풀어볼 것!

// ** input으로 받는 r,c값이 0부터인지 1부터인지 확인해야 함.. 나도 알고 싶지 않았음ㅋㅋㅋ...

// ** 온갖 종류의 BFS를 다 모아놓은 문제

// ** 도착지 기준으로 가까운 곳으로 '한칸만 이동하되', 같은 거리는 방향 우선순위 있는 경우
//    => 우선순위 방향대로, 처음 출발한 방향을 계속 r,c와 함께 cd 값으로 q에 계속 넘겨준다. <- 가장 먼저 발견되는 경우의 cd방향으로 결정
//       (이를 위해 처음 nr들에 대해선 별도의 for문 처리ㅇㅇ)

// ** 목적지 기준으로 가장 가까운 출발지 후보들 목록을 우선순위로 지정하려 했으나, 이 문제에선 잘못된 방법이었다.
//    (아직 map이 깨끗한 최초 상태 기준으로 가까워도, 로직이 진행되며 길이 막히게 되거나 돌아가야 하게 바뀔 수 있는 요소 존재)
//    => 그냥 매번 목적지 기준으로 가장 가까운 출발지(baseCamp)를 찾아야 한다. <-- 매번 실행되는 게 아니라, 각 사람마다 1번씩만 시행되므로, 괜찮다!
//    (즉, 문제의 로직 구조를 이해하고 이에 맞는 방식으로 구현할 것ㅇㅇㅇ)

// ** 한칸 이동 말고, 최단거리 목적지만 찾되 같은 거리에 있는 경우, 행과 열의 크고 작음으로 우선순위 두는 경우
//    => 무조건 depth 써야 한다. 방향 설정만으론 절대 안된다.(이게 되는 건 한칸만 이동하는 경우ㅇㅇ)
//       depth가 같은 것들을 pq에 넣고 첫번째 것만 꺼내서 그걸로 선택하면 됨. 

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct SpotInfo
{
    int r, c, v;
};

struct Spot
{
    int r, c;
};

struct Person
{
    int r, c;
    bool isArrive = false;
};

int dr[] = { -1, 0, 0, 1 };
int dc[] = { 0, -1, 1, 0 };

int n, m;
vector<vector<int>> map(16, vector<int>(16));
queue< SpotInfo> aq;
vector< Person> p(31);
vector< Spot> mk(31);

bool isOutOfBound(int r, int c) {
    return r <= 0 || n < r || c <= 0 || n < c;
}

// ** 이렇게 각 목적지별로 가까운 bc들을 미리 찾아놓으려 했으나. 결과적으로 생각해보면, 어차피 시간 소요는 같음(각 사람마다 최초 1번만 시행되므로ㅇㅇ)
//    그리고, 맵의 이동 불가 영역들이 점점 추가되는 로직에선, 이는 틀린 방법일 수밖에 없음.(최초 상태 기준으로 순위를 매겼으므로ㅇㅇ)
//void getBaseCampDistBFS() {
//    queue<Spot> q;
//    vector<vector<int>> visited(16, vector<int>(16, 0));
//    for (int i = 1; i <= m; i++)
//    {
//        q.push({ mk[i].r, mk[i].c });
//        visited[mk[i].r][mk[i].c] = i;
//
//        while (!q.empty())
//        {
//            int r = q.front().r;
//            int c = q.front().c;
//            q.pop();
//
//            for (int d = 0; d < 4; d++)
//            {
//                int nr = r + dr[d];
//                int nc = c + dc[d];
//
//                if (isOutOfBound(nr, nc)) continue;
//
//                if (map[nr][nc] == 1 && visited[nr][nc] != i)
//                {
//                    nBC[i].push_back({ nr, nc });
//                    visited[nr][nc] = i;
//                    q.push({ nr, nc });
//                }
//                else if(visited[nr][nc] != i)
//                {
//                    visited[nr][nc] = i;
//                    q.push({ nr, nc });
//                }
//            }
//        }
//    }
//}

void moveBFS(int i, vector<vector<int>> &visited) {
    queue<SpotInfo> q;
    int r = p[i].r;
    int c = p[i].c;
    for (int d = 0; d < 4; d++)
    {
        int nr = r + dr[d];
        int nc = c + dc[d];

        if (isOutOfBound(nr, nc)) continue;

        if (nr == mk[i].r && nc == mk[i].c)
        {
            p[i].isArrive = true;
            p[i].r = nr;
            p[i].c = nc;
            aq.push({ nr,nc,i });
            return;
        }
        else if (map[nr][nc] <= 1) {
            q.push({ nr, nc, d });
            visited[nr][nc] = i;
        }
    }

    while (!q.empty())
    {
        int r = q.front().r;
        int c = q.front().c;
        int cd = q.front().v;
        q.pop();
        for (int d = 0; d < 4; d++)
        {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (isOutOfBound(nr, nc)) continue;

            if (nr == mk[i].r && nc == mk[i].c)
            {
                p[i].r += dr[cd];
                p[i].c += dc[cd];
                return;
            }
            else if (map[nr][nc] <= 1 && visited[nr][nc] != i) {
                q.push({ nr, nc, cd });
                visited[nr][nc] = i;
            }
        }
    }
}

struct cmp
{
    bool operator()(Spot& a, Spot& b) {
        if (a.r == b.r) return a.c > b.c;
        return a.r > b.r;
    }
};

void selectNearBaseCampBFS(int idx) {
    vector<vector<int>> visited(16, vector<int>(16, 0));
    queue<SpotInfo> q;
    priority_queue<Spot, vector<Spot>, cmp> bc_pq;
    q.push({ mk[idx].r, mk[idx].c, 0 });
    visited[mk[idx].r][mk[idx].c] = 1;
    int minDepth = 100000;

    while (!q.empty())
    {
        int r = q.front().r;
        int c = q.front().c;
        int depth = q.front().v;
        q.pop();

        // ** 거리가 같은 경우, r&c 값 기준으로 우선순위 정해야 할 때,
        //    이렇게 minDepth 처리하면 된다.
        if (minDepth < depth) break; 

        for (int d = 0; d < 4; d++)
        {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (isOutOfBound(nr, nc)) continue;
            
            if (map[nr][nc] == 1)
            {
                minDepth = min(depth, minDepth); // <<-- 여기!!!!!
                bc_pq.push({ nr,nc });
                visited[nr][nc] = 1;
            }
            else if (map[nr][nc] <= 1 && visited[nr][nc] != 1) {
                q.push({ nr, nc, depth + 1 });
                visited[nr][nc] = 1;
            }
        }
    }

    int bc_r = bc_pq.top().r; // <<-- 여기!!!!!
    int bc_c = bc_pq.top().c;
    bc_pq.pop();
    p[idx].r = bc_r;
    p[idx].c = bc_c;
    map[bc_r][bc_c] = idx + 1;
    return;
}

int main() {
    cin >> n >> m;
    for (int r = 1; r <= n; r++)
    {
        for (int c = 1; c <= n; c++)
        {
            cin >> map[r][c];
        }
    }

    for (int i = 1; i <= m; i++)
    {
        cin >> mk[i].r >> mk[i].c;
        p[i].isArrive = false;
    }

    int t = 1;
    while (true)
    {
        bool isEnd = true;
        vector<vector<int>> visited(16, vector<int>(16, 0));
        if (t < m)
        {
            for (int i = 1; i <= t; i++)
            {
                if (p[i].isArrive) continue;
                isEnd = false;
                moveBFS(i, visited);
            }
        }
        else
        {
            for (int i = 1; i <= m; i++)
            {
                if (p[i].isArrive) continue;
                isEnd = false;
                moveBFS(i, visited);
            }
        }

        while (!aq.empty())
        {
            int r = aq.front().r;
            int c = aq.front().c;
            int idx = aq.front().v;
            aq.pop();

            map[r][c] = idx + 1;
        }

        if (t <= m)
        {
            isEnd = false;
            selectNearBaseCampBFS(t);
        }
        if (isEnd) {
            t--;
            break;
        }
        t++;
    }
    cout << t << '\n';
    return 0;
}