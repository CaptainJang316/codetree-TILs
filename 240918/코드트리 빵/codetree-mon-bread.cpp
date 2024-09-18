// 12:57 ~ 1:02, 1:31 ~ 3:13, 11:50 ~ 12:05, 12:43 ~ 

// 빵을 구하고자 하는 m명의 사람이 있는데, 1번 사람은 정확히 1분에, 2번 사람은 정확히 2분에, ..., 
// m번 사람은 정확히 m 분에 각자의 베이스캠프에서 출발하여 편의점으로 이동하기 시작합니다. 
// 
// 사람들은 출발 시간이 되기 전까지 '격자 밖에 나와있으며', 사람들이 목표로 하는 편의점은 '모두 다릅니다'. 이 모든 일은 n*n 크기의 격자 위에서 진행됩니다.

// 코드트리 빵을 구하고 싶은 사람들은 다음과 같은 방법으로 움직입니다. 
// 이 3가지 행동은 '총 1분 동안 진행되며', 정확히 1, 2, 3 순서로 진행되어야 함에 유의합니다.

// 1. 격자에 있는 사람들 모두가 본인이 가고 싶은 편의점 방향을 향해서 1 칸 움직입니다. 
// '최단거리로 움직이며' 최단 거리로 움직이는 방법이 '여러가지라면' ↑, ←, →, ↓ 의 '우선 순위로' 움직이게 됩니다. 

// 2. 만약 편의점에 '도착한다면' 해당 편의점에서 '멈추게 되고', 이때부터 다른 사람들은 해당 편의점이 있는 칸을 '지나갈 수 없게 됩니다'. 
// ** 격자에 있는 사람들이 모두 이동한 '뒤에' 해당 칸을 지나갈 수 없어짐에 유의합니다. <-- 남은 사람들의 해당 이동 턴이 끝날 때까지는 통행 가능!

// 3. 현재 시간이 t분이고 t ≤ m를 만족한다면, t번 사람은 자신이 가고 싶은 편의점과 가장 가까이 있는 베이스 캠프에 들어갑니다. <-- 각 편의점별로 가장 가까운 베캠 정보 처음에 저장ㅇㅇ
// 여기서 가장 가까이에 있다는 뜻 역시 최단거리에 해당하는 곳을 의미합니다. 
// 가장 가까운 베이스캠프가 '여러 가지인 경우'에는 그 중 '행이 작은 베이스캠프', 행이 같다면 '열이 작은 베이스 캠프'로 들어갑니다. 
// t번 사람이 베이스 캠프로 이동하는 데에는 '시간이 전혀 소요되지 않습니다.'

// '이때부터' '다른 사람들은' 해당 베이스 캠프가 있는 칸을 '지나갈 수 없게 됩니다.' 
// t번 사람이 편의점을 향해 움직이기 시작했더라도 해당 베이스 캠프는 '앞으로 절대 지나갈 수 없음'에 유의합니다. 
// 마찬가지로 해당 턴 격자에 있는 사람들이 모두 이동한 뒤에 해당 칸을 지나갈 수 없어짐에 유의합니다.


// 이미 사람들이 도착한 편의점이나 출발한 적이 있는 베이스캠프의 경우 움직일 때 '절대 지나갈 수 없는 공간'임을 유의합니다. (예시에서는 빨간색으로 표시되어 있습니다.)

// 도달 못하는 경우는 없음.

// 또한, 이동하는 도중 동일한 칸에 둘 이상의 사람이 위치하게 되는 경우 역시 '가능함'에 유의합니다.


//첫 번째 줄에는 격자의 크기 n과 사람의 수 m이 공백을 사이에 두고 주어집니다.
//
//이후 n개의 줄에 걸쳐 격자의 정보가 주어집니다.각 줄에 각각의 행에 해당하는 n개의 수가 공백을 사이에 두고 주어집니다.
//0의 경우에는 빈 공간, 1의 경우에는 베이스캠프를 의미합니다.
//
//이후 m개의 줄에 걸쳐 각 사람들이 가고자 하는 편의점 위치의 행 x, 열 y의 정보가 공백을 사이에 두고 주어집니다.
//
//각 사람마다 가고 싶은 편의점의 위치는 겹치지 않으며, 편의점의 위치와 베이스캠프의 위치도 겹치지 않습니다.


// ** input으로 받는 r,c값이 0부터인지 1부터인지 확인해야 함.. 나도 알고 싶지 않았음ㅋㅋㅋ...

#include <iostream>
#include <vector>
#include <queue>

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
vector<vector< Spot>> nBC(31);

bool isOutOfBound(int r, int c) {
    return r <= 0 || n < r || c <= 0 || n < c;
}

void getBaseCampDistBFS() {
    queue<Spot> q;
    vector<vector<int>> visited(16, vector<int>(16, 0));
    for (int i = 1; i <= m; i++)
    {
        q.push({ mk[i].r, mk[i].c });
        visited[mk[i].r][mk[i].c] = i;

        while (!q.empty())
        {
            int r = q.front().r;
            int c = q.front().c;
            q.pop();

            for (int d = 0; d < 4; d++)
            {
                int nr = r + dr[d];
                int nc = c + dc[d];

                if (isOutOfBound(nr, nc)) continue;

                if (map[nr][nc] == 1 && visited[nr][nc] != i)
                {
                    nBC[i].push_back({ nr, nc });
                    visited[nr][nc] = i;
                    q.push({ nr, nc });
                }
                else if(visited[nr][nc] != i)
                {
                    visited[nr][nc] = i;
                    q.push({ nr, nc });
                }
            }
        }
    }
}

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
            aq.push({ nr,nc });
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

            if (nr == mk[i].r && c == mk[i].c)
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

    getBaseCampDistBFS();

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
            for (int i = 0; i < nBC[t].size(); i++)
            {
                int r = nBC[t][i].r;
                int c = nBC[t][i].c;

                if (map[r][c] <= 1)
                {
                    p[t].r = r;
                    p[t].c = c;
                    map[r][c] = t + 1;
                    break;
                }
            }
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