// 9:01 ~ 

// 1이상 6이하 중 임의의 숫자가 그려진 n * n 격자판에 
// 한 면이 1 * 1 크기인 정육면체를 놓여져 있습니다. 해당 격자판에서 정육면체를 굴리려합니다.

// 처음 정육면체의 각 면에는 1부터 6까지의 숫자가 다음과 같이 쓰여져 있고 m번에 걸쳐 주사위를 계속 '1칸씩' 굴리게 됩니다. 
// 이때, '마주보는 면에 적혀있는 숫자의 합'은 정확히 7입니다.

// 주사위는 항상 초기에 격자판의 '1행 1열'에 놓여져 있고, '처음에는 항상 오른쪽'으로 움직입니다.

// 주사위를 '움직일때마다', 
// 격자판 위 주사위가 놓여있는 칸에 적혀있는 숫자와 
// 상하좌우로 인접하며 '같은 숫자가 적혀있는' '모든 칸의 합만큼' 점수를 얻게 됩니다.

// 이때, 주사위의 아랫면이 보드의 해당 칸에 있는 숫자보다 '크면' 
// 현재 진행방향에서 90' '시계방향으로' 회전하여 다시 이동을 진행하게 되고, 
// 
// 주사위의 아랫면의 숫자가 더 '작다면' 현재 진행방향에서 90' '반시계방향으로' 회전하게 되며, 
// '동일하다면' 현재 방향으로 계속 진행하게 됩니다.

// 만약 진행 도중, 가야할 곳이 격자판을 벗어나게 된다면, 방향이 반대로 바뀌게 된 뒤, '한 칸 움직이게 됩니다'.

// n * n 크기의 격자판의 상태가 주어졌을 때, m번 진행하며 얻게되는 점수의 총 합을 구하는 프로그램을 작성해보세요.

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Spot
{
	int r, c;
};

int n, m, sr, sc, cd, totalScore;
vector<vector<int>> map(21, vector<int>(21));
vector<vector<int>> visited(21, vector<int>(21, 0));
vector<int> dice = { 0, 1, 2, 3, 4, 5, 6 }; // <-- 6번째가 bottom
queue<Spot> q;

int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

bool isInBound(int r, int c) {
	return 1 <= r && r <= n && 1 <= c && c <= n;
}

void diceMove() {
	sr = sr + dr[cd];
	sc = sc + dc[cd];

	if (!isInBound(sr, sc))
	{
		cd = (cd + 2) % 4;
		sr = sr + (dr[cd] * 2);
		sc = sc + (dc[cd] * 2);
	}

	// 상 우 하 좌
	int temp;
	switch (cd)
	{
	//상
	case 0:
		temp = dice[1];
		dice[1] = dice[2];
		dice[2] = dice[6];
		dice[6] = dice[5];
		dice[5] = temp;

		break;
	//우
	case 1:
		temp = dice[1];
		dice[1] = dice[4];
		dice[4] = dice[6];
		dice[6] = dice[3];
		dice[3] = temp;

		break;
	//하
	case 2:
		temp = dice[1];
		dice[1] = dice[5];
		dice[5] = dice[6];
		dice[6] = dice[2];
		dice[2] = temp;

		break;
	//좌
	case 3:
		temp = dice[1];
		dice[1] = dice[3];
		dice[3] = dice[6];
		dice[6] = dice[4];
		dice[4] = temp;

		break;
	default:
		break;
	}
}

void getScoreBFS(int turnCnt) {
	int v = map[sr][sc];
	int sameCnt = 1;
	q.push({ sr, sc });
	visited[sr][sc] = turnCnt;

	while (!q.empty())
	{
		int r = q.front().r;
		int c = q.front().c;
		q.pop();

		for (int d = 0; d < 4; d++)
		{
			int nr = r + dr[d];
			int nc = c + dc[d];

			//if (isInBound(nr, nc) && map[nr][nc] == v && visited[nr][nc] != turnCnt); <-- 이건 대체 뭔 실수냐.....?
			if (isInBound(nr, nc) && map[nr][nc] == v && visited[nr][nc] != turnCnt)
			{
				visited[nr][nc] = turnCnt;
				sameCnt++;
				q.push({ nr, nc });
			}
		}
	}
	totalScore += (v * sameCnt);
}

void setNextDir() {
	if (dice[6] > map[sr][sc]) cd = (cd + 1) % 4;
	else if (dice[6] < map[sr][sc]) cd = (cd + 3) % 4;

	return;
}

int main() {
	sr = 1;
	sc = 1;
	cd = 1;
	totalScore = 0;

    cin >> n >> m;
	for (int r = 1; r <= n; r++)
	{
		for (int c = 1; c <= n; c++)
		{
			cin >> map[r][c];
		}
	}

	for (int t = 1; t <= m; t++)
	{
		// 1. 주사위 이동
		diceMove();
		// 2. 점수ㅇㅇ(BFS)
		getScoreBFS(t);
		// 3. 다음 방향 정하기
		setNextDir();
	}
	cout << totalScore << '\n';

    return 0;
}