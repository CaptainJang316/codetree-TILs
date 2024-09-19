// 8:12 ~ 

// 당신은 고고학자로서 5×5 격자 내에서 3×3 격자를 선택하여 격자를 회전시킬 수 있습니다. 
// 선택된 격자는 시계 방향으로 90도, 180도, 270도 중 '하나의 각도'만큼 회전시킬 수 있습니다. 
// 단, 선택된 격자는 항상 회전을 진행해야만 합니다.


// [1] 탐사 진행
// 가능한 회전의 방법 중 
// (1) 유물 1차 획득 가치를 최대화하고, 
// 
// 그러한 방법이 여러가지인 경우 
// (2) 회전한 각도가 가장 작은 방법을 선택합니다. 
// 
// 그러한 경우도 여러가지인 경우 
// (3) 회전 중심 좌표의 '열'이 가장 '작은' 구간을, 
//     그리고 열이 같다면 '행'이 가장 '작은' 구간을 선택합니다.


// [2] 유물 획득
// 
// * 유물 1차 획득
// 상하좌우로 '인접한' '같은 종류의 유물 조각'은 서로 연결되어 있습니다. <-- BFS
// 이 조각들이 3개 이상 연결된 경우, 조각이 모여 유물이 되고 사라집니다. <-- 3개 이상 모이면 삭제
// 
// 유물의 가치는 모인 조각의 개수와 같습니다. <- 모은 조각 size

// 유적의 벽면에는 1부터 7 사이의 숫자가 M개 적혀 있습니다. 
// 이들은 유적에서 조각이 사라졌을 때 '새로 생겨나는 조각'에 대한 정보를 담고 있습니다.

// 조각이 사라진 위치에는 유적의 벽면에 '적혀있는 순서대로' 새로운 조각이 생겨납니다. 
// 새로운 조각은 
// (1) 열 번호가 '작은 순'으로 조각이 생겨납니다. 
// 만약 열 번호가 같다면 
// (2) 행 번호가 '큰 순'으로 조각이 생겨납니다. 
// 
// 단, 벽면의 숫자는 충분히 많이 적혀 있어 생겨날 조각의 수가 '부족한 경우는 없다'고 가정해도 좋습니다.

// 단, 유적의 벽면에 써 있는 숫자를 사용한 이후에는 다시 사용할 수 없으므로, 이후 부터는 남은 숫자부터 순서대로 사용합니다. 
// 즉, 이후에는 아래 그림에서 8번째로 적혀있는 수부터 다시 사용이 가능합니다.


// * 유물 연쇄 획득
// 새로운 유물 조각이 생겨난 이후에도 조각들이 3개 이상 연결될 수 있습니다.
// 이 경우 앞과 같은 방식으로 조각이 모여 유물이 되고 사라집니다.
// 사라진 위치에는 또다시 새로운 조각이 생겨나며 이는 더 이상 조각이 3개 이상 연결되지 않아 '유물이 될 수 없을 때까지 반복'됩니다.


// [3] 탐사 반복
// 
// 이 문제에서는 탐사 진행 ~유물 연쇄 획득의 과정까지를 1턴으로 생각하며, 총 K 번의 턴에 걸쳐 진행됩니다.
// 각 턴마다 획득한 유물의 가치의 총합을 출력하는 프로그램을 작성해야 합니다.
// 단, 아직 K번의 턴을 진행하지 못했지만, 탐사 진행 과정에서 어떠한 방법을 사용하더라도 '유물을 획득할 수 없었다면' 모든 '탐사는 그 즉시 종료'됩니다.
// ** 이 경우 얻을 수 있는 유물이 존재하지 않음으로, 종료되는 턴에 아무 값도 출력하지 않음에 유의합니다.


//첫 번째 줄에 탐사의 반복 횟수 K와 벽면에 적힌 유물 조각의 개수 M이 공백을 사이에 두고 주어집니다.
//
//그 다음 5개의 줄에 걸쳐 유물의 각 행에 있는 유물 조각에 적혀 있는 숫자들이 공백을 사이에 두고 순서대로 주어집니다.
//
//그 다음 줄에는 벽면에 적힌 M개의 유물 조각 번호가 공백을 사이에 두고 순서대로 주어집니다.
//
//단, 초기에 주어지는 유적지에서는 탐사 진행 이전에 유물이 발견되지 않으며, 첫 번째 턴에서 탐사를 진행한 이후에는 항상 유물이 발견됨을 가정해도 좋습니다.
//
//1≤K≤10
//10≤M≤300
//1≤ 유물 조각 번호 ≤7
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct SpotInfo
{
    int r, c, n;
};

struct Spot
{
    int r, c;
};

int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

int K, M;
vector<vector<int>> map(5, vector<int>(5, 0));
vector<vector<int>> bestMap(5, vector<int>(5, 0));
vector<int> p(300);
queue<SpotInfo> q;
queue<Spot> deleteQ;
int cCnt, maxCnt, bd, br, bc;
int score;
int pCnt;
bool isExist;
bool isEnd;

void rotate(int sr, int sc, vector<vector<int>> &cMap) {
    vector<vector<int>> temp(cMap);
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            temp[c + sr][3 - r - 1 + sc] = cMap[r + sr][c + sc];
        }
    }
    cMap = temp;
}

bool isOutOfBound(int r, int c) {
    return r < 0 || 5 <= r || c < 0 || 5 <= c;
}

void findRelicsBFS(int sr, int sc, vector<vector<int>>& map, vector<vector<int>>& visited, int cnt) {
    q.push({ sr, sc, map[sr][sc]});
    visited[sr][sc] = cnt;
    int currCnt = 1;

    while (!q.empty())
    {
        int r = q.front().r;
        int c = q.front().c;
        int n = q.front().n;
        q.pop();

        for (int d = 0; d < 4; d++)
        {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (isOutOfBound(nr, nc)) continue;

            if (map[nr][nc] == n && visited[nr][nc] != cnt)
            {
                currCnt++;
                visited[nr][nc] = cnt;
                q.push({ nr, nc, n });
            }
        }
    }

    if (currCnt >= 3) cCnt += currCnt;
    return;
}

// 그러한 방법이 여러가지인 경우 
// (2) 회전한 각도가 가장 작은 방법을 선택합니다. 
// 
// 그러한 경우도 여러가지인 경우 
// (3) 회전 중심 좌표의 '열'이 가장 '작은' 구간을, 
//     그리고 열이 같다면 '행'이 가장 '작은' 구간을 선택합니다.
void getSimulation(int sr, int sc, int cnt, int d, vector<vector<int>> &cMap, vector<vector<int>> &visited) {
    cCnt = 0;
    for (int r = 0; r < 5; r++)
    {
        for (int c = 0; c < 5; c++)
        {
            if (visited[r][c] != cnt) findRelicsBFS(r, c, cMap, visited, cnt);
        }
    }

    if (maxCnt < cCnt)
    {
        maxCnt = cCnt;
        bestMap = cMap;
        bd = d;
        br = sr;
        bc = sc;
    }
    else if (maxCnt > 0 && maxCnt == cCnt)
    {
        if (d < bd)
        {
            bestMap = cMap;
            bd = d;
            br = sr;
            bc = sc;
        }
        else if (d == bd)
        {
            if (sc < bc)
            {
                bestMap = cMap;
                bd = d;
                br = sr;
                bc = sc;
            }
            else if (sc == bc)
            {
                if (sr < br)
                {
                    bestMap = cMap;
                    bd = d;
                    br = sr;
                    bc = sc;
                }
            }
        }
    }
}

void findMaxCase() {
    vector<vector<int>> cMap(map);
    vector<vector<int>> visited(5, vector<int>(5, 0));
    int cnt = 1;
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            for (int d = 0; d < 3; d++)
            {
                rotate(r, c, cMap);
                getSimulation(r, c, cnt, d, cMap, visited);
                cnt++;
            }
            cMap = map;
        }
    }

    if (maxCnt > 0) {
        //cout << "maxCnt: " << maxCnt << endl;
        isExist = true;
        map = bestMap;
    }
    return;
}

void getRelicsBFS(int sr, int sc, vector<vector<int>> &visited, int cnt) {
    vector<Spot> pList;
    q.push({ sr, sc, map[sr][sc] }); 
    visited[sr][sc] = cnt;
    pList.push_back({ sr, sc });

    while (!q.empty())
    {
        int r = q.front().r;
        int c = q.front().c;
        int n = q.front().n;
        q.pop();

        for (int d = 0; d < 4; d++)
        {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (isOutOfBound(nr, nc)) continue;

            if (map[nr][nc] == n && visited[nr][nc] != cnt)
            {
                visited[nr][nc] = cnt;
                q.push({ nr, nc, n });
                pList.push_back({ nr, nc });
            }
        }
    }

    if (pList.size() >= 3) {
        for (int i = 0; i < pList.size(); i++)
        {
            isEnd = false;
            map[pList[i].r][pList[i].c] = -1;
        }
        score += pList.size();
    }
    pList.clear();
}

int main() {
    score = 0;
    pCnt = 0;

    cin >> K >> M;
    for (int r = 0; r < 5; r++)
    {
        for (int c = 0; c < 5; c++)
        {
            cin >> map[r][c];
        }
    }

    for (int i = 0; i < M; i++)
    {
        cin >> p[i];
    }

    for (int t = 1; t <= K; t++)
    {
        score = 0;
        maxCnt = 0;
        isExist = false;
        findMaxCase();

        if (!isExist) break;
        //cout << "qwer\n";

        //cout << "maxCnt: " << maxCnt << endl;

        vector<vector<int>> visited(5, vector<int>(5, 0));
        int cnt = 0;
        while (true)
        {
            isEnd = true;
            cnt++;
            for (int r = 0; r < 5; r++)
            {
                for (int c = 0; c < 5; c++)
                {
                    if(visited[r][c] != cnt) getRelicsBFS(r, c, visited, cnt);
                }
            }

            if (isEnd) break;

            for (int c = 0; c < 5; c++)
            {
                for (int r = 4; r >= 0; r--)
                {
                    if (map[r][c] == -1) {
                        map[r][c] = p[pCnt];
                        pCnt++;
                    }
                }
            }

            //cout <<"=== 한 턴 끝 ===" << endl;
            //for (int r = 0; r < 5; r++)
            //{
            //    for (int c = 0; c < 5; c++)
            //    {
            //        cout << map[r][c] << ' ';
            //    }
            //    cout << endl;
            //}
        }

        cout << score << " ";
    }
    cout << '\n';

    return 0;
}