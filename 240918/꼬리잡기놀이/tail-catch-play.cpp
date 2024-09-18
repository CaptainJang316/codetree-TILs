// 3:31 ~ 4:21, 5:05 ~ 

// n * n 격자에서 꼬리잡기놀이를 진행. 꼬리잡기놀이는 다음과 같이 진행됩니다.

// 3명 이상이 한 팀이 됩니다. 
// 모든 사람들은 자신의 '앞 사람의 허리'를 잡고 움직이게 되며, 맨 앞에 있는 사람을 머리사람, 맨 뒤에 있는 사람을 꼬리사람이라고 합니다. 
// 각 팀은 게임에서 주어진 이동 선을 따라서'만' 이동합니다. 
// 각 팀의 이동 선은 끝이 '이어져있습니다'. 
// 
// 각 팀의 이동 선은 '서로 겹치지 않습니다'.

// 게임은 라운드 별로 진행이 되며, 한 라운드는 다음과 같이 진행됩니다.

// 1. 먼저 각 팀은 '머리사람을 따라서' 한 칸 이동합니다.

// 2. 각 라운드마다 공이 정해진 선을 따라 던져집니다. 

// 3. 공이 던져지는 경우에 해당 선에 사람이 '있으면' 최초에 만나게 되는 사람'만'이 공을 얻게 되어 점수를 얻게 됩니다. 
// 점수는 해당 사람이 머리사람을 시작으로 팀 내에서 k번째 사람이라면 'k의 제곱만큼' 점수를 얻게 됩니다. 
// 아무도 공을 받지 못하는 경우에는 아무 점수도 획득하지 '못합니다'. 

// 공을 획득한 팀의 경우에는 머리사람과 꼬리사람이 바뀝니다. 즉 '방향을 바꾸게 됩니다'.


// 총 격자의 크기, 각 팀의 위치, 각 팀의 이동 선, 총 진행하는 라운드의 수가 주어질 때 '각 팀이 획득한 점수의 총합'을 구하는 프로그램을 구하세요.


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

void getTeam(int sr, int sc, int idx, vector<vector<int>> &visited) {
    queue<Spot> q;
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
                q.push({ nr,nc });
                team[idx].push_back({ nr,nc });
            }
        }
    }
}

void moveTeam(int idx) {
    //cout << "team[idx].size(): " << team[idx].size() << endl;
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

                    if (isInBound(nr, nc) && map[nr][nc] == 4)
                    {
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
                            map[r][c] = 4;
                            orderMap[r][c].n = 0;
                            orderMap[r][c].t = 0;
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

                    if (isInBound(nr, nc) && map[nr][nc] == 4)
                    {
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
                            map[r][c] = 4;
                            orderMap[r][c].n = 0;
                            orderMap[r][c].t = 0;
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
                    //cout << "orderMap[idx][c].n: " << orderMap[idx][c].n << endl;
                    score += (orderMap[idx][c].n * orderMap[idx][c].n);
                    dir[orderMap[idx][c].t] = dir[orderMap[idx][c].t] == 0 ? 1 : 0;
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
                    score += (orderMap[idx][c].n * orderMap[idx][c].n);
                    dir[orderMap[idx][c].t] = dir[orderMap[idx][c].t] == 0 ? 1 : 0;
                    return;
                }
            }
        }
    }
    else if (w == 'c')
    {
        if (d == 0)
        {
            for (int r = 0; r < n; r++)
            {
                if (map[r][idx] > 0 && map[r][idx] < 4)
                {
                    score += (orderMap[r][idx].n * orderMap[r][idx].n);
                    dir[orderMap[r][idx].t] = dir[orderMap[r][idx].t] == 0 ? 1 : 0;
                    return;
                }
            }
        }
        else
        {
            for (int r = n - 1; r >= 0; r--)
            {
                if (map[r][idx] > 0 && map[r][idx] < 4)
                {
                    score += (orderMap[r][idx].n * orderMap[r][idx].n);
                    dir[orderMap[r][idx].t] = dir[orderMap[r][idx].t] == 0 ? 1 : 0;
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
    //cout << '\n';
    //for (int r = 0; r < n; r++)
    //{
    //    for (int c = 0; c < n; c++)
    //    {
    //        cout << visited[r][c] << ' ';
    //    }
    //    cout << '\n';
    //}

    int v;
    for (int curr = 0; curr < k; curr++)
    {
        for (int i = 1; i <= m; i++)
        {
            moveTeam(i);
        }

        //cout << '\n';
        //for (int r = 0; r < n; r++)
        //{
        //    for (int c = 0; c < n; c++)
        //    {
        //        cout << map[r][c] << ' ';
        //    }
        //    cout << '\n';
        //}

        v = curr / n;
        if (v == 0) shoutBall('r', curr, 0);
        else if (v == 1) shoutBall('c', curr % n, 0);
        else if (v == 2) shoutBall('r', n - 1 - (curr % n), 1);
        else if (v == 3) shoutBall('c', n - 1 - (curr % n), 1);
    }
    cout << score << '\n';

    return 0;
}