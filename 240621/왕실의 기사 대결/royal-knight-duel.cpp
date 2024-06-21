// 9:50 ~ ....., 11:40 ~ 12:10

//왕실의 기사들은 L×L 크기의 체스판 위에서 대결을 준비하고 있습니다.
//체스판의 왼쪽 상단은(1, 1)로 시작하며, 각 칸은 빈칸, 함정, 또는 벽으로 구성되어 있습니다.
//체스판 밖도 벽으로 간주합니다.
//
//기사들은 자신의 마력으로 상대방을 밀쳐낼 수 있습니다.
//각 기사의 초기위치는(r, c)로 주어지며, 
//그들은 방패를 들고 있기 때문에(r, c)를 좌측 상단으로 하며 h(높이)×w(너비) 크기의 직사각형 형태를 띄고 있습니다.각 기사의 체력은 k로 주어집니다.

//(1) 기사 이동
//
// 왕에게 명령을 받은 기사는 상하좌우 중 하나로 한 칸 이동할 수 있습니다.
// 이때 만약 이동하려는 위치에 다른 기사가 있다면 그 기사도 함께 연쇄적으로 한 칸 밀려나게 됩니다.
// 그 옆에 또 기사가 있다면 연쇄적으로 한 칸씩 밀리게 됩니다.
// 하지만 만약 기사가 이동하려는 방향의 끝에 벽이 있다면 모든 기사는 이동할 수 없게 됩니다.
// 또, 체스판에서 사라진 기사에게 명령을 내리면 아무런 반응이 없게 됩니다. <-- 체스판에서 사라지면 해당 기사는 out!
//
//(2) 대결 대미지
//
// 명령을 받은 기사가 다른 기사를 밀치게 되면, 밀려난 기사들은 피해를 입게 됩니다.
// 이때 각 기사들은 해당 기사가 이동한 곳에서 w×h 직사각형 내에 놓여 있는 함정의 수만큼만 피해를 입게 됩니다.
// 각 기사마다 피해를 받은 만큼 체력이 깎이게 되며, 현재 체력 이상의 대미지를 받을 경우 기사는 '체스판에서 사라지게' 됩니다.
// 
// 단, 명령을 받은 기사는 피해를 입지 않으며, 기사들은 모두 밀린 이후에 대미지를 입게 됩니다.
// 밀렸더라도 밀쳐진 위치에 함정이 전혀 없다면 그 기사는 피해를 전혀 입지 않게 됨에 유의합니다.
//
// Q 번에 걸쳐 왕의 명령이 주어졌을 때, Q 번의 대결이 모두 끝난 후 '생존한 기사들이 총 받은 대미지의 합'을 출력하는 프로그램을 작성해보세요.


// Q번에 걸쳐 아래 명령 반복
// 
//1. 명령을 받은 기사를 찾아서, 상하좌우 한 칸 이동.(체스판에서 사라진 기사라면 명령에 반응 X)
//2-1. 명령을 받은 기사들의 연쇄 반응까지 고려해서 가장 끝에 벽이 있다면 움직이지 않는다.
//2-2. 밀려서 이동을 하게 된다면, 밀린 기사의 새 위치에 있는 함정의 수만큼 피해를 받되, 연쇄 이동이 다 끝난 이후에 데미지를 줄 것.(명령 받은 기사는 제외)
// 
// 단, 처음 주어지는 기사들의 위치는 기사들끼리 서로 겹쳐져 있지 않습니다. 또한 기사와 벽은 겹쳐서 주어지지 않습니다.
// 입력은 1번 기사부터 N번 기사까지 순서대로 정보가 주어집니다.

#include<iostream>
#include<vector>
#include<queue>
#include<stack>

using namespace std;

int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

struct Order {
    int i;
    int dir;
};

struct Knight
{
    int r;
    int c;
    int h;
    int w;
    int k;
};

int L, N, Q;
vector<vector<int>> map(41, vector<int>(41, 0));
vector<vector<int>> knightMap(41, vector<int>(41, 0));
vector<Order> orderList(100);
vector<Knight> knightList(31);
vector<int> originalKnightHP(31, 0);
stack<Knight> knightQueue;

bool isOutOfBOund(int r, int c) {
    return r < 1 || L < r || c < 1 || L < c;
}

bool isMovePossible(Knight startKnight, int dir) {
    queue<Knight> tempKnightQueue;

    tempKnightQueue.push(startKnight);
    while (!tempKnightQueue.empty()) {
        Knight currKnight = tempKnightQueue.front();
        tempKnightQueue.pop();

        bool isNextEmpty = true;

        int nextR, nextC;
        if (dir == 0)
        {
            nextR = currKnight.r - 1;
            for (int c = currKnight.c; c < currKnight.c + currKnight.w; c++)
            {
                if (isOutOfBOund(nextR, c) || map[nextR][c] == 2) {
                    return false;
                }

                // 다른 기사 발견
                if (0 < knightMap[nextR][c]) {
                    knightQueue.push({ currKnight });
                    tempKnightQueue.push(knightList[knightMap[nextR][c]]);
                    isNextEmpty = false;
                }
            }
        }
        else if (dir == 1)
        {
            nextC = currKnight.c + currKnight.w;
            for (int r = currKnight.r; r < currKnight.r + currKnight.h; r++)
            {
                if (isOutOfBOund(r, nextC) || map[r][nextC] == 2) {
                    return false;
                }

                // 다른 기사 발견
                if (0 < knightMap[r][nextC]) {
                    // cout << "??: " << knightMap[currKnight.r][currKnight.c] << endl;
                    // cout << "!!: " << knightMap[r][nextC] << endl;
                    knightQueue.push({ currKnight });
                    tempKnightQueue.push(knightList[knightMap[r][nextC]]);
                    isNextEmpty = false;
                }
            }
        }
        else if (dir == 2)
        {
            nextR = currKnight.r + currKnight.h;
            for (int c = currKnight.c; c < currKnight.c + currKnight.w; c++)
            {
                if (isOutOfBOund(nextR, c) || map[nextR][c] == 2) {
                    return false;
                }

                // 다른 기사 발견
                if (0 < knightMap[nextR][c]) {
                    knightQueue.push({ currKnight });
                    tempKnightQueue.push(knightList[knightMap[nextR][c]]);
                    isNextEmpty = false;
                }
            }
        }
        else if (dir == 3)
        {
            nextC = currKnight.c - 1;
            for (int r = currKnight.r; r < currKnight.r + currKnight.h; r++)
            {
                if (isOutOfBOund(r, nextC) || map[r][nextC] == 2) {
                    return false;
                }

                // 다른 기사 발견
                if (0 < knightMap[r][nextC]) {
                    knightQueue.push({ currKnight });
                    tempKnightQueue.push(knightList[knightMap[r][nextC]]);
                    isNextEmpty = false;
                }
            }
        }

        if (isNextEmpty) {
            //cout << "마지막 기사: " << knightMap[currKnight.r][currKnight.c] << endl;
            knightQueue.push({ currKnight });

        }
    }

    return true;

    //벽이면 return false

    //다른 기사 있으면 재귀 & queue에 넣기

    //비어있으면 queue에 넣기 & return true 

}

void removeKnight(int idx) {

    for (int r = knightList[idx].r; r < knightList[idx].r + knightList[idx].h; r++)
    {
        for (int c = knightList[idx].c; c < knightList[idx].c + knightList[idx].w; c++)
        {
            knightMap[r][c] = 0;
        }
    }
}

void move(int dir) {

    //cout << "dir: " << dir << endl;
    //for (int r = 1; r <= L; r++)
    //{
    //    for (int c = 1; c <= L; c++)
    //    {
    //        cout << knightMap[r][c] << " ";
    //    }
    //    cout << endl;
    //}

    int deleteR, deleteC, idx;
    Knight currKnight;
    while (!knightQueue.empty()) {
        currKnight = knightQueue.top();
        knightQueue.pop();

        // 새로운 공간들에 표시 & 함정 데미지 받기
        idx = knightMap[currKnight.r][currKnight.c];
        //cout << "idx: " << idx << endl;
        //cout << "knightList[idx].r: " << knightList[idx].r << ", knightList[idx].c: " << knightList[idx].c << endl;
        knightList[idx].r = currKnight.r + dr[dir];
        knightList[idx].c = currKnight.c + dc[dir];
        //cout << "knightList[idx].r: " << knightList[idx].r << ", knightList[idx].c: " << knightList[idx].c << endl;
        //cout << endl;
        // 원래 자리의 끝 1줄은 지워주기tty 
        if (dir == 0)
        {
            deleteR = currKnight.r + currKnight.h - 1;
            for (int c = currKnight.c; c < currKnight.c + currKnight.w; c++)
            {
                knightMap[deleteR][c] = 0;
            }
        }
        else if (dir == 1)
        {
            deleteC = currKnight.c;
            for (int r = currKnight.r; r < currKnight.r + currKnight.h; r++)
            {
                knightMap[r][deleteC] = 0;
            }
        }
        if (dir == 2)
        {
            deleteR = currKnight.r;
            for (int c = currKnight.c; c < currKnight.c + currKnight.w; c++)
            {
                knightMap[deleteR][c] = 0;
            }
        }
        else if (dir == 3)
        {
            deleteC = currKnight.c + currKnight.w - 1;
            for (int r = currKnight.r; r < currKnight.r + currKnight.h; r++)
            {
                knightMap[r][deleteC] = 0;
            }
        }

        for (int r = knightList[idx].r; r < knightList[idx].r + currKnight.h; r++)
        {
            for (int c = knightList[idx].c; c < knightList[idx].c + currKnight.w; c++)
            {
                knightMap[r][c] = idx;

                if (map[r][c] == 1 && !knightQueue.empty()) {
                    knightList[idx].k -= 1;
                }
            }
        }
        if (knightList[idx].k <= 0) removeKnight(idx);
    }

    //cout << endl;
    //for (int r = 1; r <= L; r++)
    //{
    //    for (int c = 1; c <= L; c++)
    //    {
    //        cout << knightMap[r][c] << " ";
    //    }
    //    cout << endl;
    //}
}

int main() {
    cin >> L >> N >> Q;

    for (int r = 1; r <= L; r++)
    {
        for (int c = 1; c <= L; c++)
        {
            cin >> map[r][c];
        }
    }

    for (int i = 1; i <= N; i++)
    {
        cin >> knightList[i].r >> knightList[i].c >> knightList[i].h >> knightList[i].w >> knightList[i].k;
        originalKnightHP[i] = knightList[i].k;
    }

    for (int i = 1; i <= N; i++)
    {
        for (int r = knightList[i].r; r < knightList[i].r + knightList[i].h; r++)
        {
            for (int c = knightList[i].c; c < knightList[i].c + knightList[i].w; c++)
            {
                knightMap[r][c] = i;
            }
        }
    }
    //cout << endl;
    //for (int r = 1; r <= L; r++)
    //{
    //    for (int c = 1; c <= L; c++)
    //    {
    //        cout << knightMap[r][c] << " ";
    //    }
    //    cout << endl;
    //}

    for (int i = 0; i < Q; i++)
    {
        cin >> orderList[i].i >> orderList[i].dir;
    }

    for (int n = 0; n < Q; n++)
    {
        int currIdx = orderList[n].i;
        int currDir = orderList[n].dir;
        if (knightList[currIdx].k <= 0) continue;

        if (isMovePossible(knightList[currIdx], currDir)) {
            //cout << "n: " << n << endl;
            // 1. 큐에 모아놓은 기사 이동 작업
            // 2. 함정 적용
            move(currDir);
        }
        else {
            while (!knightQueue.empty()) {
                knightQueue.pop();
            }
        }
    }

    int totalDamage = 0;
    for (int i = 1; i <= N; i++)
    {
        if (0 < knightList[i].k) totalDamage += (originalKnightHP[i] - knightList[i].k);
    }
    cout << totalDamage << endl;
}