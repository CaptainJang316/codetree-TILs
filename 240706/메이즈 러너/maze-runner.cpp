#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Runner {
    int r;
    int c;
    int isOut = false;
};

struct Spot {
    int r;
    int c;
};

struct SpotWithDepth {
    int r;
    int c;
    int depth;
};

struct SquareInfo {
    int r;
    int c;
    int size;
};

int dr[] = { -1, 1, 0, 0 };
int dc[] = { 0, 0, -1, 1 };

int bfsDr[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int bfsDc[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

int N, M, K;
int totalMCnt = 0;
vector<vector<int>> map(11, vector<int>(11, 0));
vector< Runner> runnerList;
Spot exitSpot;

bool isOutOfBound(int r, int c) {
    return r < 1 || N < r || c < 1 || N < c;
}

bool isCloser(int nr, int nc, int r, int c) {
    return abs(nr - exitSpot.r) + abs(nc - exitSpot.c) < abs(r - exitSpot.r) + abs(c - exitSpot.c);
}

void move() {

    //int nr, nc;
    //for (int r = 1; r <= N; r++)
    //{
    //    for (int c = 1; c <= N; c++)
    //    {
    //        if (map[r][c] == -1) {
    //            for (int dir = 0; dir < 4; dir++)
    //            {
    //                nr = r + dr[dir];
    //                nc = c + dc[dir];

    //                if (!isOutOfBound(nr, nc) && map[nr][nc] <= 0 && isCloser(nr, nc, r, c)) {
    //                    //map[r][c] = 0;
    //                    totalMCnt += 1;

    //                    // 출구에 도착했으면
    //                    if (nr == exitSpot.r && nc == exitSpot.c) {
    //                        break;
    //                    }

    //                    //map[nr][nc] = -1;
    //                    break;
    //                }
    //            }
    //        }
    //    }
    //}
    
    for (int i = 0; i < runnerList.size(); i++)
    {
        int nr, nc;
        if (!runnerList[i].isOut) {
            int r = runnerList[i].r;
            int c = runnerList[i].c;

            //cout << "runnerList[i]: " << r << " " << c << " " << runnerList[i].isOut << endl;

            for (int dir = 0; dir < 4; dir++)
            {
                nr = r + dr[dir];
                nc = c + dc[dir];

                if (!isOutOfBound(nr, nc) && map[nr][nc] <= 0 && isCloser(nr, nc, r, c)) {
                    //cout << "asdfasdfdsafdsafdsafadsf\n";
                    map[r][c] = 0;
                    totalMCnt += 1;

                    // 출구에 도착했으면
                    if (nr == exitSpot.r && nc == exitSpot.c) {
                        runnerList[i].isOut = true;
                        break;
                    }

                    map[nr][nc] = -1;
                    runnerList[i].r = nr;
                    runnerList[i].c = nc;
                    break;
                }
            }
        }
    }

    runnerList.erase(remove_if(runnerList.begin(), runnerList.end(), [](const Runner& runner) {
        return runner.isOut == true;
    }), runnerList.end());
}

bool compare(SquareInfo& spot1, SquareInfo& Spot2) {
    if (spot1.r == Spot2.r) return spot1.c < Spot2.c;
    return spot1.r < Spot2.r;
}

void rotate(SquareInfo squreEdge) {
    vector<vector<int>> map_copy(map);
    bool isExitRotate = false;

    for (int r = 0; r <= squreEdge.size; r++)
    {
        for (int c = 0; c <= squreEdge.size; c++)
        {
            // 회전 범위 내의 벽 내구도 -=1
            if (0 < map_copy[squreEdge.r + r][squreEdge.c + c]) map_copy[squreEdge.r + r][squreEdge.c + c] -= 1;

            // 한 번 회전했으면 또 회전되진 않도록 flag 처리 필요함.
            // 또 그렇다고, 반복을 피하겠다고 반복문 밖에서 처리하면 안됨. 회전 범위는 그것이 특정 좌표를 기준으로 만들어진, 새로운 map이라고 생각해야 한다. 기존의 r,c값은 (0,0)을 기준으로 한 값이므로, 안맞다. 
            if (!isExitRotate && squreEdge.r + r == exitSpot.r && squreEdge.c + c == exitSpot.c) {
                isExitRotate = true;

                exitSpot.r = squreEdge.r + c;
                exitSpot.c = squreEdge.c + squreEdge.size - r;
                //cout << "exitSpot: " << exitSpot.r << " " << exitSpot.c << endl;
            }

            map[squreEdge.r + c][squreEdge.c + squreEdge.size - r] = map_copy[squreEdge.r + r][squreEdge.c + c];
        }
    }

    for (int i = 0; i < runnerList.size(); i++)
    {
        if (!runnerList[i].isOut) {
            // Runner의 위치가 현재 회전할 영역 범위 내에 있다면,
            if (squreEdge.r <= runnerList[i].r && runnerList[i].r <= squreEdge.r + squreEdge.size) {
                if (squreEdge.c <= runnerList[i].c && runnerList[i].c <= squreEdge.c + squreEdge.size) {

                    int diffR = runnerList[i].r - squreEdge.r;
                    int diffC = runnerList[i].c - squreEdge.c;

                    runnerList[i].r = squreEdge.r + diffC;
                    runnerList[i].c = squreEdge.c + squreEdge.size - diffR;
                }
            }
        }
        
    }
}

void findSqureBFS() {
    queue<SpotWithDepth> q;
    vector<vector<int>> visited(11, vector<int>(11, 0));
    q.push({ exitSpot.r, exitSpot.c, 0 });
    visited[exitSpot.r][exitSpot.c] = 1;

    int findDepth = 1000;
    vector<Spot> candidateList;
    vector<SquareInfo> squareInfoList;

    int r, c, nr, nc, depth;
    while (!q.empty()) {
        r = q.front().r;
        c = q.front().c;
        depth = q.front().depth;
        q.pop();

        // 사람이 발견됐다면, 그보다 더 깊이 들어가려 할 때, break;
        if (findDepth == depth) break;

        for (int dir = 0; dir < 8; dir++)
        {
            nr = r + bfsDr[dir];
            nc = c + bfsDc[dir];

            if (!isOutOfBound(nr, nc) && visited[nr][nc] != 1) {
                visited[nr][nc] = 1;

                // 사람이 발견되면
                if (map[nr][nc] == -1) {
                    //찾은 깊이 기록하고
                    findDepth = depth + 1;
                    //후보리스트에 넣는다.
                    //cout << "findDepth: " << findDepth << endl;
                    candidateList.push_back({ nr, nc });
                }
                else q.push({ nr, nc, depth + 1 });
            }
        }
    }

    int sqrR, sqrC, biggerV, diffR, diffC;
    for (int i = 0; i < candidateList.size(); i++)
    {
        r = candidateList[i].r;
        c = candidateList[i].c;

        diffR = abs(exitSpot.r - r);
        diffC = abs(exitSpot.c - c);
        biggerV = max(diffR, diffC);

        //cout << "biggerV: " << biggerV << endl;

        int standR = max(r, exitSpot.r);
        int standC = max(c, exitSpot.c);

        
        sqrR = standR - biggerV;
        if (sqrR < 1) sqrR = 1;

        sqrC = standC - biggerV;
        if (sqrC < 1) sqrC = 1;
        
        squareInfoList.push_back({ sqrR , sqrC, biggerV });

        //if (r <= exitSpot.r && c <= exitSpot.c) {
        //    sqrR = exitSpot.r - biggerV;
        //    sqrC = exitSpot.c - biggerV;
        //    squareInfoList.push_back({ sqrR , sqrC, biggerV });
        //}
        //else if (r <= exitSpot.r && c > exitSpot.c) {
        //    sqrR = exitSpot.r - biggerV;
        //    sqrC = c - biggerV;
        //    squareInfoList.push_back({ sqrR , sqrC, biggerV });
        //}
        //else if (r > exitSpot.r && c > exitSpot.c) {
        //    sqrR = r - biggerV;
        //    sqrC = c - biggerV;
        //    squareInfoList.push_back({ sqrR , sqrC, biggerV });
        //}
        //else if (r > exitSpot.r && c <= exitSpot.c) {
        //    sqrR = r - biggerV;
        //    sqrC = exitSpot.c - biggerV;
        //    squareInfoList.push_back({ sqrR , sqrC, biggerV });
        //}
    }

    sort(squareInfoList.begin(), squareInfoList.end(), compare);

    rotate(squareInfoList[0]);
}

int main() {
    // 여기에 코드를 작성해주세요.
    cin >> N >> M >> K;

    for (int r = 1; r <= N; r++)
    {
        for (int c= 1; c <= N; c++)
        {
            cin >> map[r][c];
        }
    }

    for (int i = 0; i < M; i++)
    {
        int r, c;
        cin >> r >> c;
        map[r][c] = -1;

        runnerList.push_back({ r,c });
    }
    cin >> exitSpot.r >> exitSpot.c;

    for (int i = 0; i < K; i++)
    {
        if (runnerList.size() == 0) break;
        
        move();

        //cout << "====== after move =======" << endl;
        //for (int r = 1; r <= N; r++)
        //{
        //    for (int c = 1; c <= N; c++)
        //    {
        //        cout << map[r][c] << " ";
        //    }
        //    cout << endl;
        //}

        findSqureBFS();

        //cout << "====== findSqureBFS move =======" << endl;
        //for (int r = 1; r <= N; r++)
        //{
        //    for (int c = 1; c <= N; c++)
        //    {
        //        cout << map[r][c] << " ";
        //    }
        //    cout << endl;
        //}
    }


    cout << totalMCnt << '\n';
    cout << exitSpot.r << " " << exitSpot.c << '\n';

    return 0;
}