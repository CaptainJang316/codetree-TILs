// 2:30 ~ 4:55 <-- 2시간 20분

// ** 처음부터 로직을 제대로 이해할 수 있도록 정리해서 구현하기!(덕분에 헤대는 건 별로 없었다)

// ** 주요 이동 정보들은 map에 기입해서 확인해보면 빠르게 원인 파악이 가능하다.

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Runner
{
    int r, c, d;
    bool isDead = false;
};

struct Tagger
{
    int r, c;
    int dir = 0;
};

struct SpotInfo
{
    int r, c, idx;
};

int dr[2][4] = { {-1, 0, 1, 0}, {1, 0, -1, 0} };
int dc[2][4] = { {0, 1, 0, -1}, {0, 1, 0, -1} };

int rDr[] = { 0, 0, 1, -1 };
int rDc[] = { -1, 1, 0, 0 };

int n, m, h, k;
vector<vector<vector<int>>> map(100, vector<vector<int>>(100));
vector<vector<int>> tMap(100, vector<int>(100, 0));
vector< Runner> p(10000);
Tagger tg;
queue< SpotInfo> q;
int cd, mCnt, cr, cc, mBound, dCnt, score;

bool isOutOfBound(int r, int c) {
    return r < 1 || n < r || c < 1 || n < c;
}

int changeDir(int d) {
    if (d == 0) return 1;
    if (d == 1) return 0;
    if (d == 2) return 3;
    if (d == 3) return 2;
}

void taggerMove() {
    // 현재 방향으로 먼저 한칸 이동하고
    int dir = tg.dir;
    tg.r = tg.r + dr[dir][cd];
    tg.c = tg.c + dc[dir][cd];
    mCnt++;

    if (tg.dir == 0)
    {
        if (mCnt == mBound)
        {
            mCnt = 0;
            cd = (cd + 1) % 4;

            if (dCnt == 0) dCnt = 1;
            else if (dCnt == 1 && mBound == n - 1) dCnt = 2;
            else
            {
                // 만약 양 끝으로 도착하면 방향 반대로 전환
                if (tg.r == 1 && tg.c == 1) {
                    tg.dir = 1;
                    cd = 0;
                }
                else {
                    mBound++;
                }
                dCnt = 0;
            }
        }
    }
    else if (tg.dir == 1)
    {
        if (mCnt == mBound)
        {
            mCnt = 0;
            cd = (cd + 1) % 4;

            if (dCnt == 0) dCnt = 1;
            else if (dCnt == 1 && mBound == n - 1) dCnt = 2;
            else
            {
                if (tg.r == cr && tg.c == cc) {
                    tg.dir = 0;
                    cd = 0;
                }
                else {
                    mBound--;
                }
                dCnt = 0;
            }
        }
    }
}

void seekRunner(int t) {
    int r = tg.r;
    int c = tg.c;
    int dir = tg.dir;
    for (int i = 0; i < 3; i++)
    {
        if (isOutOfBound(r, c)) continue;

        //t x 현재 턴에서 잡힌 도망자의 수
        if (map[r][c].size() > 0 && tMap[r][c] != 1)
        {
            score += (t * map[r][c].size());
            for (int j = 0; j < map[r][c].size(); j++)
            {
                p[map[r][c][j]].isDead = true;
            }
            map[r][c].clear();
        }

        r = r + dr[dir][cd];
        c = c + dc[dir][cd];
    }
}

int main() {
    cd = 0;
    mCnt = 0;
    mBound = 1;
    dCnt = 0;
    score = 0;

    cin >> n >> m >> h >> k;

    cr = n / 2 + 1;
    cc = n / 2 + 1;
    tg.r = cr;
    tg.c = cc;

    for (int i = 1; i <= m; i++)
    {
        cin >> p[i].r >> p[i].c >> p[i].d;
        map[p[i].r][p[i].c].push_back(i);
    }
    for (int i = 1; i <= h; i++)
    {
        int r, c;
        cin >> r >> c;
        tMap[r][c] = 1;
    }

    for (int t = 1; t <= k; t++)
    {
        for (int i = 1; i <= m; i++)
        {
            if (p[i].isDead) continue;

            int r = p[i].r;
            int c = p[i].c;
            int d = p[i].d;

            if (abs(r - tg.r) + abs(c - tg.c) > 3) continue;

            int nr = r + rDr[d];
            int nc = c + rDc[d];

            if (isOutOfBound(nr, nc))
            {
                d = p[i].d = changeDir(d);
                nr = r + rDr[d];
                nc = c + rDc[d];
            }
            if (nr != tg.r || nc != tg.c)
            {
                q.push({ nr, nc, i });
                for (int j = 0; j < map[r][c].size(); j++)
                {
                    if (map[r][c][j] == i) {
                        map[r][c].erase(map[r][c].begin() + j);
                        j--;
                        break;
                    }
                }
                p[i].r = nr;
                p[i].c = nc;
            }
        }
        while (!q.empty())
        {
            int r = q.front().r;
            int c = q.front().c;
            int idx = q.front().idx;
            q.pop();

            map[r][c].push_back(idx);
        }

        taggerMove();

        seekRunner(t);
    }
    
    cout << score << '\n';
    return 0;
}