// 7:40 ~ 8:40, 10:53 ~ 11:59 <-- 2시간 10분 소요

// ** '정해진' 짧은 길이만큼 DFS로 탐색하되, 갔던 곳으로 되돌아오는 게 가능한 경우엔, visited를 좀 더 잘 사용해야 한다!

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
vector<vector<deque<int>>> map(5, vector<deque<int>>(5));
vector<vector<int>> visited(5, vector<int>(5, 0));
vector<vector<int>> dead(5, vector<int>(5, 0));
vector<Monstor> mList(1);
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
                findPackmanWayDFS(nr, nc, score + map[nr][nc].size(), cnt + 1);
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

        while (map[r][c].size() > 0)
        {
            mList[map[r][c][0]].isDead = true;
            map[r][c].pop_front();
            dead[r][c] = 3;
        }
    }
}

int main() {
    cin >> m >> t;
    cin >> sr >> sc;

    for (int i = 1; i <= m; i++) {
        int r, c, d;
        cin >> r >> c >> d;
        mList.push_back({ r, c, d - 1 });
        map[r][c].push_back({ i });
    }

    for (int k = 1; k <= t; k++)
    {

        // 1번: 몬스터 복제
        for (int i = 1; i <= m; i++) {
            if (mList[i].isDead) continue;

            mList.push_back({ mList[i].r, mList[i].c, mList[i].d, true });
        }

        // 2번: 몬스터 이동
        for (int i = 1; i <= m; i++) {

            if (mList[i].isDead) continue;

            int r = mList[i].r;
            int c = mList[i].c;
            int d = mList[i].d;

            for (int j = 0; j < 8; j++) {
                int nr = r + dr[d];
                int nc = c + dc[d];

                if (isInBound(nr, nc) && (nr != sr || nc != sc) && dead[nr][nc] == 0) {
                    map[r][c].pop_front();
                    map[nr][nc].push_back(i);
                    mList[i].r = nr;
                    mList[i].c = nc;
                    break;
                }
                else {
                    mList[i].d = d = (d + 1) % 8;
                }
            }
        }

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
        for (int i = m + 1; i < mList.size(); i++)
        {
            mList[i].isEgg = false;
            map[mList[i].r][mList[i].c].push_back({ i });
        }

        m = mList.size() - 1;
    }

    int aliveNum = 0;
    for (int i = 1; i < mList.size(); i++)
    {
        if (mList[i].isDead) continue;
        if (mList[i].isEgg) break;

        aliveNum++;
    }
    cout << aliveNum << '\n';

    return 0;
}