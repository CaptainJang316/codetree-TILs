// 6:45 ~ 7:10, 8:10 ~ 9:23 ~ 9:39, 9:55 ~ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

//첫 번째 줄에는 숲의 크기를 의미하는 R, C, 정령의 수 K가 공백을 사이에 두고 주어집니다.
//
//그다음 줄부터 K개의 줄에 거쳐 각 골렘이 출발하는 열 c, 골렘의 출구 방향 정보 d가 공백을 사이에 두고 주어집니다.
//
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

//bool rightPossible(int r, int c) {
//    if (isOutOfBound(r + 1, c - 1) || isOutOfBound(r + 2, c) || isOutOfBound(r + 1, c + 1)) return false;
//
//    if (map[r + 1][c - 1] != 0 || map[r + 2][c] != 0 || map[r + 1][c + 1] != 0) return false;
//
//    return true;
//}

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
            //cout << "down\n";
            //moveDown(r, g);
            r++;
        }
        else if (leftPossible(r, g[i].c)) {
            //cout << "left\n";
            moveLeft(g[i]);
            r++;
        }
        else if (rightPossible(r, g[i].c))
        {
            //cout << "right\n";
            moveRight(g[i]);
            r++;
        }
        else
        {
            //cout << "end!\n";
            if (r <= 1)
            {
                fill(map.begin(), map.end(), vector<int>(71, 0));
                fill(exitMap.begin(), exitMap.end(), vector<int>(71, 0));
                //cout << "out!\n";
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

                //cout << "=== map ===" << endl;
                //for (int r = 1; r <= R; r++)
                //{
                //    for (int c = 1; c <= C; c++)
                //    {
                //        cout << map[r][c] << " ";
                //    }
                //    cout << endl;
                //}
                //cout << endl;
                //cout << "=== exitMap ===" << endl;
                //for (int r = 1; r <= R; r++)
                //{
                //    for (int c = 1; c <= C; c++)
                //    {
                //        cout << exitMap[r][c] << " ";
                //    }
                //    cout << endl;
                //}

                maxR = r;
                fairyMoveBFS(r, c, i);
                //visited[r][c] = 1;
                //fairyMoveDFS(r, c);
                //visited[r][c] = 0;

                totalScore += maxR;
            }
            return;
        }
    }
}

// 각 정령들이 최종적으로 위치한 행의 총합을 출력하세요.
int main() {
    totalScore = 0;
    cin >> R >> C >> K;
    for (int i = 1; i <= K; i++)
    {
        cin >> g[i].c >> g[i].d;

        golemMove(i);
        //cout << "totalScore: " << totalScore << endl;
    }

    cout << totalScore << '\n';
    return 0;
}