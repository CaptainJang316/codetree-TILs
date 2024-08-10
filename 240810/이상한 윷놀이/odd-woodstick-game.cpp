// 8:10 ~ 9:00, 12:52 ~ 4:00 <- 거의 4시간 꽉 채움, 혹은 넘음;;;

// n * n 격자판

// 격자판은 흰색, 빨간색, 파란색 중 하나의 색을 가지고 있습니다.
// 말은 총 k개가 주어지며, 모두 격자판의 한 지점에 놓여있습니다. 
// '1번부터 k번까지' 번호가 지정되어 있으며 이동 방향 또한 미리 정해져있습니다. 
// 상하좌우의 4가지 방향으로 움직일 수 있습니다.

// 턴 한 번동안 1번 말부터 k번 말까지 규칙대로 '순서대로' 움직입니다. 
// 말을 옮기는 규칙은 다음과 같습니다.

// 1. 말이 이동하려는 칸이 흰색인 경우에는 해당 칸으로 이동합니다. 
// 이동하려는 칸에 말이 이미 있는 경우에는, '해당 말 위에' 이동하려던 말을 올려둡니다. 이미 말이 올려져 있는 상태에도 말을 올릴 수 '있습니다'.
// => 밑바닥에 있는 것부터 옆칸의 top으로 옮긴다.

// 2. 이동하려는 칸이 빨간색인 경우에는 해당 칸으로 '이동하기 전', '(현재 칸의) 순서를 뒤집습니다.'
// 이후, 해당 칸에 말이 있는 경우에는 흰색 칸과 같이 '그 위에 쌓아둡니다.'
// => 가려는 칸이 빨간색이면, 현재 칸에 쌓인 것들(전부)의 순서를 뒤집어서 가려는 칸에 있는 말들 위에 쌓는다.(현재 칸의 top에 있는 것부터 옆칸의 top으로 옮긴다.) 

// 이동하려는 칸이 파란색일 경우에는, 방향을 반대로 전환한 뒤 이동합니다. 
// 만일 반대 방향으로 전환한 뒤 이동하려는 칸도 파란색이라면, 방향만 반대로 전환한 뒤 '이동하지 않고 가만히 있습니다.' 
// 이동하려는 말에 다른 말들이 쌓여있을 경우에, '이동하려는 말만' 방향을 반대로 바꿔야 함에 유의합니다.

// 격자판의 범위를 벗어나는 이동일 경우에도, 파란색으로 이동하려는 것과 똑같이 처리해줍니다.

// 쌓여있는 말을 이동하는 경우에는, '본인 위에' 있는 말과 함께 이동해줍니다. 

// 게임이 진행되는 동안 '아직 한 턴이 다 끝나지 않은 경우더라도' 말이 4개 이상 겹쳐지는 경우가 생긴다면 그 즉시 게임을 종료합니다. 

// 초기 상태에 4개 이상 겹쳐진 입력은 주어지지 않습니다. <-- 시작하자마자 끝나는 경우는 없다.

// 격자판의 상태와 말의 위치, 이동 방향이 주어질 때 게임이 종료되는 순간의 턴의 번호를 구하는 프로그램을 설계하세요.


// input d는 1: 오른쪽, 2: 왼쪽, 3: 윗쪽, 4: 아랫쪽

// 게임이 종료되는 턴의 번호를 출력합니다. 답이 1000보다 크거나 불가능한 경우에는 -1을 출력합니다.


//첫번째 줄에는 윷놀이 판의 크기 n, 말의 개수 k가 공백을 두고 주어집니다.
//
//두번째 줄부터(n + 1)번째 줄까지 윷놀이 판의 정보가 주어집니다. 0은 흰색 판, 1은 빨간색 판, 2는 파란색 판을 의미합니다.
//
//(n + 2)번째 줄부터 k개의 줄에는 말의 정보가 1번 말부터 차례대로 주어집니다.말의 정보는 위치 x, y와 방향 d로 주어지며, x는 몇 번째 행인지를 y는 몇 번째 열인지를 의미합니다.d는 1일 경우 오른쪽, 2일 경우 왼쪽, 3일 경우 윗쪽, 4일 경우 아랫쪽을 의미합니다.
//
//4 ≤ n ≤ 12
//
//4 ≤ k ≤ 10
//
//같은 칸에 두 개의 말이 입력으로 주어지는 경우는 없다고 가정해도 좋습니다.


// ** map들을 필요에 의해 별도로 관리하더라도, 초기 세팅은 잊지 않고 잘 해놔야 한다...!!(자칫 빈 칸으로 시작할 수 있다..!)
// ** 문제를 똑바로 이해해야 한다..! 설명이 명확하지 않고 찍어서 이해해야 할 것 같은 부분은, 내가 뭘 놓친 거다. 설명이나, 예제에라도 분명히 답이 있다.
// 
// ** whiteMove 가동 조건: colorMap[nr][nc] == 0,
//    colorMap 범위는(1,1) ~ (n,n)까지 쓰기 위해 n+1*n+1 로 설정하고 0으로 초기화되어 있었다.
//    => 여기서 로직 구멍 발생...!!! 
//      ==> 따라서, 선언시에, 애초에 조건문에 쓰이지 않는 값으로(ex. -1)로 초기화를 정하거나, Bound 체크를 그 때 그 때 다시 해줘야 한다! 


#include <iostream>
#include <vector>
#include <deque>

using namespace std;

struct Info
{
    int r, c, d;
};

//d는 1: 오른쪽, 2 : 왼쪽, 3 : 윗쪽, 4 : 아랫쪽
int dr[] = { 100, 0, 0, -1, 1 };
int dc[] = { 100, 1, -1, 0, 0 };

int n, k;
vector<vector<deque<int>>> pMap(13, vector<deque<int>>(13, deque<int>()));
vector<vector<int>> colorMap(13, vector<int>(13, -1));
vector<Info> piece(10);
bool isEnd = false;

bool isInBound(int r, int c) {
    return 0 < r && r <= n && 0 < c && c <= n;
}

int changeDir(int d) {
    if (d == 1) return 2;
    if (d == 2) return 1;
    if (d == 3) return 4;
    if (d == 4) return 3;
}

void whiteMove(int idx, int r, int c, int nr, int nc) {
    for (int i = pMap[r][c].size() - 1; 0 <= i; i--)
    {
        if (pMap[r][c][i] == idx)
        {
            for (int j = i; 0 <= j; j--)
            {
                int v = pMap[r][c][j];
                pMap[nr][nc].push_front(v);
                piece[v].r = nr;
                piece[v].c = nc;
            }

            for (int j = 0; j <= i; j++)
            {
                pMap[r][c].pop_front();
            }

            if (4 <= pMap[nr][nc].size()) isEnd = true;
            return;
        }
    }
}

void redMove(int idx, int r, int c, int nr, int nc) {
    // 문제를 잘못 이해함;;
    //deque<int> temp;
    //for (int i = 0; i < pMap[r][c].size(); i++)
    //{
    //    temp.push_front(pMap[r][c][i]);
    //}
    //pMap[r][c] = temp;

    for (int i = pMap[r][c].size() - 1; 0 <= i; i--)
    {
        if (pMap[r][c][i] == idx)
        {
            for (int j = 0; j <= i; j++)
            {
                int v = pMap[r][c][0];
                pMap[nr][nc].push_front(v);
                piece[v].r = nr;
                piece[v].c = nc;

                pMap[r][c].pop_front();
            }

            if (4 <= pMap[nr][nc].size()) isEnd = true;
            return;
        }
    }
}

int main() {
    isEnd = false;
    cin >> n >> k;
    for (int r = 1; r <= n; r++)
    {
        for (int c = 1; c <= n; c++)
        {
            cin >> colorMap[r][c];
        }
    }
    for (int i = 0; i < k; i++)
    {
        cin >> piece[i].r >> piece[i].c >> piece[i].d;
        pMap[piece[i].r][piece[i].c].push_back(i);
    }

    int turnCnt = 1;
    while (true)
    {
        if (1000 < turnCnt) {
            turnCnt = -1;
            break;
        }

        for (int i = 0; i < k; i++)
        {
            int r = piece[i].r;
            int c = piece[i].c;
            int d = piece[i].d;

            int nr = r + dr[d];
            int nc = c + dc[d];

            // 맵 바깥으로 이동하거나, 파란색일 경우 방향과 다음r,c값 재변경
            if (!isInBound(nr, nc) || colorMap[nr][nc] == 2)
            {
                d = changeDir(d);
                piece[i].d = d;
                nr = r + dr[d];
                nc = c + dc[d];

                //// 방향을 바꾼 후에도 여전히 맵 밖이거나 파란색이면 이동하지 않음
                // => 이 코드를 달았더니 Pass가 떴다. 근데, 그게 이상했다. 이게 있으나 없으나 다른 게 없는데.... 알고보니,
                //if (!isInBound(nr, nc) || colorMap[nr][nc] == 2) 
                //{
                //    continue;  // 다음 말로 넘어감
                //}
            }

            // 여기가 문제였던 것. Bound체크는 위에서 한 번만 하고 있고, 거기거 팅겨서 방향 바뀌었을 땐 체크하지 않는다.
            // => 문제는, 쓰면 안되는 범위(0,0)라인도 0으로 초기화되어 있음.... 벽으로 인식해서 넘어가야 하는데, 그러질 않도록 구현되어 있었다..!
            if (colorMap[nr][nc] == 0)
            {
                whiteMove(i, r, c, nr, nc);
                if (isEnd) break;
            }
            else if (colorMap[nr][nc] == 1)
            {
                redMove(i, r, c, nr, nc);
                if (isEnd) break;
            }

            // 문제를 잘못 이해함;;
            //else
            //{
            //    d = changeDir(d);
            //    piece[i].d = d;
            //}
        }
        if (isEnd) break;

        turnCnt += 1;
    }

    cout << turnCnt << '\n';
    return 0;
}