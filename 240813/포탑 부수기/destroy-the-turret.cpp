// 11:37 ~ 1:45, 4:50 ~ 6:10, 10:15 ~ 
// 
// // N×M 격자가 있고, 모든 위치에는 포탑이 존재합니다. (즉, 포탑의 개수는 N*M개)
// 
// 각 포탑에는 공격력이 존재하며, 상황에 따라 공격력이 줄어들거나 늘어날 수 있습니다. 
// 또한, 공격력이 0 이하가 된다면, 해당 포탑은 부서지며 더 이상의 공격을 할 수 없습니다. 
// 
// 최초에 공격력이 0인 포탑 즉, 부서진 포탑이 존재할 수 '있습니다.'

// 하나의 턴은 다음의 4가지 액션을 순서대로 수행하며, 총 K번 반복됩니다.
// 만약 부서지지 않은 포탑이 1개가 된다면 그 즉시 중지됩니다.

// 1. 공격자 선정
// 
// 부서지지 않은 포탑 중 '가장 약한 포탑이 공격자로 선정'됩니다.
// 공격자로 선정되면 N + M만큼의 공격력이 증가됩니다.

// 가장 약한 포탑은 다음의 기준으로 선정됩니다.
// 
// 1. 공격력이 가장 '낮은' 포탑이 가장 약한 포탑입니다.
// 2. 만약 공격력이 가장 낮은 포탑이 2개 이상이라면, 가장 '최근에 공격한 포탑'이 가장 약한 포탑입니다. (모든 포탑은 시점 0에 모두 공격한 경험이 있다고 가정하겠습니다.)
// 3. 만약 그러한 포탑이 2개 이상이라면, 각 포탑 위치의 '행과 열의 합이 가장 큰 포탑'이 가장 약한 포탑입니다.
// 4. 만약 그러한 포탑이 2개 이상이라면, 각 포탑 위치의 '열 값이 가장 큰 포탑'이 가장 약한 포탑입니다.


// 2. 공격자의 공격
//
// 위에서 선정된 공격자는 '자신을 제외한 가장 강한 포탑을 공격'합니다.
// 가장 강한 포탑은 위에서 정한 가장 약한 포탑 선정 기준의 반대이며, 다음과 같습니다.
//
// 1. 공격력이 가장 높은 포탑이 가장 강한 포탑입니다.
// 2. 만약 공격력이 가장 높은 포탑이 2개 이상이라면, '공격한지 가장 오래된 포탑'이 가장 강한 포탑입니다. (모든 포탑은 시점 0에 모두 공격한 경험이 있다고 가정하겠습니다.)
// 3. 만약 그러한 포탑이 2개 이상이라면, 각 포탑 위치의 '행과 열의 합이 가장 작은 포탑'이 가장 강한 포탑입니다.
// 4. 만약 그러한 포탑이 2개 이상이라면, 각 포탑 위치의 '열 값이 가장 작은 포탑'이 가장 강한 포탑입니다.

// 공격을 할 때에는 '레이저 공격'을 먼저 시도하고, 만약 그게 안 된다면 '포탄 공격'을 합니다. 각 공격의 규칙은 다음과 같습니다.

// 1) 레이져 공격
// - DFS로 최단거리로 이동 <-- 부서진 포탑은 지날 수 없기에, 혹시 가능한 경로가 없다면 포탄 공격ㅇㅇ
//	 => 이 때, 범위 벗어나면 반대쪽으로 이어짐ㅇㅇ
//		=> 이동이 가능하다면, 공격 대상은 공격력만큼, 경로 내의 포탑들은 공격력/2 만큼 감소ㅇㅇ
//		=> 이동 가능한 경로가 여러개라면, 우/하/좌/상의 우선순위대로 먼저 움직인 경로가 선택됨. <-- 따라서, 방향 우선순위대로 돌려서 먼저 나오는 경로로 하면 됨.

// 2) 포탄 공격
// - 주위 8개의 방향에 있는 포탑도 피해를 입는데, 공격자 공격력의 절반 만큼의 피해를 받습니다. 
//	  => 이 때도 가장자리면, 반대쪽의 포탑들이 피해받음ㅇㅇ
// - 공격자는 해당 공격에 영향을 받지 않는다.


// 3. 포탑 부서짐
// 
// 공격을 받아 공격력이 0 이하가 된 포탑은 부서집니다.
// 
// 
// 4. 포탑 정비
// 
// 공격이 끝났으면, 부서지지 않은 포탑 중 '공격과 무관했던 포탑'은 공격력이 1씩 올라갑니다.
// 공격과 무관하다는 뜻은 
// - 공격자도 아니고, 
// - 공격에 피해를 입은 포탑도 아니라는 뜻입니다.


// 전체 과정이 종료된 후 남아있는 포탑 중 '가장 강한 포탑의 공격력'을 출력하는 프로그램을 작성해보세요.

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Info
{
    int r, c, p;
    int t = 0;
};

int dr[] = { 0, 1, 0, -1 };
int dc[] = { 1, 0, -1, 0 };

int eDr[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
int eDc[] = { -1, 0, 1, 1, 1, 0, -1, -1 };

//첫 번째 줄에 N, M, K가 공백을 사이에 두고 주어집니다.
int N, M, K;
vector<vector<int>> wayMap(11, vector<int>(11, 0));
vector<vector<int>> map(11, vector<int>(11, 0));
vector<vector<int>> visited(11, vector<int>(11, 0));
vector<Info> tList;
int minD;
int a, t;
bool isWayExist;
//두 번째 줄부터 N개의 줄에 걸쳐서 N×M 격자에 대한 정보가 주어집니다.단, 최초에 부서지지 않은 포탑은 최소 2개 이상 존재합니다.

void laserWayDFS(int r, int c, int depth) {
    if (minD <= depth) return;

    for (int d = 0; d < 4; d++)
    {
        int nr = r + dr[d];
        int nc = c + dc[d];

        if (nr < 1) nr = N;
        else if (nr > N) nr = 1;
        else if (nc < 1) nc = M;
        else if (nc > M) nc = 1;

        if (nr == tList[t].r && nc == tList[t].c)
        {
            minD = depth + 1;
            wayMap = visited;
            isWayExist = true;
            return;
        }
        
        if (0 < map[nr][nc] && visited[nr][nc] != 1)
        {
            visited[nr][nc] = 1;
            laserWayDFS(nr, nc, depth + 1);
            visited[nr][nc] = 0;
        }
    }

    return;
}

void bomb() {
    int r = tList[t].r;
    int c = tList[t].c;
    fill(wayMap.begin(), wayMap.end(), vector<int>(11, 0));

    for (int d = 0; d < 8; d++)
    {
        int nr = r + eDr[d];
        int nc = c + eDc[d];

        if (nr < 1) nr = N;
        else if (nr > N) nr = 1;
        else if (nc < 1) nc = M;
        else if (nc > M) nc = 1;

        if (0 < map[nr][nc]) {
            wayMap[nr][nc] = 1;
        }
    }

    tList[t].p -= tList[a].p;
    map[tList[t].r][tList[t].c] = tList[t].p;

    for (int i = 0; i < tList.size(); i++)
    {
        int r = tList[i].r;
        int c = tList[i].c;
        if (wayMap[r][c] == 1 && (r != tList[a].r || c != tList[a].c))
        {
            tList[i].p -= tList[a].p / 2;
            map[tList[i].r][tList[i].c] = tList[i].p;
        }
    }
}

int main() {
    cin >> N >> M >> K;
    for (int r = 1; r <= N; r++)
    {
        for (int c = 1; c <= M; c++)
        {
            cin >> map[r][c];
            if (map[r][c] != 0) tList.push_back({ r, c, map[r][c] });
        }
    }

    for (int k = 1; k <= K; k++)
    {
        minD = 10000;
        isWayExist = false;

        // 공격자 선정
        a = 0;
        for (int i = 1; i < tList.size(); i++)
        {
            if (tList[a].p > tList[i].p) a = i;
            else if (tList[a].p == tList[i].p)
            {
                if (tList[a].t < tList[i].t) a = i;
                else if(tList[a].t == tList[i].t)
                {
                    if (tList[a].r + tList[a].c < tList[i].r + tList[i].c) a = i;
                    else if(tList[a].r + tList[a].c == tList[i].r + tList[i].c)
                    {
                        if (tList[a].c < tList[i].c) a = i;
                    }
                }
            }
        }

        // 타겟 선정
        t = 0;
        for (int i = 1; i < tList.size(); i++)
        {
            if (tList[t].p < tList[i].p) t = i;
            else if (tList[t].p == tList[i].p)
            {
                if (tList[t].t > tList[i].t) t = i;
                else if (tList[t].t == tList[i].t)
                {
                    if (tList[t].r + tList[t].c > tList[i].r + tList[i].c) t = i;
                    else if (tList[t].r + tList[t].c == tList[i].r + tList[i].c)
                    {
                        if (tList[t].c > tList[i].c) t = i;
                    }
                }
            }
        }

        tList[a].t = k;
        tList[a].p += (N + M);
        map[tList[a].r][tList[a].c] = tList[a].p;

        //cout << endl;
        //for (int r = 1; r <= N; r++)
        //{
        //    for (int c = 1; c <= M; c++)
        //    {
        //        cout << map[r][c] << " ";
        //    }
        //    cout << endl;
        //}

        visited[tList[a].r][tList[a].c] = 1;
        laserWayDFS(tList[a].r, tList[a].c, 0);

        if (isWayExist)
        {
            tList[t].p -= tList[a].p;
            map[tList[t].r][tList[t].c] = tList[t].p;

            for (int i = 0; i < tList.size(); i++)
            {
                if ((tList[i].r != tList[a].r || tList[i].c != tList[a].c) && (tList[i].r != tList[t].r || tList[i].c != tList[t].c))
                {
                    if (wayMap[tList[i].r][tList[i].c] == 1)
                    {
                        tList[i].p -= (tList[a].p / 2);
                        map[tList[i].r][tList[i].c] = tList[i].p;
                    }
                }
            }
            visited[tList[a].r][tList[a].c] = 0;
        }
        else {
            visited[tList[a].r][tList[a].c] = 0;
            bomb();
        }

        //cout << endl;
        //for (int r = 1; r <= N; r++)
        //{
        //    for (int c = 1; c <= M; c++)
        //    {
        //        cout << map[r][c] << " ";
        //    }
        //    cout << endl;
        //}

        int tR = tList[t].r;
        int tC = tList[t].c;

        tList.erase(remove_if(tList.begin(), tList.end(), [](Info& t) {
            return t.p <= 0;
            }), tList.end());

        if (tList.size() <= 1) break;

        for (int i = 0; i < tList.size(); i++)
        {
            int r = tList[i].r;
            int c = tList[i].c;
            // Bomb일 때는 또 어떻게 할지 고민해야 함.
            if (wayMap[r][c] != 1 && (r != tR || c != tC))
            {
                tList[i].p += 1;
                map[r][c] = tList[i].p;
            }
        }

        //cout << endl;
        //for (int r = 1; r <= N; r++)
        //{
        //    for (int c = 1; c <= M; c++)
        //    {
        //        cout << map[r][c] << " ";
        //    }
        //    cout << endl;
        //}
    }
    
    int maxP = tList[0].p;
    for (int i = 0; i < tList.size(); i++)
    {
        maxP = max(maxP, tList[i].p);
    }
    cout << maxP << '\n';
    return 0;
}