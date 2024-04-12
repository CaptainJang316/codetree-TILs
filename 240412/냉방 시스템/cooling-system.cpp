// 2:10 ~ 3:00, 3:10 ~  6:07, 6:13 ~
// 온풍기를 설치하려고 한다. 
// 온풍기의 성능을 테스트하기 위해 구사과는 집을 크기가 R×C인 격자판으로 나타냈고, 1×1 크기의 칸으로 나눴다. 
// 구사과는 각 칸 (r, c)의 온도를 실시간으로 모니터링하는 시스템을 개발했다. 
// (r, c)는 r행 c열을 의미한다.

// 성능 테스트는 다음과 같은 작업이 순차적으로 이루어지며, 가장 '처음에 모든 칸의 온도는 0'이다.
// 문제의 그림에서 빈 칸은 온도가 0인 칸을 의미한다.
// 
// (다음의 순서로 진행)
// 1. 집에 있는 모든 온풍기에서 바람이 한 번 나옴
// 
// 2. 온도가 조절됨
// => 모든 인접한 칸에 대해서, 온도가 높은 칸에서 낮은 칸으로 ⌊(두 칸의 온도의 차이)/4⌋만큼 온도가 조절된다. 
// 온도가 높은 칸은 이 값만큼 온도가 감소하고, 낮은 칸은 온도가 상승한다. 
// 인접한 두 칸 사이에 벽이 있는 경우에는 온도가 조절되지 않는다. 이 과정은 모든 칸에 대해서 '동시에' 발생한다.
// 옆칸이 0이어도 계산함
// 
// 3. 온도가 1 이상인 가장 바깥쪽 칸의 온도가 1씩 감소
// => 온도가 0인 칸은 온도가 감소하지 않는다.
// 
// 4. 초콜릿을 하나 먹는다.
// 
// 5. 조사하는 모든 칸의 온도가 K 이상이 되었는지 검사. 
// => 모든 칸의 온도가 K이상이면 테스트를 중단하고, 아니면 1번으로 돌아감.
// 
// 온풍기의 바람이 나오는 방향은 오른쪽, 왼쪽, 위, 아래 중 하나이다.

//어떤 칸에 같은 온풍기에서 나온 바람이 여러 번 도착한다고 해도 온도는 여러번 상승하지 않는다.
//=> 즉,다른 온풍기에서 왔다면 여러번 상승 가능

// 모든 인접한 칸에 대해서, 온도가 높은 칸에서 낮은 칸으로 ⌊(두 칸의 온도의 차이)/4⌋만큼 온도가 조절된다. 
// 온도가 높은 칸은 이 값만큼 온도가 감소하고, 낮은 칸은 온도가 상승한다. 
// 인접한 두 칸 사이에 벽이 있는 경우에는 온도가 조절되지 않는다. 이 과정은 모든 칸에 대해서 '동시에' 발생한다.
// 옆칸이 0이어도 계산함ㅇㅇ

// 방의 크기와 방에 설치된 온풍기의 정보, 벽의 위치와 조사하려고 하는 칸의 위치가 주어진다. 
// 먹은 초콜릿의 개수를 출력한다.


// 첫째 줄에 세 정수 R, C, K가 주어진다. 둘째 줄부터 R개의 줄에 방의 정보가 주어진다. 
// i번째 줄의 j번째 정수는 (i, j)의 정보를 의미하며 다음 중 하나이다.
// 0: 빈 칸
// 1 : 방향이 오른쪽인 온풍기가 있음
// 2 : 방향이 왼쪽인 온풍기가 있음
// 3 : 방향이 위인 온풍기가 있음]
// 4 : 방향이 아래인 온풍기가 있음
// 5 : 온도를 조사해야 하는 칸
// 
// 다음 줄에는 벽의 개수 W가 주어진다.다음 W개의 줄에는 벽의 정보가 주어지며, 이 정보는 세 정수 x, y, t로 이루어져 있다.
// t가 0인 경우(x, y)와(x - 1, y) 사이에 벽이 있는 것이고, 1인 경우에는(x, y)와(x, y + 1) 사이에 벽이 있는 것이다.

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int R, C, K, W;
queue<vector<int>> orderListQ;
vector<vector<int> > machineSpotList;
vector<vector<int> > checkSpotList;
vector<vector<int> > wallList;
vector< vector<vector<int>>> wallInfo(21, vector<vector<int>>(21, vector<int>(5, 0)));

bool isOutOfBound(int r, int c) {
    if (r < 1 || R < r || c < 1 || C < c) return true;
    return false;
}

void machineWork(vector<vector<int> >& map) {
    // 방향 1~4으로 저장(오왼위아)
    int count = 0;
    int startR, startC, dir;

    for (int i = 0; i < machineSpotList.size(); i++) {
        vector<int> currentMachine = machineSpotList[i];
        vector<vector<int>> tempMap(R+1, vector<int>(C+1, 0));
        queue<vector<int>> spotQ;

        startR = currentMachine[0];
        startC = currentMachine[1];
        dir = currentMachine[2];

        //오른쪽
        if (dir == 1) {
            // 방향 1~4으로 저장(오왼위아)
            if (!isOutOfBound(startR, startC + 1) && wallInfo[startR][startC+1][2] != 1) {
                spotQ.push({ startR , startC+1 , 4});
                map[startR][startC + 1] += 5;
                tempMap[startR][startC + 1] = 5;
            }

            while (!spotQ.empty()) {
                vector<int> current = spotQ.front();
                spotQ.pop();

                int r = current[0];
                int c = current[1];
                int n = current[2];

                if (1 <= n) {
                    if (!isOutOfBound(r - 1, c)&& wallInfo[r - 1][c][4] != 1 && wallInfo[r - 1][c][1] != 1) {
                        if (!isOutOfBound(r - 1, c + 1) && tempMap[r - 1][c + 1] == 0) {
                            map[r - 1][c + 1] += n;
                            tempMap[r - 1][c + 1] = n;
                            spotQ.push({ r - 1, c + 1, n - 1 });
                        }
                    }
                    if (!isOutOfBound(r + 1, c)&& wallInfo[r + 1][c][3] != 1 && wallInfo[r + 1][c][1] != 1) {
                        if (!isOutOfBound(r + 1, c + 1) && tempMap[r + 1][c + 1] == 0) {
                            map[r + 1][c + 1] += n;
                            tempMap[r + 1][c + 1] = n;
                            spotQ.push({ r + 1, c + 1, n - 1 });
                        }
                    }
                    if (wallInfo[r][c][1] != 1) {
                        if (!isOutOfBound(r, c + 1) && tempMap[r][c + 1] == 0) {
                            map[r][c + 1] += n;
                            tempMap[r][c + 1] = n;
                            spotQ.push({ r, c + 1, n - 1 });
                        }
                    }
                }
            }
        }
        //왼쪽
        else if (dir == 2) {

            if (!isOutOfBound(startR, startC - 1) && wallInfo[startR][startC-1][2] != 1) {
                spotQ.push({ startR , startC-1, 4});
                map[startR][startC - 1] += 5;
                tempMap[startR][startC - 1] = 5;
            }

            while (!spotQ.empty()) {
                vector<int> current = spotQ.front();
                spotQ.pop();

                int r = current[0];
                int c = current[1];
                int n = current[2];

                //이 때만 동작 수행
                if (1 <= n) {
                    if (!isOutOfBound(r - 1, c) && wallInfo[r - 1][c][4] != 1 && wallInfo[r - 1][c][2] != 1) {
                        if (!isOutOfBound(r - 1, c - 1) && tempMap[r - 1][c - 1] == 0) {
                            map[r - 1][c - 1] += n;
                            tempMap[r - 1][c - 1] = n;
                            spotQ.push({ r - 1, c - 1, n - 1 });
                        }
                    }
                    if (!isOutOfBound(r + 1, c) && wallInfo[r + 1][c][3] != 1 && wallInfo[r + 1][c][2] != 1) {
                        if (!isOutOfBound(r + 1, c - 1) && tempMap[r + 1][c - 1] == 0) {
                            map[r + 1][c - 1] += n;
                            tempMap[r + 1][c - 1] = n;
                            spotQ.push({ r + 1, c - 1, n - 1 });
                        }
                    }
                    if (wallInfo[r][c][2] != 1) {
                        if (!isOutOfBound(r, c - 1) && tempMap[r][c - 1] == 0) {
                            map[r][c - 1] += n;
                            tempMap[r][c - 1] = n;
                            spotQ.push({ r, c - 1, n - 1 });
                        }
                    }
                }
            }
        }
        //위
        else if (dir == 3) {
            // 방향 1~4으로 저장(오왼위아)
            if (!isOutOfBound(startR - 1, startC) && wallInfo[startR - 1][startC][2] != 1) {
                spotQ.push({ startR - 1 , startC, 4 });
                map[startR - 1][startC] += 5;
                tempMap[startR - 1][startC] = 5;
            }

            while (!spotQ.empty()) {
                vector<int> current = spotQ.front();
                spotQ.pop();

                int r = current[0];
                int c = current[1];
                int n = current[2];

                //이 때만 동작 수행
                if (1 <= n) {
                    if (!isOutOfBound(r, c - 1)&& wallInfo[r][c - 1][3] != 1 && wallInfo[r][c - 1][1] != 1) {
                        if (!isOutOfBound(r - 1, c - 1) && tempMap[r - 1][c - 1] == 0) {
                            map[r - 1][c - 1] += n;
                            tempMap[r - 1][c - 1] = n;
                            spotQ.push({ r - 1, c - 1, n - 1 });
                        }
                    }
                    if (!isOutOfBound(r, c + 1)&& wallInfo[r][c + 1][3] != 1 && wallInfo[r][c + 1][2] != 1) {
                        if (!isOutOfBound(r - 1, c + 1) && tempMap[r - 1][c + 1] == 0) {
                            map[r - 1][c + 1] += n;
                            tempMap[r - 1][c + 1] = n;
                            spotQ.push({ r - 1, c + 1, n - 1 });
                        }
                    }
                    if (wallInfo[r][c][3] != 1) {
                        if (!isOutOfBound(r - 1, c) && tempMap[r - 1][c] == 0) {
                            map[r - 1][c] += n;
                            tempMap[r - 1][c] = n;
                            spotQ.push({ r - 1, c, n - 1 });
                        }
                    }
                }
            }
        }
        // 아래
         else if (dir == 4) {
            // 방향 1~4으로 저장(오왼위아)
            if (!isOutOfBound(startR + 1, startC) && wallInfo[startR + 1][startC][2] != 1) {
                spotQ.push({ startR + 1 , startC, 4 });
                map[startR + 1][startC] += 5;
                tempMap[startR + 1][startC] = 5;
            }

            while (!spotQ.empty()) {
                vector<int> current = spotQ.front();
                spotQ.pop();

                int r = current[0];
                int c = current[1];
                int n = current[2];

                //이 때만 동작 수행
                if (1 <= n) {
                    if (!isOutOfBound(r, c - 1) && wallInfo[r][c - 1][4] != 1 && wallInfo[r][c - 1][1] != 1) {
                        if (!isOutOfBound(r + 1, c - 1) && tempMap[r + 1][c - 1] == 0) {
                            map[r + 1][c - 1] += n;
                            tempMap[r + 1][c - 1] = n;
                            spotQ.push({ r + 1, c - 1, n - 1 });
                        }
                    }
                    if (!isOutOfBound(r, c + 1)&& wallInfo[r][c + 1][4] != 1 && wallInfo[r][c + 1][2] != 1) {
                        if (!isOutOfBound(r + 1, c + 1) && tempMap[r + 1][c + 1] == 0) {
                            map[r + 1][c + 1] += n;
                            tempMap[r + 1][c + 1] = n;
                            spotQ.push({ r + 1, c + 1, n - 1 });
                        }
                    }
                    if (wallInfo[r][c][4] != 1) {
                        if (!isOutOfBound(r + 1, c) && tempMap[r + 1][c] == 0) {
                            map[r + 1][c] += n;
                            tempMap[r + 1][c] = n;
                            spotQ.push({ r + 1, c, n - 1 });
                        }
                    }
                }
            }
        }

        currentMachine.clear();
        tempMap.clear();
    }
}

void spreadHeatBFS(vector<vector<int> >& map) {
    int diff = 0;
    int wallR, wallC, dir;
    for (int r = 1; r <= R; r++) {
        for (int c = 1; c <= C; c++) {

            if (map[r][c] == 0) continue;

            if (!isOutOfBound(r + 1, c)) {
                if (wallInfo[r][c][4] != 1) {
                    diff = (map[r][c] - map[r + 1][c]) / 4;
                    if (0 < diff) orderListQ.push({ r, c, r + 1, c, diff });
                }
            }

            if (!isOutOfBound(r - 1, c)) {
                if (wallInfo[r][c][3] != 1) {
                    diff = (map[r][c] - map[r - 1][c]) / 4;
                    if (0 < diff) orderListQ.push({ r, c, r - 1, c, diff });
                }
            }

            if (!isOutOfBound(r, c + 1)) {

                if (wallInfo[r][c][1] != 1) {
                    diff = (map[r][c] - map[r][c + 1]) / 4;
                    if (0 < diff) orderListQ.push({ r, c, r, c + 1, diff });
                }
            }

            if (!isOutOfBound(r, c - 1)) {

                if (wallInfo[r][c][2] != 1) {
                    diff = (map[r][c] - map[r][c - 1]) / 4;
                    if (0 < diff) orderListQ.push({ r, c, r, c - 1, diff });
                }
            }
        }
    }

    vector<int> currentOrder;
    int giveR, giveC, getR, getC, value;
    while (!orderListQ.empty()) {
        currentOrder = orderListQ.front();
        orderListQ.pop();

        giveR = currentOrder[0];
        giveC = currentOrder[1];
        getR = currentOrder[2];
        getC = currentOrder[3];
        value = currentOrder[4];

        map[getR][getC] += value;
        map[giveR][giveC] -= value;
    }
}

int main() {
    cin >> R >> C >> K; // K는 기준 온도
    vector<vector<int> > map(R+1, vector<int>(C+1, 0));
    int chocolateCount = 0;

    for (int r = 1; r <= R; r++) {
        for (int c = 1; c <= C; c++) {
            cin >> map[r][c];

            if (map[r][c] != 0) {

                if (map[r][c] == 5) {
                    checkSpotList.push_back({ r, c });
                    map[r][c] = 0;
                }
                else {
                    machineSpotList.push_back({ r, c, map[r][c] }); // 방향 1~4으로 저장(오왼위아)
                    map[r][c] = 0;
                }
            }
        }
    }

    cin >> W;
    int wallR, wallC, wallDir;
    for (int i = 0; i < W; i++) {
        cin >> wallR >> wallC >> wallDir;
 
        //방향 1~4으로 저장(오 왼 위 아)
        if (wallDir == 0) {
            wallInfo[wallR][wallC][3] = 1;
            wallInfo[wallR - 1][wallC][4] = 1;
        }
        else if (wallDir == 1) {
            wallInfo[wallR][wallC][1] = 1;
            wallInfo[wallR][wallC + 1][2] = 1;
        }
    }

    bool isComplete = false;
    while (!isComplete) {
        isComplete = true;

        // 1. 집에 있는 모든 온풍기에서 바람이 한 번 나옴
        machineWork(map);

        spreadHeatBFS(map);

        // 3. 온도가 1 이상인 가장 바깥쪽 칸의 온도가 1씩 감소
        // => 온도가 0인 칸은 온도가 감소하지 않는다.
        for (int c = 1; c <= C; c++) {
            if (map[1][c] != 0) map[1][c] -= 1;
            if (map[R][c] != 0) map[R][c] -= 1;
        }
        for (int r = 2; r <= R-1; r++) {
            if (map[r][1] != 0) map[r][1] -= 1;
            if (map[r][C] != 0) map[r][C] -= 1;
        }

        // 4. 초콜릿을 하나 먹는다.
        chocolateCount += 1;

        // 5. 조사하는 모든 칸의 온도가 K 이상이 되었는지 검사. 
        // => 모든 칸의 온도가 K이상이면 테스트를 중단하고, 아니면 1번으로 돌아감.
        int checkR, checkC;
        for (int i = 0; i < checkSpotList.size(); i++) {
            checkR = checkSpotList[i][0];
            checkC = checkSpotList[i][1];

            if (map[checkR][checkC] < K) isComplete = false;
        }

        if (100 < chocolateCount) {
            chocolateCount = 101;
            break;
        }
    }
    
    // 초콜릿의 개수가 100을 넘어가면 101을 출력한다.
    cout << chocolateCount << "\n";
}