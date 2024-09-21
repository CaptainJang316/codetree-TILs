// 7:25 ~ 8:40, 12:51 ~ 

// N×M 격자가 있고, 모든 위치에는 포탑이 존재합니다. (즉, 포탑의 개수는 NM개)

// 각 포탑에는 공격력이 존재하며, 상황에 따라 공격력이 줄어들거나 늘어날 수 있습니다. 
// 또한, 공격력이 0 이하가 된다면, 해당 포탑은 부서지며 더 이상의 공격을 할 수 없습니다. 
// 
// 최초에 공격력이 0인 포탑 즉, 부서진 포탑이 존재할 수 '있습니다'.

// 하나의 턴은 다음의 4가지 액션을 '순서대로' 수행하며, 총 K번 반복됩니다.
// 만약 부서지지 않은 포탑이 1개가 된다면 그 '즉시 중지'됩니다.

// 1. 공격자 선정
// 
// 부서지지 않은 포탑 중 가장 '약한' 포탑이 '공격자로 선정'됩니다.
// 공격자로 선정되면, N + M만큼의 공격력이 '증가'됩니다.

// 가장 약한 포탑은 다음의 기준으로 선정됩니다.
// 
// 1. 공격력이 '가장 낮은' 포탑이 가장 약한 포탑입니다.
// 2. 만약 공격력이 가장 낮은 포탑이 2개 이상이라면, 가장 '최근에 공격한 포탑'이 가장 약한 포탑입니다. (모든 포탑은 시점 0에 모두 공격한 경험이 있다고 가정하겠습니다.)
// 3. 만약 그러한 포탑이 2개 이상이라면, 각 포탑 위치의 '행과 열의 합'이 '가장 큰' 포탑이 가장 약한 포탑입니다.
// 4. 만약 그러한 포탑이 2개 이상이라면, 각 포탑 위치의 '열 값이 가장 큰' 포탑이 가장 약한 포탑입니다.


// 2. 공격자의 공격
// 
// 위에서 선정된 공격자는 '자신을 제외한' '가장 강한 포탑을' 공격합니다.
// 가장 강한 포탑은 위에서 정한 가장 약한 포탑 선정 기준의 반대이며, 다음과 같습니다.
// 
// 1. 공격력이 '가장 높은' 포탑이 가장 강한 포탑입니다.
// 2. 만약 공격력이 가장 높은 포탑이 2개 이상이라면, 공격한지 '가장 오래된 포탑'이 가장 강한 포탑입니다. (모든 포탑은 시점 0에 모두 공격한 경험이 있다고 가정하겠습니다.)
// 3. 만약 그러한 포탑이 2개 이상이라면, 각 포탑 위치의 '행과 열의 합'이 '가장 작은' 포탑이 가장 강한 포탑입니다.
// 4. 만약 그러한 포탑이 2개 이상이라면, 각 포탑 위치의 '열 값이 가장 작은' 포탑이 가장 강한 포탑입니다.

// 공격을 할 때에는 레이저 공격을 먼저 시도하고, 
// 만약 그게 안 된다면 포탄 공격을 합니다. 각 공격의 규칙은 다음과 같습니다.

// (1) 레이저 공격
// 레이저는 다음의 규칙으로 움직입니다.

// 1. 상하좌우의 4개의 방향으로 움직일 수 있습니다.
// 2. 부서진 포탑이 있는 위치는 지날 수 '없습니다'.
// 3. 가장자리에서 막힌 방향으로 진행하고자 한다면, 반대편으로 나옵니다.(양 끝은 연결되어 있다)

// 레이저 공격은 공격자의 위치에서 공격 대상 포탑까지의 '최단 경로'로 공격합니다. 
// 만약 그러한 경로가 존재하지 않는다면(갈 수 없다면)
// (2) 포탄 공격을 진행합니다. 만약 경로의 길이가 똑같은 최단 경로가 2개 이상이라면, 우/하/좌/상의 우선순위대로 '먼저 움직인 경로'가 선택됩니다.

// 최단 경로가 정해졌으면, 공격 대상에는 '공격자의 공격력 만큼'의 피해를 입히며, 피해를 입은 포탑은 '해당 수치만큼 공격력이 줄어듭니다'. 
// 또한 공격 대상을 제외한 '레이저 경로에 있는 포탑도' 공격을 받게 되는데, 이 포탑은 공격자 공격력의 '절반 만큼의 공격'을 받습니다. (절반이라 함은 공격력을 2로 나눈 몫을 의미합니다.)


// (2) 포탄 공격
// 
// 공격 대상에 포탄을 던집니다.
// 공격 대상은 '공격자 공격력 만큼'의 피해를 받습니다.
// 
// 추가적으로 주위 8개의 방향에 있는 포탑도 피해를 입는데, 공격자 공격력의 '절반 만큼의 피해'를 받습니다.  
// '공격자는' 해당 공격에 영향을 받지 '않습니다'.
// 
// 만약 가장자리에 포탄이 떨어졌다면, 위에서의 레이저 이동처럼 포탄의 추가 피해가 '반대편 격자에 미치게 됩니다'.

// 3. 포탑 부서짐 
// 공격을 받아 공격력이 0 이하가 된 포탑은 부서집니다.

// 4. 포탑 정비
// 공격이 끝났으면, '부서지지 않은 포탑 중' '공격과 무관했던 포탑'은 공격력이 1씩 올라갑니다. 
// 공격과 무관하다는 뜻은 공격자도 아니고, 공격에 피해를 입은 포탑도 아니라는 뜻입니다.


// 전체 과정이 종료된 후 남아있는 포탑 중 '가장 강한 포탑의 공격력'을 출력하는 프로그램을 작성해보세요.

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

//첫 번째 줄에 N, M, K가 공백을 사이에 두고 주어집니다.
//
//두 번째 줄부터 N개의 줄에 걸쳐서 N×M 격자에 대한 정보가 주어집니다.단, 최초에 부서지지 않은 포탑은 최소 2개 이상 존재합니다.
//
//4≤N, M≤10
//1≤K≤1, 000
//0≤공격력≤5, 000
using namespace std;

// 우/하/좌/상의 우선순위대로
int dr[] = { 0, 1, 0, -1 };
int dc[] = { 1, 0, -1, 0 };

//struct SpotInfo
//{
//    int lr, lc, cr, cc;
//};

struct Spot
{
    int r, c;
};

struct SpotInfo
{
    int r, c, depth;
};

struct Top
{
    int r, c;
    int t = 0;
    //bool isChanged = false;
};

int N, M, K;
vector<vector<int>> map(10, vector<int>(10, 0));
vector<vector<int>> visited(10, vector<int>(10, 0));
vector<vector<int>> visited2(10, vector<int>(10, 0));
vector<vector<int>> isAttacked(10, vector<int>(10, 0));
//vector< Top> t;
vector< Spot> w;
deque< Top> dq;
int maxScore, ar, ac, tr, tc, shortCut;
bool isLazerPossible;

bool cmp(Top& a, Top& b) {
    if (map[a.r][a.c] == map[b.r][b.c])
    {
        if (a.t == b.t)
        {
            if (a.r + a.c == b.r + b.c)
            {
                return a.c > b.c;
            }
            return a.r + a.c > b.r + b.c;
        }
        return a.t > b.t;
    }
    return map[a.r][a.c] < map[b.r][b.c];
    // 1. 공격자 선정
    // 
    // 부서지지 않은 포탑 중 가장 '약한' 포탑이 '공격자로 선정'됩니다.
    // 공격자로 선정되면, N + M만큼의 공격력이 '증가'됩니다.

    // 가장 약한 포탑은 다음의 기준으로 선정됩니다.
    // 
    // 1. 공격력이 '가장 낮은' 포탑이 가장 약한 포탑입니다.
    // 2. 만약 공격력이 가장 낮은 포탑이 2개 이상이라면, 가장 '최근에 공격한 포탑'이 가장 약한 포탑입니다. (모든 포탑은 시점 0에 모두 공격한 경험이 있다고 가정하겠습니다.)
    // 3. 만약 그러한 포탑이 2개 이상이라면, 각 포탑 위치의 '행과 열의 합'이 '가장 큰' 포탑이 가장 약한 포탑입니다.
    // 4. 만약 그러한 포탑이 2개 이상이라면, 각 포탑 위치의 '열 값이 가장 큰' 포탑이 가장 약한 포탑입니다.
}

bool isOutOfBound_R(int r) {
    return r < 0 || N <= r;
}
bool isOutOfBound_C(int c) {
    return c < 0 || M <= c;
}
int changeR(int r) {
    if (r < 0) return N - 1;
    else if (N <= r) return 0;
}
int changeC(int c) {
    if (c < 0) return M - 1;
    else if (M <= c) return 0;
}

int getShortcutBFS(int sr, int sc, int turnCnt) {
    queue<SpotInfo> q;
    q.push({ sr, sc, 0 });
    visited2[sr][sc] = turnCnt;

    while (!q.empty())
    {
        int r = q.front().r;
        int c = q.front().c;
        int depth = q.front().depth;
        q.pop();
        
        for (int d = 0; d < 4; d++)
        {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (isOutOfBound_R(nr)) nr = changeR(nr);
            if (isOutOfBound_C(nc)) nc = changeC(nc);

            if (nr == tr && nc == tc) return depth + 1;

            if (map[nr][nc] > 0 && visited2[nr][nc] != turnCnt)
            {
                visited2[nr][nc] = turnCnt;
                q.push({ nr, nc, depth + 1 });
            }
        }
    }
}


// 최단 경로가 정해졌으면, 공격 대상에는 '공격자의 공격력 만큼'의 피해를 입히며, 피해를 입은 포탑은 '해당 수치만큼 공격력이 줄어듭니다'. 
// 또한 공격 대상을 제외한 '레이저 경로에 있는 포탑도' 공격을 받게 되는데, 이 포탑은 공격자 공격력의 '절반 만큼의 공격'을 받습니다. (절반이라 함은 공격력을 2로 나눈 몫을 의미합니다.)
void lazerAttack(int turnCnt) {
    map[tr][tc] -= map[ar][ac];
    isAttacked[tr][tc] = turnCnt;
    int v = map[ar][ac] / 2;

    //cout << "============\n";
    //for (int r = 0; r < N; r++)
    //{
    //    for (int c = 0; c < M; c++)
    //    {
    //        cout << visited[r][c] << " ";
    //    }
    //    cout << '\n';
    //}

    for (int i = w.size()-2; i >= 1; i--)
    {
        map[w[i].r][w[i].c] -= v;
        isAttacked[w[i].r][w[i].c] = turnCnt;
    }
}

void laserDFS(int cnt, int turnCnt) {
    if (cnt > shortCut) return;

    int r = w[w.size() - 1].r;
    int c = w[w.size() - 1].c;

    if (r == tr && c == tc)
    {
        isLazerPossible = true;
        lazerAttack(turnCnt);
        return;
    }

    for (int d = 0; d < 4; d++)
    {
        int nr = r + dr[d];
        int nc = c + dc[d];

        if (isOutOfBound_R(nr)) nr = changeR(nr);
        if (isOutOfBound_C(nc)) nc = changeC(nc);

        if (map[nr][nc] > 0 && visited[nr][nc] != 1)
        {
            visited[nr][nc] = 1;
            w.push_back({ nr, nc });
            laserDFS(cnt + 1, turnCnt);
            w.pop_back();
            visited[nr][nc] = 0;

            if (isLazerPossible) return;
        }
    }
}

// (2) 포탄 공격
// 
// 공격 대상에 포탄을 던집니다.
// 공격 대상은 '공격자 공격력 만큼'의 피해를 받습니다.
// 
// 추가적으로 주위 8개의 방향에 있는 포탑도 피해를 입는데, 공격자 공격력의 '절반 만큼의 피해'를 받습니다.  
// '공격자는' 해당 공격에 영향을 받지 '않습니다'.
// 
// 만약 가장자리에 포탄이 떨어졌다면, 위에서의 레이저 이동처럼 포탄의 추가 피해가 '반대편 격자에 미치게 됩니다'.
int bDr[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
int bDc[] = { -1, 0, 1, 1, 1, 0, -1, -1 };

void bombAttack(int turnCnt) {
    map[tr][tc] -= map[ar][ac];
    isAttacked[tr][tc] = turnCnt;
    int v = map[ar][ac] / 2;

    for (int d = 0; d < 8; d++)
    {
        int nr = tr + bDr[d];
        int nc = tc + bDc[d];

        if (isOutOfBound_R(nr)) nr = changeR(nr);
        if (isOutOfBound_C(nc)) nc = changeC(nc);

        if (map[nr][nc] > 0) {
            map[nr][nc] -= v;
            isAttacked[nr][nc] = turnCnt;
        }
    }
}

int main() {
    cin >> N >> M >> K;
    for (int r = 0; r < N; r++)
    {
        for (int c = 0; c < M; c++)
        {
            cin >> map[r][c];
            if (map[r][c] > 0) dq.push_back({ r, c, map[r][c]});
        }
    }

    for (int k = 1; k <= K; k++)
    {
        isLazerPossible = false;
        sort(dq.begin(), dq.end(), cmp);

        //bool isEnd = true;

        // 1개만 남게되면 걍 게임 끝
        if (dq.size() <= 1)
        {
            break;
        }

        ar = dq.front().r;
        ac = dq.front().c;
        dq.front().t = k;
        isAttacked[ar][ac] = k;

        tr = dq.back().r;
        tc = dq.back().c;

        //N + M만큼의 공격력이 '증가'됩니다.
        map[ar][ac] += (N + M);
        //cout << "map[ar][ac]: " << map[ar][ac] << endl;
        
        shortCut = getShortcutBFS(ar, ac, k);

        visited[ar][ac] = 1;
        w.push_back({ ar, ac });
        laserDFS(0, k);
        w.pop_back();
        visited[ar][ac] = 0;

        //for (int i = 0; i < t.size(); i++)
        //{
        //    if (t[i].isDead) continue;
        //    isEnd = false;
        //}
        if (!isLazerPossible)
        {
            bombAttack(k);

            //cout << "==== after bombAttack ====\n";
            //for (int r = 0; r < N; r++)
            //{
            //    for (int c = 0; c < M; c++)
            //    {
            //        cout << map[r][c] << " ";
            //    }
            //    cout << '\n';
            //}
        }
        //else {
        //    cout << "==== after laser ====\n";
        //    for (int r = 0; r < N; r++)
        //    {
        //        for (int c = 0; c < M; c++)
        //        {
        //            cout << map[r][c] << " ";
        //        }
        //        cout << '\n';
        //    }
        //}

        for (int i = 0; i < dq.size(); i++)
        {
            int r = dq[i].r;
            int c = dq[i].c;

            if (isAttacked[r][c] != k) map[r][c] += 1;
        }

        dq.erase(remove_if(dq.begin(), dq.end(), [&](Top& a) {
            return map[a.r][a.c] <= 0;
            }), dq.end());

        // 3. 포탑 부서짐 
        // 공격을 받아 공격력이 0 이하가 된 포탑은 부서집니다.

        // 4. 포탑 정비
        // 공격이 끝났으면, '부서지지 않은 포탑 중' '공격과 무관했던 포탑'은 공격력이 1씩 올라갑니다. 
        // 공격과 무관하다는 뜻은 공격자도 아니고, 공격에 피해를 입은 포탑도 아니라는 뜻입니다.

    }

    // 전체 과정이 종료된 후 남아있는 포탑 중 '가장 강한 포탑의 공격력'을 출력하는 프로그램을 작성해보세요.
    maxScore = 0;
    for (int i = 0; i < dq.size(); i++)
    {
        int r = dq[i].r;
        int c = dq[i].c;

        maxScore = max(maxScore, map[r][c]);
    }
    cout << maxScore << '\n';

    return 0;
}