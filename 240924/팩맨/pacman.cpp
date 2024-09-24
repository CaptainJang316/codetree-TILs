// 7:40 ~ 8:40, 10:53 ~ 11:59 <-- 2시간 10분 소요

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Monstor {
    int r, c, d;
    bool isEgg = false;
    bool isDead = false;
};

struct Spot {
    int r, c;
};

int dr[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int dc[] = { 0, -1, -1, -1, 0, 1, 1, 1 };

// 상-좌-하-우
int pDr[] = { -1, 0, 1, 0 };
int pDc[] = { 0, -1, 0, 1 };

int m, t, sr, sc;
vector<vector<vector<int>>> map(5, vector<vector<int>>(5, vector<int>(8, 0)));
vector<vector<vector<int>>> temp(5, vector<vector<int>>(5, vector<int>(8, 0)));
vector<vector<vector<int>>> eggMap(5, vector<vector<int>>(5, vector<int>(8, 0)));
vector<vector<int>> nMap(5, vector<int>(5, 0));
vector<vector<int>> temp_nMap(5, vector<int>(5, 0));
vector<vector<int>> egg_nMap(5, vector<int>(5, 0));
vector<vector<int>> visited(5, vector<int>(5, 0));
vector<vector<int>> dead(5, vector<int>(5, 0));
//vector<Monstor> mList(1);
vector<Spot> pWay;
vector<Spot> bestWay;
int maxScore;

bool isInBound(int r, int c) {
    return 1 <= r && r <= 4 && 1 <= c && c <= 4;
}

void findPackmanWayDFS(int r, int c, int score, int cnt) {
    if (cnt == 3)
    {
        if (maxScore < score) {
            bestWay = pWay;
            maxScore = score;
            sr = r;
            sc = c;
        }

        return;
    }

    for (int d = 0; d < 4; d++)
    {
        int nr = r + pDr[d];
        int nc = c + pDc[d];

        if (isInBound(nr, nc))
        {
            pWay.push_back({ nr, nc });
            if (visited[nr][nc] != 1) {
                visited[nr][nc] = 1;
                findPackmanWayDFS(nr, nc, score + nMap[nr][nc], cnt + 1);
                visited[nr][nc] = 0; // <-- 처음에 이걸 1로 만든 단계에서만, 이걸 다시 0으로 바꿀 수 있어야 한다..!
            }
            else {
                findPackmanWayDFS(nr, nc, score, cnt + 1);
            }
            pWay.pop_back();
        }
    }
}

void packmanMove() {
    for (int i = 0; i < bestWay.size(); i++)
    {
        int r = bestWay[i].r;
        int c = bestWay[i].c;

        if (nMap[r][c] > 0)
        {
            fill(map[r][c].begin(), map[r][c].end(), 0);
            nMap[r][c] = 0;
            dead[r][c] = 3;
        }
    }
}

int findDir(int r, int c, int d) {
    int dir = d;
    for (int i = 0; i < 8; i++)
    {
        int nr = r + dr[dir];
        int nc = c + dc[dir];

        if (isInBound(nr, nc) && (nr != sr || nc != sc) && dead[nr][nc] == 0) {
            return dir;
        }
        else {
            dir = (dir + 1) % 8;
        }
    }
    return -1;
}

int main() {
    cin >> m >> t;
    cin >> sr >> sc;

    for (int i = 1; i <= m; i++) {
        int r, c, d;
        cin >> r >> c >> d;
        map[r][c][d-1] += 1;
        nMap[r][c] += 1;
    }

    for (int k = 1; k <= t; k++)
    {
        // 1번: 몬스터 복제
        for (int r = 1; r <= 4; r++)
        {
            for (int c = 1; c <= 4; c++)
            {
                if (nMap[r][c] > 0)
                {
                    for (int d = 0; d < 8; d++)
                    {
                        if (map[r][c][d] > 0)
                        {
                            eggMap[r][c][d] = map[r][c][d];
                        }
                    }
                    egg_nMap[r][c] += nMap[r][c];
                }
            }
        }

        // 2번: 몬스터 이동
        for (int r = 1; r <= 4; r++)
        {
            for (int c = 1; c <= 4; c++)
            {
                if (nMap[r][c] > 0)
                {
                    for (int d = 0; d < 8; d++)
                    {
                        if (map[r][c][d] == 0) continue;

                        int nd = findDir(r, c, d);

                        if (nd != -1)
                        {
                            int nr = r + dr[nd];
                            int nc = c + dc[nd];

                            temp[nr][nc][nd] += map[r][c][d];

                            temp_nMap[nr][nc] += map[r][c][d];
                            map[r][c][d] = 0;

                            nMap[r][c] = 0;
                        }
                    }
                }
            }
        }

        for (int r = 1; r <= 4; r++)
        {
            for (int c = 1; c <= 4; c++)
            {
                if (temp_nMap[r][c] > 0)
                {
                    for (int d = 0; d < 8; d++)
                    {
                        if (temp[r][c][d] == 0) continue;

                        map[r][c][d] = temp[r][c][d];
                    }
                    nMap[r][c] += temp_nMap[r][c];
                    temp_nMap[r][c] = 0;
                }
            }
        }
        fill(temp.begin(), temp.end(), vector< vector<int>>(5, vector<int>(8, 0)));

        // 3번: 팩맨 이동(이 문제에선, 처음 위치에 visited 처리하지 않는다)
        maxScore = -1;
        findPackmanWayDFS(sr, sc, 0, 0);
        packmanMove();

        // 4번: 시체 1 감소
        for (int r = 1; r <= 4; r++)
        {
            for (int c = 1; c <= 4; c++)
            {
                if (dead[r][c] > 0) dead[r][c] -= 1;
            }
        }

        // 5번: 알 부화
        for (int r = 1; r <= 4; r++)
        {
            for (int c = 1; c <= 4; c++)
            {
                if (egg_nMap[r][c] > 0)
                {
                    for (int d = 0; d < 8; d++)
                    {
                        if (eggMap[r][c][d] == 0) continue;

                        map[r][c][d] += eggMap[r][c][d];
                        eggMap[r][c][d] = 0;
                    }
                    nMap[r][c] += egg_nMap[r][c];
                    egg_nMap[r][c] = 0;
                }
            }
        }
    }

    int aliveNum = 0;
    for (int r = 1; r <= 4; r++)
    {
        for (int c = 1; c <= 4; c++)
        {
            if (nMap[r][c] > 0) aliveNum += nMap[r][c];
        }
    }
    cout << aliveNum << '\n';

    return 0;
}