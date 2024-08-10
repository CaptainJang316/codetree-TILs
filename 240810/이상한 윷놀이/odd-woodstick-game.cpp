// 11:11 ~ 1:23, 2:12 ~ 2:46, 11:28 ~ 12:32

// 새로운 게임은 크기가 N×N인 체스판에서 진행되고, 사용하는 말의 개수는 K개이다.
// 
// 말은 원판모양이고, 하나의 말 위에 다른 말을 올릴 수 있다. 
// 체스판의 각 칸은 흰색, 빨간색, 파란색 중 하나로 색칠되어있다.

// 게임은 체스판 위에 말 K개를 놓고 시작한다. 
// 말은 1번부터 K번까지 번호가 매겨져 있고, 이동 방향도 미리 정해져 있다. 
// 이동 방향은 위, 아래, 왼쪽, 오른쪽 4가지 중 하나이다.

// 턴 한 번은 1번 말부터 K번 말까지 순서대로 이동시키는 것이다. 한 말이 이동할 때 위에 올려져 있는 말도 함께 이동한다. 
// 말의 이동 방향에 있는 칸에 따라서 말의 이동이 다르며 아래와 같다. 턴이 진행되던 중에 말이 4개 이상 쌓이는 순간 게임이 종료된다.

// 기존에 말이 있으면 그 위에 쌓는다.
// 흰색으로 이동 -> 그대로 위에 쌓는다.
// 빨간색으로 이동 -> 기존의 말들은 그대로, 그 위에 새로 온것들의 순서를 바꿔서 쌓는다.
// 파란색 or 범위 밖으로 이동 -> 반대방향으로 한 칸 이동(if 거기도 파란색이면 이동하지 않음.)

// 체스판의 크기와 말의 위치, 이동 방향이 모두 주어졌을 때, 게임이 종료되는 턴의 번호를 구해보자.

// 첫째 줄에 체스판의 크기 N, 말의 개수 K가 주어진다. 
// 둘째 줄부터 N개의 줄에 체스판의 정보가 주어진다. 체스판의 정보는 정수로 이루어져 있고, 각 정수는 칸의 색을 의미한다. 0은 흰색, 1은 빨간색, 2는 파란색이다.
// 
// 다음 K개의 줄에 말의 정보가 1번 말부터 순서대로 주어진다.말의 정보는 세 개의 정수로 이루어져 있고, 순서대로 행, 열의 번호, 이동 방향이다.
// 행과 열의 번호는 1부터 시작하고, 이동 방향은 4보다 작거나 같은 자연수이고 1부터 순서대로 →, ←, ↑, ↓의 의미를 갖는다.
// 같은 칸에 말이 두 개 이상 있는 경우는 입력으로 주어지지 않는다.

// 게임이 종료되는 턴의 번호를 출력한다. 그 값이 1,000보다 크거나 절대로 게임이 종료되지 않는 경우에는 -1을 출력한다.


// ** 방향 번호 순서가 어떻게 되는지를 잘 숙지할 것!(상황에 따라 방향을 바꿔줘야 할 때, (dir + 2) % 4 같은 공식이 안맞을 수 있다.
//    이 문제의 경우, 방향 순서가 →, ←, ↑, ↓이므로, 일반적인 방향 전환 공식으로는 안된다.
// ** 이동 방향은 4보다 작거나 같은 자연수이고 1부터 순서대로 →, ←, ↑, ↓의 의미를 갖는다.(4 -> 3으로 변경)
// ** 요구사항들 잘못 이해함....;;;;;; <-- 문제 좀 제대로 보자. 예시 시뮬레이션 보고 헷갈리지 말자.

#include <iostream>
#include <vector>
#include <stack>
#include <deque>

using namespace std;

int dr[] = { 1000, 0, 0, -1, 1 };
int dc[] = { 1000, 1, -1, 0, 0 };

struct SpotInfo
{
	int n;
	int dir;
};

struct NInfo
{
	int r;
	int c;
	int dir;
};

int N, K;
vector<vector< int>> colorMap(13, vector< int>(13, 0));
deque<deque< deque< SpotInfo>>> nMap(13, deque< deque< SpotInfo>>(13));
vector< NInfo> nList(11);
stack< SpotInfo> st;
bool endGame = false;

int changeDir(int dir) {
	if (dir == 1) dir = 2;
	else if (dir == 2) dir = 1;
	else if (dir == 3) dir = 4;
	else if (dir == 4) dir = 3;

	return dir;
}

bool isOutOfBound(int r, int c) {
	return r < 1 || N < r || c < 1 || N < c;
}

// 이게 틀렸다. 이동하는 칸의 size만큼 꼭 다 이동하는 게 아니란 걸 간과했다... 역시 문제 풀 때 정신 잘 차리고 풀어야 함...;;;;
bool check(int r, int c) {
	//if (4 <= nMap[r][c].size() + nMap[nr][nc].size()) return true;
	if (4 <= nMap[r][c].size()) return true;
	return false;
}

void redMove(int r, int c, int nr, int nc, int curr) {

	for (int i = nMap[r][c].size() - 1; 0 <= i; i--)
	{
		if (nMap[r][c][i].n == curr)
		{
			st.push({ curr, nList[curr].dir });
			nList[curr].r = nr;
			nList[curr].c = nc;

			for (int j = i - 1; 0 <= j; j--)
			{
				int n = nMap[r][c][j].n;
				st.push({ n, nList[n].dir });
				nList[n].r = nr;
				nList[n].c = nc;
			}

			nMap[r][c].erase(nMap[r][c].begin(), nMap[r][c].begin() + i + 1);

			break;
		}
	}

	while (!st.empty()) {
		int n = st.top().n;
		int dir = st.top().dir;
		st.pop();

		nMap[nr][nc].push_front({ n, dir });
	}
}

void whiteMove(int r, int c, int nr, int nc, int curr) {
	 
	for (int i = nMap[r][c].size() - 1; 0 <= i; i--)
	{
		if (nMap[r][c][i].n == curr)
		{
			nMap[nr][nc].push_front({ curr, nList[curr].dir });
			nList[curr].r = nr;
			nList[curr].c = nc;

			for (int j = i - 1; 0 <= j; j--)
			{
				int n = nMap[r][c][j].n;
				nMap[nr][nc].push_front({ n, nList[n].dir });
				nList[n].r = nr;
				nList[n].c = nc;
			}

			nMap[r][c].erase(nMap[r][c].begin(), nMap[r][c].begin() + i + 1);

			break;
		}
	}
}

void blueMove(int r, int c, int curr) {

	int newDir = changeDir(nList[curr].dir);

	for (int i = nMap[r][c].size() - 1; 0 <= i; i--)
	{
		if (nMap[r][c][i].n == curr) {

			nMap[r][c][i].dir = newDir;
			nList[curr].dir = newDir;
			break;
		}
	}
}

int main() {
	cin >> N >> K;
	for (int r = 1; r <= N; r++)
	{
		for (int c = 1; c <= N; c++)
		{
			cin >> colorMap[r][c];
		}
	}

	for (int i = 1; i <= K; i++)
	{
		int r, c, dir;
		cin >> r >> c >> dir;
		nList[i].r = r;
		nList[i].c = c;
		nList[i].dir = dir;

		nMap[r][c].push_back({ i, dir });
	}

	int cnt = 1;
	while (true) {

		for (int i = 1; i <= K; i++)
		{
			int r = nList[i].r;
			int c = nList[i].c;

			int nr = r + dr[nList[i].dir];
			int nc = c + dc[nList[i].dir];

			if (isOutOfBound(nr, nc) || colorMap[nr][nc] == 2)
			{
				blueMove(r, c, i);

				nr = r + dr[nList[i].dir];
				nc = c + dc[nList[i].dir];

				if (isOutOfBound(nr, nc) || colorMap[nr][nc] == 2) continue;
			}
			
			if (colorMap[nr][nc] == 1)
			{
				redMove(r, c, nr, nc, i);
				if (check(nr, nc)) {
					endGame = true;
					break;
				}
			}
			else
			{
				whiteMove(r, c, nr, nc, i);
				if (check(nr, nc)) {
					endGame = true;
					break;
				}
			}
		}

		if (endGame) break;
		cnt += 1;
		if (1000 < cnt) break;
	}
	if (1000 < cnt) cnt = -1;

	cout << cnt << '\n';
	return 0;
}