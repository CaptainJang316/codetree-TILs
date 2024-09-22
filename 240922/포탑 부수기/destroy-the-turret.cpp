// 7:25 ~ 8:40, 12:51 ~ 3:02 <-- 3시간 25분 소요;;

// ** 최단거리는 분명 BFS인데, 왔던 경로 정보를 알아야 한다... 이건 또 DFS 아닌가....
//    => BFS로 최단거리 파악하고, DFS에서 방향 우선순위로 재귀시키되, BFS로 구한 최단거리보다 길어지면 아웃시키는 방법으로 구했다..
//    ====> 이게 최선일까...?
//    (** 중요! **)
//    => 최단거리인 만큼 BFS로 구현하되, 경로를 저장할 방법도 있다!
//    ===> 현재 위치에 대해 그 전 위치가 어디였는지를 vector<vector<Spot>> back 으로 저장하면 된다!
//         현재의 cr,cc에 대해 back[cr][cc]의 Spot값 r,c는 이전 위치 정보가 저장되므로, 이렇게 타고 가면 된다!!  


// ** '아직 살아있는 것들 중에' 각각 우선순위가 반대인 양끝 정보를 매 턴마다 추출해서 써야 한다..
//    => 우선순위 큐 + 매턴마다 죽은 것 erase로 털어내기 썼는데.. 더 좋은 방법이 있을까?

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// 우/하/좌/상의 우선순위대로
int dr[] = { 0, 1, 0, -1 };
int dc[] = { 1, 0, -1, 0 };

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
};

int N, M, K;
vector<vector<int>> map(10, vector<int>(10, 0));
vector<vector<int>> visited(10, vector<int>(10, 0));
vector<vector<Spot>> back(10, vector<Spot>(10));
vector<vector<int>> isAttacked(10, vector<int>(10, 0));
deque< Top> dq;
int maxScore, ar, ac, tr, tc;

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

// 최단 경로가 정해졌으면, 공격 대상에는 '공격자의 공격력 만큼'의 피해를 입히며, 피해를 입은 포탑은 '해당 수치만큼 공격력이 줄어듭니다'. 
// 또한 공격 대상을 제외한 '레이저 경로에 있는 포탑도' 공격을 받게 되는데, 이 포탑은 공격자 공격력의 '절반 만큼의 공격'을 받습니다. (절반이라 함은 공격력을 2로 나눈 몫을 의미합니다.)
void lazerAttack(int turnCnt) {
    map[tr][tc] -= map[ar][ac];
    isAttacked[tr][tc] = turnCnt;
    int v = map[ar][ac] / 2;

    int cr = tr;
    int cc = tc;

    while (true)
    {
        int br = back[cr][cc].r;
        int bc = back[cr][cc].c;

        if (br == ar && bc == ac) break;

        map[br][bc] -= v;
        isAttacked[br][bc] = turnCnt;

        cr = br;
        cc = bc;
    }
}

bool laserBFS(int turnCnt) {
    queue< Spot> q;
    q.push({ ar, ac });
    visited[ar][ac] = turnCnt;

    while (!q.empty())
    {
        int r = q.front().r;
        int c = q.front().c;
        q.pop();

        if (r == tr && c == tc)
        {
            lazerAttack(turnCnt);
            return true;
        }

        for (int d = 0; d < 4; d++)
        {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (isOutOfBound_R(nr)) nr = changeR(nr);
            if (isOutOfBound_C(nc)) nc = changeC(nc);

            if (map[nr][nc] > 0 && visited[nr][nc] != turnCnt)
            {
                visited[nr][nc] = turnCnt;
                q.push({ nr,nc });
                back[nr][nc] = { r,c };
            }
        }
    }
    return false;
}

//int getShortcutBFS(int sr, int sc, int turnCnt) {
//    queue<SpotInfo> q;
//    q.push({ sr, sc, 0 });
//    visited2[sr][sc] = turnCnt;
//
//    while (!q.empty())
//    {
//        int r = q.front().r;
//        int c = q.front().c;
//        int depth = q.front().depth;
//        q.pop();
//        
//        for (int d = 0; d < 4; d++)
//        {
//            int nr = r + dr[d];
//            int nc = c + dc[d];
//
//            if (isOutOfBound_R(nr)) nr = changeR(nr);
//            if (isOutOfBound_C(nc)) nc = changeC(nc);
//
//            if (nr == tr && nc == tc) return depth + 1;
//
//            if (map[nr][nc] > 0 && visited2[nr][nc] != turnCnt)
//            {
//                visited2[nr][nc] = turnCnt;
//                q.push({ nr, nc, depth + 1 });
//            }
//        }
//    }
//
//    return -1;
//}

//void laserDFS(int cnt, int turnCnt) {
//    if (cnt > shortCut) return;
//
//    int r = w[w.size() - 1].r;
//    int c = w[w.size() - 1].c;
//
//    if (r == tr && c == tc)
//    {
//        isComplete = true;
//        lazerAttack(turnCnt);
//        return;
//    }
//
//    for (int d = 0; d < 4; d++)
//    {
//        int nr = r + dr[d];
//        int nc = c + dc[d];
//
//        if (isOutOfBound_R(nr)) nr = changeR(nr);
//        if (isOutOfBound_C(nc)) nc = changeC(nc);
//
//        if (map[nr][nc] > 0 && visited[nr][nc] != 1)
//        {
//            visited[nr][nc] = 1;
//            w.push_back({ nr, nc });
//            laserDFS(cnt + 1, turnCnt);
//            w.pop_back();
//            visited[nr][nc] = 0;
//
//            if (isComplete) return;
//        }
//    }
//}

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

        if (map[nr][nc] > 0 && (nr != ar || nc != ac)) {
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
            if (map[r][c] > 0) dq.push_back({ r, c });
        }
    }

    for (int k = 1; k <= K; k++)
    {
        sort(dq.begin(), dq.end(), cmp);

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

        if (!laserBFS(k))
        {
            bombAttack(k);
        }

        for (int i = 0; i < dq.size(); i++)
        {
            int r = dq[i].r;
            int c = dq[i].c;

            if (isAttacked[r][c] != k) map[r][c] += 1;
        }

        dq.erase(remove_if(dq.begin(), dq.end(), [&](Top& a) {
            return map[a.r][a.c] <= 0;
            }), dq.end());

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