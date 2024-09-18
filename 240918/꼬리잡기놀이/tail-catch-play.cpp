// 3:31 ~ 4:21, 5:05 ~ 6:07, 7:54 ~ 10:11 <-- 4시간 꽉 채움(혹은 5분 정도 넘어감);;;

// ** 호흡도 엄청 길고, 로직도 복잡해서 실수가 굉장히 많았고, 코드가 복잡하니 에러를 찾기도 힘들었다 ...;;;
//    => 최대한 코드를 처음부터 간결하게 짜는 거 신경써야 할 듯. 원큐에 문제가 풀릴 가능성은 극히 낮으므로, 그 때부터 디버깅 해야 하는데
//       코드가 간결하지 않으면 에러 찾고, 고치는 데 굉장히 오래 걸린다....

// ** 걍 나중에 또 다시 풀어볼 것...!

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Spot
{
    int r, c;
};

struct SpotInfo
{
    int n = 0;
    int t = 0;
};

int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

int n, m, k;
vector<vector<int>> map(20, vector<int>(20));
vector<vector<SpotInfo>> orderMap(20, vector<SpotInfo>(20));
vector<int> dir(6, 0);
vector<vector< Spot>> team(6);
int score;

bool isInBound(int r, int c) {
    return 0 <= r && r < n && 0 <= c && c < n;
}

struct cmp
{
    bool operator()(Spot& a, Spot& b) {
        return map[a.r][a.c] > map[b.r][b.c];
    }
};

void getTeam(int sr, int sc, int idx, vector<vector<int>> &visited) {
    queue<Spot> q;
    priority_queue<Spot, vector<Spot>, cmp> pq;
    team[idx].push_back({ sr,sc });
    q.push({ sr,sc });
    visited[sr][sc] = idx;

    while (!q.empty())
    {
        int r = q.front().r;
        int c = q.front().c;
        q.pop();

        for (int d = 0; d < 4; d++)
        {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (isInBound(nr, nc) && map[nr][nc] > 0 && map[nr][nc] < 4 && visited[nr][nc] != idx)
            {
                visited[nr][nc] = idx;
                pq.push({ nr,nc });
            }
        }

        bool isSelected = false;
        while (!pq.empty())
        {
            int nr = pq.top().r;
            int nc = pq.top().c;
            pq.pop();

            if (isSelected) {
                visited[nr][nc] = 0;
            }
            else {
                isSelected = true;
                q.push({ nr,nc });
                team[idx].push_back({ nr,nc });
            }
        }
    }
}

void moveTeam(int idx) {
    int temp = 0;
    if (dir[idx] == 0)
    {
        for (int i = 0; i < team[idx].size(); i++)
        {
            int r = team[idx][i].r;
            int c = team[idx][i].c;

            if (i == 0)
            {
                for (int d = 0; d < 4; d++)
                {
                    int nr = r + dr[d];
                    int nc = c + dc[d];

                    if (isInBound(nr, nc) && (map[nr][nc] == 4 || (map[nr][nc] == 3 && team[idx].size() > 2)))
                    {
                        if(map[nr][nc] == 3) temp = 3;

                        team[idx][i].r = nr;
                        team[idx][i].c = nc;
                        map[nr][nc] = map[r][c];

                        map[r][c] = -1;
                        orderMap[nr][nc].n = i + 1;
                        orderMap[nr][nc].t = idx;
                    }
                }
            }
            else
            {
                for (int d = 0; d < 4; d++)
                {
                    int nr = r + dr[d];
                    int nc = c + dc[d];

                    if (isInBound(nr, nc) && map[nr][nc] == -1)
                    {
                        team[idx][i].r = nr;
                        team[idx][i].c = nc;
                        map[nr][nc] = map[r][c];

                        if (i == team[idx].size() - 1)
                        {
                            if (temp == 0) {
                                map[r][c] = 4;
                                orderMap[r][c].n = 0;
                                orderMap[r][c].t = 0;
                            } else map[nr][nc] = temp;
                        }
                        else map[r][c] = -1;
                        
                        orderMap[nr][nc].n = i + 1;
                        orderMap[nr][nc].t = idx;
                    }
                }
            }
        }
    }
    else {
        for (int i = team[idx].size() - 1; i >= 0; i--)
        {
            int r = team[idx][i].r;
            int c = team[idx][i].c;

            if (i == team[idx].size() - 1)
            {
                for (int d = 0; d < 4; d++)
                {
                    int nr = r + dr[d];
                    int nc = c + dc[d];

                    if (isInBound(nr, nc) && (map[nr][nc] == 4 || (map[nr][nc] == 1 && team[idx].size() > 2)))
                    {
                        if (map[nr][nc] == 1) temp = 1;

                        team[idx][i].r = nr;
                        team[idx][i].c = nc;
                        map[nr][nc] = map[r][c];

                        map[r][c] = -1;
                        orderMap[nr][nc].n = i + 1;
                        orderMap[nr][nc].t = idx;
                    }
                }
            }
            else
            {
                for (int d = 0; d < 4; d++)
                {
                    int nr = r + dr[d];
                    int nc = c + dc[d];

                    if (isInBound(nr, nc) && map[nr][nc] == -1)
                    {
                        team[idx][i].r = nr;
                        team[idx][i].c = nc;
                        map[nr][nc] = map[r][c];

                        if (i == 0)
                        {
                            if (temp == 0) {
                                map[r][c] = 4;
                                orderMap[r][c].n = 0;
                                orderMap[r][c].t = 0;
                            }
                            else map[nr][nc] = temp;
                        }
                        else map[r][c] = -1;
                        
                        orderMap[nr][nc].n = i + 1;
                        orderMap[nr][nc].t = idx;
                    }
                }
            }
        }
    }
}

void shoutBall(char w, int idx, int d) {
    if (w == 'r')
    {
        if (d == 0)
        {
            for (int c = 0; c < n; c++)
            {
                if (map[idx][c] > 0 && map[idx][c] < 4)
                {
                    int teamNum = orderMap[idx][c].t;
                    if (dir[teamNum] == 0)
                    {
                        int n = orderMap[idx][c].n;
                        score += n * n;
                        dir[teamNum] = 1;
                    } 
                    else
                    {
                        int n = team[teamNum].size() - orderMap[idx][c].n + 1;
                        score += n * n;
                        dir[teamNum] = 0;
                    }
                    return;
                }
            }
        }
        else
        {
            for (int c = n - 1; c >= 0; c--)
            {
                if (map[idx][c] > 0 && map[idx][c] < 4)
                {
                    int teamNum = orderMap[idx][c].t;
                    if (dir[teamNum] == 0)
                    {
                        int n = orderMap[idx][c].n;
                        score += n * n;
                        dir[teamNum] = 1;
                    }
                    else
                    {
                        int n = team[teamNum].size() - orderMap[idx][c].n + 1;
                        score += n * n;
                        dir[teamNum] = 0;
                    }
                    return;
                }
            }
        }
    }
    else if (w == 'c')
    {
        if (d == 0)
        {
            for (int r = n - 1; r >= 0; r--)
            {
                if (map[r][idx] > 0 && map[r][idx] < 4)
                {
                    int teamNum = orderMap[r][idx].t;
                    if (dir[teamNum] == 0)
                    {
                        int n = orderMap[r][idx].n;
                        score += n * n;
                        dir[teamNum] = 1;
                    }
                    else
                    {
                        int n = team[teamNum].size() - orderMap[r][idx].n + 1;
                        score += n * n;
                        dir[teamNum] = 0;
                    }
                    return;
                }
            }
        }
        else
        {
            for (int r = 0; r < n; r++)
            {
                if (map[r][idx] > 0 && map[r][idx] < 4)
                {
                    int teamNum = orderMap[r][idx].t;
                    if (dir[teamNum] == 0)
                    {
                        int n = orderMap[r][idx].n;
                        score += n * n;
                        dir[teamNum] = 1;
                    }
                    else
                    {
                        int n = team[teamNum].size() - orderMap[r][idx].n + 1;
                        score += n * n;
                        dir[teamNum] = 0;
                    }
                    return;
                }
            }
        }
    }
}

int main() {
    
    score = 0;

    cin >> n >> m >> k;
    int teamNum = 1;
    vector<vector<int>> visited(20, vector<int>(20));
    for (int r = 0; r < n; r++)
    {
        for (int c = 0; c < n; c++)
        {
            cin >> map[r][c];
            //if (map[r][c] == 1) <-- 이걸 여기서 하면 안됐다..!(아직 못받은 인풋이 당연히 남았다..;;) 
            //{
            //    getTeam(r, c, teamNum, visited);
            //    teamNum++;
            //}
        }
    }

    for (int r = 0; r < n; r++)
    {
        for (int c = 0; c < n; c++)
        {
            if (map[r][c] == 1)
            {
                getTeam(r, c, teamNum, visited);
                teamNum++;
            }
        }
    }

    int v;
    for (int curr = 0; curr < k; curr++)
    {
        for (int i = 1; i <= m; i++)
        {
            moveTeam(i);
        }

        v = (curr / n) % 4;
        if (v == 0) shoutBall('r', curr % n, 0);
        else if (v == 1) shoutBall('c', curr % n, 0);
        else if (v == 2) shoutBall('r', n - 1 - (curr % n), 1);
        else if (v == 3) shoutBall('c', n - 1 - (curr % n), 1);
    }
    cout << score << '\n';

    return 0;
}