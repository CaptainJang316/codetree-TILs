// 2:33 ~ 

// 술래잡기 게임은 n * n 크기의 격자에서 진행되며 술래는 처음 정중앙에 서있습니다. (1,1) ~~

// 술래잡기 게임에는 m명의 도망자가 있습니다. 
// 도망자는 처음 지정된 곳에 서있습니다. 도망자는 중앙에서 시작하지는 않습니다. 
// 도망자의 종류는 '좌우로만 움직이는 유형'과 '상하로만 움직이는 유형' 이렇게 2가지가 있습니다. 
// 이때 좌우로 움직이는 사람은 '항상 오른쪽을 보고 시작'하며, 상하로 움직이는 사람은 '항상 아래쪽을 보고 시작'합니다.

// 또, 이 술래잡기 게임에는 h개의 나무가 있습니다.

// 나무가 도망자와 '초기에 겹쳐져 주어지는 것' 역시 '가능'합니다.

// 술래잡기 게임에서는 m명의 도망자가 '먼저' '동시에 움직이고', 그 다음 술래가 움직이고, 도망자가 움직이고, 술래가 움직이고, ... 
// 이렇게 도망자가 1턴 그리고 이어서 술래가 1턴 진행하는 것을 총 k번 반복하게 됩니다.

// 이때 도망자가 움직일 때 현재 '술래와의 거리가 3 이하인 도망자만' 움직입니다. <-- 3칸 이내ㅇㅇ
// 도망자의 위치가 (x1, y1), 술래의 위치가 (x2, y2)라 했을 때 두 사람간의 거리는 |x1 - x2| + |y1 - y2|로 정의됩니다.

// 술래와의 거리가 3 이하인 도망자들은 1턴 동안 다음 규칙에 따라 움직이게 됩니다.

// - 현재 바라보고 있는 방향으로 1칸 움직인다 했을 때 격자를 벗어나지 않는 경우

//		* 움직이려는 칸에 술래가 있다면 움직이지 '않습니다'.
//		* 움직이려는 칸에 술래가 있지 않다면 해당 칸으로 이동합니다. 해당 칸에 나무가 있어도 괜찮습니다.

// - 현재 바라보고 있는 방향으로 1칸 움직인다 했을 때 격자를 벗어나는 경우

//		* 먼저 방향을 반대로 틀어줍니다. 이후 바라보고 있는 방향으로 1칸 움직인다 했을 때, 해당 위치에 술래가 '없다면' 1칸 앞으로 이동합니다.


// 그다음 술래 턴.
// 술래는 처음 '위 방향으로 시작하여' 달팽이 모양으로 움직입니다.

// 만약 끝에 도달하게 되면 다시 거꾸로 중심으로 이동하고, 다시 중심에 오게 되면 처음처럼 위 방향으로 시작하여 시계뱡향으로 도는 것을 k턴에 걸쳐 반복하게 됩니다.

// 술래는 1번의 턴 동안 '한 칸' 이동하게 됩니다. 이동 후의 위치가 만약 이동방향이 틀어지는 지점이라면, '방향을 바로 틀어줍니다'. 
// 만약 이동을 통해 양끝에 해당하는 위치인 (1행, 1열) 혹은 정중앙에 도달하게 된다면 이 경우 역시 '방향을 바로 틀어줘야 함'에 유의합니다.
// => 이동한 직후, 시야 3칸 이내로 잡아야 하기 때문에, 해당 턴에서 먼저 방향 틀고 나서, 그 다음 시야 체크해야 함.

// 술래의 시야는 현재 바라보고 있는 방향을 기준으로 '현재 칸을 포함하여' 총 3칸입니다. 격자 크기에 상관없이 술래의 시야는 항상 3칸임에 유의합니다.

// 하지만 만약 나무가 놓여 있는 칸이라면, 해당 칸에 있는 도망자는 나무에 가려져 보이지 않게 됩니다.

// 잡힌 도망자는 사라지고, 술래는 현재 턴을 t번째 턴이라고 했을 때 't x 현재 턴에서 잡힌 도망자의 수'만큼의 점수를 얻게 됩니다.

// k번에 걸쳐 술래잡기를 진행하는 동안 술래가 총 얻게된 점수를 출력하는 프로그램을 작성해보세요.


//첫 번째 줄에 n, m, h, k가 공백을 사이에 두고 주어집니다.n은 반드시 홀수입니다.
//
//이후 m개의 줄에 걸쳐 도망자의 위치(x, y)와 이동 방법 d가 공백을 사이에 두고 차례대로 주어집니다.이동 방법 d가 1인 경우 좌우로 움직임을, 2인 경우 상하로만 움직임을 뜻합니다.또한, 좌우로 움직이는 사람은 항상 오른쪽을 보고 시작하며, 상하로 움직이는 사람은 항상 아래쪽을 보고 시작함에 유의합니다.
//
//이후 h개의 줄에 걸쳐 나무의 위치 x, y가 공백을 사이에 두고 차례대로 주어집니다.
//
//도망자끼리 위치가 겹쳐져 주어지는 경우는 없으며, 나무들 역시 위치가 겹쳐져 주어지는 경우는 없다고 가정해도 좋습니다.
// 단, 이동 도중 도망자들의 위치는 겹칠 수 있음에 유의합니다.

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
    //cout << tg.r << ", " << tg.c << '\n';
    //cout << "===== tMap =====" << endl;
    //tMap[tg.r][tg.c] = 2;
    //for (int r = 1; r <= n; r++)
    //{
    //    for (int c = 1; c <= n; c++)
    //    {
    //        cout << tMap[r][c] << " ";
    //    }
    //    cout << endl;
    //}
    //cout << "===== before runner move - map[r][c].size() =====" << endl;
    //for (int r = 1; r <= n; r++)
    //{
    //    for (int c = 1; c <= n; c++)
    //    {
    //        cout << map[r][c].size() << " ";
    //    }
    //    cout << endl;
    //}
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

        //cout << "===== after runner move - map[r][c].size() =====" << endl;
        //for (int r = 1; r <= n; r++)
        //{
        //    for (int c = 1; c <= n; c++)
        //    {
        //        cout << map[r][c].size() << " ";
        //    }
        //    cout << endl;
        //}

        //tMap[tg.r][tg.c] = 0;
        taggerMove();

        //cout << "===== tMap =====" << endl;
        //tMap[tg.r][tg.c] = 2;
        //for (int r = 1; r <= n; r++)
        //{
        //    for (int c = 1; c <= n; c++)
        //    {
        //        cout << tMap[r][c] << " ";
        //    }
        //    cout << endl;
        //}

        seekRunner(t);
        //cout << "score: " << score << '\n';
        //cout << "===== after seekRunner =====" << endl;
        //for (int r = 1; r <= n; r++)
        //{
        //    for (int c = 1; c <= n; c++)
        //    {
        //        cout << map[r][c].size() << " ";
        //    }
        //    cout << endl;
        //}
    }
    
    cout << score << '\n';
    return 0;
}