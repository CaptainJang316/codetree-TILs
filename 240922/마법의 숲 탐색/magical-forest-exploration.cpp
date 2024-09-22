// 6:45 ~ 7:10, 8:10 ~ 9:23 ~ 9:39, 9:55 ~ 10:29 <-- 2시간 반 소요

// ** 완전탐색인데 맵이 충분히 작지 않은 경우엔, DFS보다 BFS로 구현하는 게 낫다!
//    (DFS로 구현했을 땐 시간초과 발생 --> BFS로 변경하니 바로 통과됨)
//    => 완전탐색을 해야 할 경우, 웬만하면 BFS가 나은 듯.

// ** 출구 정보는 별도의 exitMap으로 관리하니 수월 & 간편함.

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

//골렘의 출구 방향 정보 d는 0과 3 사이의 수로 주어지며 각각의 숫자 0, 1, 2, 3은 북, 동, 남, 서쪽을 의미합니다.
int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

struct Golem
{
    int c, d;
};

struct Spot
{
    int r, c;
};

int R, C, K;
vector<vector<int>> map(71, vector<int>(71, 0));
vector<vector<int>> exitMap(71, vector<int>(71, 0));
vector<vector<int>> visited(71, vector<int>(71, 0));
vector<Golem> g(1001);
queue< Spot> q;
int maxR, totalScore;

bool isInBound(int r, int c) {
    return 1 <= r && r <= R && 1 <= c && c <= C;
}

bool isOutOfBound(int r, int c) {
    return r > R || c < 1 || C < c;
}

bool downPossible(int r, int c) {
    if (isOutOfBound(r + 1, c - 1) || isOutOfBound(r + 2, c) || isOutOfBound(r + 1, c + 1)) return false;

    if (map[r + 1][c - 1] != 0 || map[r + 2][c] != 0 || map[r + 1][c + 1] != 0) return false;

    return true;
}

bool leftPossible(int r, int c) {
    if (isOutOfBound(r, c - 2) || isOutOfBound(r + 1, c - 2) || isOutOfBound(r + 1, c - 1) || isOutOfBound(r + 2, c - 1)) return false;

    if ((r >= 0 && map[r][c - 2] != 0) || map[r + 1][c - 2] != 0 || map[r + 1][c - 1] != 0 || map[r + 2][c - 1] != 0) return false;

    return true;
}

bool rightPossible(int r, int c) {
    if (isOutOfBound(r, c + 2) || isOutOfBound(r + 1, c + 2) || isOutOfBound(r + 1, c + 1) || isOutOfBound(r + 2, c + 1)) return false;

    if ((r >= 0 && map[r][c + 2] != 0) || map[r + 1][c + 2] != 0 || map[r + 1][c + 1] != 0 || map[r + 2][c + 1] != 0) return false;

    return true;
}

void moveDown(int r, Golem& g) {
    
}

void moveLeft(Golem& g) {
    g.c -= 1;
    g.d = (g.d + 3) % 4;
}

void moveRight(Golem& g) {
    g.c += 1;
    g.d = (g.d + 1) % 4;
}

//void fairyMoveDFS(int r, int c) {
//    if (r > maxR)
//    {
//        maxR = r;
//    }
//
//    for (int d = 0; d < 4; d++)
//    {
//        int nr = r + dr[d];
//        int nc = c + dc[d];
//
//        if (isInBound(nr, nc)) {
//            if (map[nr][nc] == 0 || visited[nr][nc] == 1) continue;
//
//            if (map[nr][nc] == map[r][c])
//            {
//                visited[nr][nc] = 1;
//                fairyMoveDFS(nr, nc);
//                visited[nr][nc] = 0;
//            }
//            else if(exitMap[r][c] != 0) // <-- 다음 위치가 현재 위치랑 다르다면, 현재 위치가 출구인지 확인!
//            {
//                visited[nr][nc] = 1;
//                fairyMoveDFS(nr, nc);
//                visited[nr][nc] = 0;
//                //int idx = map[r][c];
//                //int g_c = g[idx].c;
//                //int d = g[idx].d;
//            }
//        }
//    }
//}

void fairyMoveBFS(int sr, int sc, int idx) {
    q.push({ sr, sc });
    visited[sr][sc] = idx;

    while (!q.empty())
    {
        int r = q.front().r;
        int c = q.front().c;
        q.pop();

        maxR = max(r, maxR);

        for (int d = 0; d < 4; d++)
        {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (isInBound(nr, nc)) {
                if (map[nr][nc] == 0 || visited[nr][nc] == idx) continue;

                if (map[nr][nc] == map[r][c])
                {
                    visited[nr][nc] = idx;
                    q.push({ nr, nc });
                }
                else if (exitMap[r][c] != 0) // <-- 다음 위치가 현재 위치랑 다르다면, 현재 위치가 출구인지 확인!
                {
                    visited[nr][nc] = idx;
                    q.push({ nr, nc });
                }
            }
        }
    }
}

void golemMove(int i) {
    int r = -1;
    while (true)
    {
        if (downPossible(r, g[i].c)) {
            r++;
        }
        else if (leftPossible(r, g[i].c)) {
            moveLeft(g[i]);
            r++;
        }
        else if (rightPossible(r, g[i].c))
        {
            moveRight(g[i]);
            r++;
        }
        else
        {
            if (r <= 1)
            {
                fill(map.begin(), map.end(), vector<int>(71, 0));
                fill(exitMap.begin(), exitMap.end(), vector<int>(71, 0));
            }
            else
            {
                int c = g[i].c;
                map[r][c] = i;
                
                for (int d = 0; d < 4; d++)
                {
                    int nr = r + dr[d];
                    int nc = c + dc[d];

                    map[nr][nc] = i;
                }

                int er = r + dr[g[i].d];
                int ec = c + dc[g[i].d];
                exitMap[er][ec] = i;

                maxR = r;
                fairyMoveBFS(r, c, i);

                //visited[r][c] = 1; <-- DFS로 구현하니, 시간초과 발생..!
                //fairyMoveDFS(r, c);
                //visited[r][c] = 0;

                totalScore += maxR;
            }
            return;
        }
    }
}

int main() {
    totalScore = 0;
    cin >> R >> C >> K;
    for (int i = 1; i <= K; i++)
    {
        cin >> g[i].c >> g[i].d;

        golemMove(i);
    }
    cout << totalScore << '\n';
    return 0;
}