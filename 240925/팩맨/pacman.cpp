// 7:40 ~ 8:40, 10:53 ~ 11:59 <-- 2시간 10분 소요

// ** '정해진' 짧은 길이만큼 DFS로 탐색하되, 갔던 곳으로 되돌아오는 게 가능한 경우엔, visited를 좀 더 잘 사용해야 한다!
//
//if (isInBound(nr, nc))
//{
//    pWay.push_back({ nr, nc });
//    if (visited[nr][nc] != 1) {
//        visited[nr][nc] = 1;
//        findPackmanWayDFS(nr, nc, score + map[nr][nc].size(), cnt + 1);
//        visited[nr][nc] = 0; <-- 처음에 이걸 1로 만든 단계에서만, 이걸 다시 0으로 바꿀 수 있어야 한다..!
//    }
//    else {
//        findPackmanWayDFS(nr, nc, score, cnt + 1);
//    }
//    pWay.pop_back();
//}

// ** 관리해야 할 요소가 엄청나게 많아질 수 있는 경우(이 문제의 경우, 몬스터의 수가 100만개까지 늘어갈 수도 있다고 했다),
//    + 맵이 고정적으로 작은 경우,
// 
//    리스트를 따로 만들기보다, (3차원을 만들더라도)map을 활용해서 푸는 게, 문제에서 의도한 바일 가능성이 크다..!
//    (모든 리스트를 매번 다 일일이 다루지 않고, 특정 위치에 있는 요소들을 한번에 다루면 훨씬 빠르다)
//    => 중간중간 map 전체를 복사하거나, 비우는 작업을 추가해도, 리스트로 만들어 풀 때보다 시간 복잡도가 말도 안되게 적었다.
//       (리스트로 풀었을 때: 108ms & 32MB vs map만으로 풀었을 때: 7ms & 0MB)
//    
//    => 리스트로 관리하는 게 일반적으론 더 효율적인 방식이 맞다.
//       하지만, 위와 같은 경우엔 굉장히 연산이 오래 걸리는 케이스들이 발생할 수 있는 반면, map 중심으로 관리하면 각 케이스가 엄청 빠르진 않아도,
//       요소가 아무리 많아지던 상관없이 모든 케이스의 연산 속도가 일관성이 있다.

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

                        map[r][c][d] += temp[r][c][d];
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