// 9:01 ~ 9:52 <-- 1시간 소요!

// ** if (isInBound(nr, nc) && map[nr][nc] == v && visited[nr][nc] != turnCnt); <-- 이건 대체 뭔 실수냐.....?
//    => 컴파일 에러로 안잡아서 더 골아프다. 조심할 것..!!

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