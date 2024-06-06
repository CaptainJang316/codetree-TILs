#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Spot {
    int r;
    int c;
};

// 충돌시 state = 2 & 매 턴마다 -= 1. 탈락 시 -1.
struct SantaInfo {
    int num;
    int r;
    int c;
    int state = 0;
    int score = 0;
};

int eightDr[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int eightDc[] = { 0, 1, 1, 1, 0, -1, -1, -1 };

int fourDr[] = { -1, 0, 1, 0 };
int fourDc[] = { 0, 1, 0, -1 };

int N, M, P, C, D;
int outSantaNum = 0;
Spot rudolfSpot;
vector<SantaInfo> santaInfoList(31);
vector<vector<int>> map(51, vector<int>(51, 0));

bool isOutOfBound(int r, int c) {
    if (r < 1 || N < r || c < 1 || N < c) return true;
    return false;
}

int getDistance(int r1, int c1, int r2, int c2) {
    return (r1 - r2) * (r1 - r2) + (c1 - c2) * (c1 - c2);
}

void crash(int dir, int crashedR, int crashedC, int dist, int crashedSantaNum, string order) {
    int nextR, nextC;

    if (order == "rudolf") {
        map[crashedR][crashedC] = -1;
        santaInfoList[crashedSantaNum].state = 2;
        santaInfoList[crashedSantaNum].score += dist;

        nextR = crashedR + (eightDr[dir] * dist);
        nextC = crashedC + (eightDc[dir] * dist);
    }
    else if(order == "rudolf_interation") {
        nextR = crashedR + (eightDr[dir] * dist);
        nextC = crashedC + (eightDc[dir] * dist);
    }
    else if(order == "santa") {
        nextR = crashedR + (fourDr[dir] * dist);
        nextC = crashedC + (fourDc[dir] * dist);
        santaInfoList[crashedSantaNum].score += dist;
    }
    else if(order == "santa_interation") {
        nextR = crashedR + (fourDr[dir] * dist);
        nextC = crashedC + (fourDc[dir] * dist);
    }

    
   santaInfoList[crashedSantaNum].r = nextR;
   santaInfoList[crashedSantaNum].c = nextC;
    
    //해당 산타 처리
    // 위치, map 기입, 상호작용

    // 범위 아웃
    if (isOutOfBound(nextR, nextC)) {
        santaInfoList[crashedSantaNum].state = -1;
        outSantaNum += 1;
    }
    // 다른 산타자리
    else if(0 < map[nextR][nextC]) {
      if(order == "rudolf" || order == "rudolf_interation") {
         crash(dir, nextR, nextC, 1, map[nextR][nextC], "rudolf_interation");
      }
      else {
         crash(dir, nextR, nextC, 1, map[nextR][nextC], "santa_interation");
      }
      map[nextR][nextC] = crashedSantaNum;
    }
    // 루돌프 자리가 아니면
    else if(map[nextR][nextC] != -1){
        map[nextR][nextC] = crashedSantaNum;
    }
}

void moveRudolf() {
    int minDist = 10000;
    int minDistSantaNum, minSantaR, minSantaC;
    int currDist;
    for (int i = 1; i <= P ; i++)
    {
        if (santaInfoList[i].state != -1) {
            currDist = getDistance(santaInfoList[i].r, santaInfoList[i].c, rudolfSpot.r, rudolfSpot.c);
            if (currDist < minDist) {
                minDist = currDist;
                minSantaR = santaInfoList[i].r;
                minSantaC = santaInfoList[i].c;
                minDistSantaNum = i;
             }
            else if (currDist == minDist) {
                if (minSantaR < santaInfoList[i].r) {
                    minSantaR = santaInfoList[i].r;
                    minDistSantaNum = i;
                }
                else if (minSantaR == santaInfoList[i].r && minSantaC < santaInfoList[i].c) {
                    minSantaC = santaInfoList[i].c;
                    minDistSantaNum = i;
                }
            }
        }
    }

    int nextR, nextC;
    minDist = 10000;
    int currR = rudolfSpot.r;
    int currC = rudolfSpot.c;

    map[currR][currC] = 0;

    for (int dir = 0; dir < 8; dir++)
    {
        nextR = currR + eightDr[dir];
        nextC = currC + eightDc[dir];

        if (!isOutOfBound(nextR, nextC)) {
            currDist = getDistance(santaInfoList[minDistSantaNum].r, santaInfoList[minDistSantaNum].c, nextR, nextC);

            if (santaInfoList[minDistSantaNum].r == nextR && santaInfoList[minDistSantaNum].c == nextC) {
                rudolfSpot.r = nextR;
                rudolfSpot.c = nextC;
                crash(dir, nextR, nextC, C, map[nextR][nextC], "rudolf");
            }
            else if (currDist < minDist) {
                minDist = currDist;
                rudolfSpot.r = nextR;
                rudolfSpot.c = nextC;
            }
        }
    }
    map[rudolfSpot.r][rudolfSpot.c] = -1;
} 

void moveSantas() {
    SantaInfo currSanta;
    int nextR, nextC, currDist, minCastDir;

    for (int i = 1; i <= P; i++)
    {
        currSanta = santaInfoList[i];
        // 현재 위치와의 거리를 기준으로 삼는다.(최초 minDist로 설정한다)
        int minDist = getDistance(rudolfSpot.r, rudolfSpot.c, currSanta.r, currSanta.c);
        bool closertSpotExist = false;

        if (currSanta.state == 0) {
            
            for (int dir = 0; dir < 4; dir++)
            {
                nextR = currSanta.r + fourDr[dir];
                nextC = currSanta.c + fourDc[dir];
                
                // 범위 밖이 아니고, 다른 산타도 없으면 
                if (!isOutOfBound(nextR, nextC) && (map[nextR][nextC] == 0 || map[nextR][nextC] == -1)) {
                    
                    currDist = getDistance(rudolfSpot.r, rudolfSpot.c, nextR, nextC);
                    if (currDist < minDist) {
                        minDist = currDist;
                        minCastDir = dir;
                        closertSpotExist = true;
                        santaInfoList[i].r = nextR;
                        santaInfoList[i].c = nextC;
                    }
                }
            }
            // 이동 가능한 더 가까운 거리의 좌표가 존재함.
            if (closertSpotExist) {
                nextR = santaInfoList[i].r;
                nextC = santaInfoList[i].c;
                
                map[currSanta.r][currSanta.c] = 0;

                if (map[nextR][nextC] == -1) {
                    santaInfoList[i].state = 2;
                    crash((minCastDir + 2) % 4, nextR, nextC, D, santaInfoList[i].num, "santa");
                }
                else {
                    map[nextR][nextC] = santaInfoList[i].num;
                }
            }
        }
    }
}

bool compareSanta(SantaInfo &Santa1, SantaInfo &Santa2) {
   return Santa1.num < Santa2.num;
}

int main() {
    cin >> N >> M >> P >> C >> D;
    cin >> rudolfSpot.r >> rudolfSpot.c;
    map[rudolfSpot.r][rudolfSpot.c] = -1;

    for (int i = 1; i <= P; i++)
    {
        cin >> santaInfoList[i].num >> santaInfoList[i].r >> santaInfoList[i].c;
    }

    sort(santaInfoList.begin()+1, santaInfoList.begin()+P+1, compareSanta);
    for (int i = 1; i <= P; i++)
    {
        map[santaInfoList[i].r][santaInfoList[i].c] = santaInfoList[i].num;
    }

    for (int i = 0; i < M; i++)
    {
        moveRudolf();

        // 매 턴마다 산타의 state-- 하고, state가 0인 것들만 움직이도록 함.
        moveSantas();

        if (P <= outSantaNum) break;

        for (int i = 1; i <= P; i++)
        {
            if (0 < santaInfoList[i].state) santaInfoList[i].state -= 1;
            if (santaInfoList[i].state != -1) santaInfoList[i].score += 1;;
        }
    }

    for (int i = 1; i <= P; i++)
    {
        cout << santaInfoList[i].score << " ";
    }
    cout << endl;

    return 0;
}