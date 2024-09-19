// 8:10 ~ 10:00 <-- 1시간 50분

// ** 이상하고도, 아주 치명적인 실수를 할 뻔 했다.
//    => visited를 여러 반복적인 실행에 재사용하기 위해 cnt와 함께 재활용할 때, 
//       cnt 값을 올려주는 위치는 저어어얼대 r&c for문 안에 놓으면 안된다. 절대 안된다!!

// 아래와 같이, cnt는 r&c for문 바깥에서 값을 바꾸도록
// 즉, visited에 사용하기 위한 cnt는 값 하나가 맵 전체를 다 돌 때까진 값이 절대 변하면 안된다!!
// 
//cnt++; <-- 여기에 놔야 한다! for문 안에 들어가면 큰일난다.
//for (int r = 0; r < 5; r++)
//{
//    for (int c = 0; c < 5; c++)
//    {
//        if (visited[r][c] != cnt) getRelicsBFS(r, c, visited, cnt);
//    }
//}

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
        }

        cout << score << " ";
    }
    cout << '\n';

    return 0;
}