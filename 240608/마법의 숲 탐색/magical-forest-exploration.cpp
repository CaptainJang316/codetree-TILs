#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Golem {
   int r = 2;
   int c;
   int exitDir;
};

struct Spot {
   int r;
   int c;
};

int R, C, K;
vector<vector<int>> map(74, vector<int>(74, 0));
vector<vector<int>> exitMap(74, vector<int>(74, 0));
vector<Golem> golemList(1001);
int total = 0;
bool isSuccessed = false;

int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

bool isOutOfBoundForGolem(int r, int c) {
   if (r < 1 || R + 3 < r || c < 1 || C < c) return true;
   return false;
}

bool isOutOfBoundForFairy(int r, int c) {
   if (r < 4 || R + 3 < r || c < 1 || C < c) return true;
   return false;
}

// 이렇게 일반화할 수 있으니, 에러잡기도 훠~~~월씬 수월해짐. <-- 이 방법 찾는 게, 나중에 디버깅하는 거 생각하면 시간 훨씬 절약됨.
bool isPossibleMove(int r, int c) {
   bool flag = !isOutOfBoundForGolem(r, c) && map[r][c] == 0;
   
   flag = flag && !isOutOfBoundForGolem(r - 1, c - 1) && map[r - 1][c - 1] == 0;
   flag = flag && !isOutOfBoundForGolem(r - 1, c) && map[r - 1][c] == 0;
   flag = flag && !isOutOfBoundForGolem(r - 1, c + 1) && map[r - 1][c + 1] == 0;
   flag = flag && !isOutOfBoundForGolem(r, c - 1) && map[r][c - 1] == 0;
   flag = flag && !isOutOfBoundForGolem(r, c + 1) && map[r][c + 1] == 0;
   flag = flag && !isOutOfBoundForGolem(r + 1, c) && map[r + 1][c] == 0;

   return flag;
}

int moveFairyBFS(int cnt) {
    vector<vector<int>> visited(74, vector<int>(74, 0));
    queue<Spot> spotQueue;
    spotQueue.push({golemList[cnt].r, golemList[cnt].c});

    // 최초 시작점 visited[initialR][initialC] = 1 처리 꼭 먼저 해주고 시작해야 함!
    visited[golemList[cnt].r][golemList[cnt].c] = 1;
    int maxRow = 0;

    while(!spotQueue.empty()) {
        int currR = spotQueue.front().r;
        int currC = spotQueue.front().c;
        spotQueue.pop();

        if(maxRow < currR) maxRow = currR;

        int previousValue = map[currR][currC];

        int nextR, nextC;
        for (int dir = 0; dir < 4; dir++)
        {
            nextR = currR + dr[dir];
            nextC = currC + dc[dir];
            //최종 r에 -3 할 것.
            if (!isOutOfBoundForFairy(nextR, nextC) && map[nextR][nextC] != 0 && visited[nextR][nextC] != 1)
            {
                if(map[nextR][nextC] == previousValue) {
                    visited[nextR][nextC] = 1;
                    spotQueue.push({nextR, nextC});
                } else if (exitMap[currR][currC] == 1)
                {
                    visited[nextR][nextC] = 1;
                    spotQueue.push({nextR, nextC});
                }
            }
        }
    }
    maxRow -= 3;
    visited.clear();
    return maxRow;
}

bool moveGolem(int cnt) {
   int currR = golemList[cnt].r;
   int currC = golemList[cnt].c;
   int exitDir = golemList[cnt].exitDir;

   while (true) {
      // down
      if (isPossibleMove(currR + 1, currC)) {
         currR += 1;
      }
      // left
      else if (isPossibleMove(currR + 1, currC - 1)) {
         currR += 1;
         currC -= 1;

         exitDir = (exitDir+3) % 4;
      }
      // right
      else if (isPossibleMove(currR + 1, currC + 1)) {
         currR += 1;
         currC += 1;

         exitDir = (exitDir+1) % 4;
      }
      else break;
   }

    //맵에 제대로 들어가지 못했다면
   if(currR-1 < 4) {

    isSuccessed = false;
    fill(map.begin(), map.end(), vector<int>(74, 0));
    fill(exitMap.begin(), exitMap.end(), vector<int>(74, 0));
   } else {

    isSuccessed = true;
    //최종 위치 map에 표시
    map[currR][currC] = cnt;
    map[currR][currC-1] = cnt;
    map[currR][currC+1] = cnt;
    map[currR-1][currC] = cnt;
    map[currR+1][currC] = cnt;

    int exitR = currR + dr[exitDir];
    int exitC = currC + dc[exitDir];
    exitMap[exitR][exitC] = 1;

    //최종 위치를 그 다음 함수에서도 인식할 수 있게끔 처리를 해줘야 한다. 안그러면 그 다음 함수에선 그 좌표를 모른다.
    golemList[cnt].r = currR;
    golemList[cnt].c = currC;
   }

   return isSuccessed;
}

int main() {
   cin >> R >> C >> K;
   
   for (int i = 1; i <= K; i++) {
      cin >> golemList[i].c >> golemList[i].exitDir;

      //이게 실패해서 리셋되면 다음 과정 실행하지 않도록 한다. <-- 로직간의 상관관계 잊지 않도록 처음부터 명시적으로 정리할 것.
      if(moveGolem(i)) total += moveFairyBFS(i);
   }

   cout << total << endl;

   return 0;
}