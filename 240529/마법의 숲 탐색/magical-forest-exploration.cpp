#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Golem {
	int r = -1;
	int c;
	int exitDir;
};

struct Spot {
	int r;
	int c;
};

// 0, 1, 2, 3은 북, 동, 남, 서쪽을 의미합니다.
int exitDr[] = { -1, 0, 1, 0 };
int exitDc[] = { 0, 1, 0, -1 };

int R, C, K, currDir;
int total = 0;
int maxRow = 0;
vector<vector<int>> map(71, vector<int>(71, 0));
vector<vector<int>> exitMap(71, vector<int>(71, 0));
vector<vector<int>> visited(71, vector<int>(71, 0));
vector<Golem> golemList(1001);
Spot leftSpot;
Spot rightSpot;
Spot top;
Spot bottom;
bool isSuccessed = false;

bool isOutOfBound(int r, int c) {
	if (r < 1 || R < r || c < 1 || C < c) return true;
	return false;
}

bool isPossibleGoBottom() {
	if (bottom.r < 1) {
		if (map[bottom.r + 1][bottom.c] == 0) return true;
		return false;
	}
	else {
		if (!isOutOfBound(bottom.r + 1, bottom.c) && map[bottom.r + 1][bottom.c] == 0 && map[leftSpot.r + 1][leftSpot.c] == 0 && map[rightSpot.r + 1][rightSpot.c] == 0) {
			return true;
		}
		return false;
	}
}
bool isPossibleGoLeft() {
	if (bottom.r < 1) {
		if (map[bottom.r + 1][bottom.c - 1] == 0 && !isOutOfBound(leftSpot.r, leftSpot.c - 1)) return true;
		return false;
	}
	else if (bottom.r == 1) {
		if (isOutOfBound(leftSpot.r + 1, leftSpot.c - 1) || map[leftSpot.r + 1][leftSpot.c - 1] != 0) return false;
		if (map[leftSpot.r + 1][leftSpot.c] != 0) return false;
		if(isOutOfBound(leftSpot.r + 2, leftSpot.c) || map[leftSpot.r + 2][leftSpot.c] != 0) return false;

		return true;
	}
	else if (bottom.r == 2) {
		if (isOutOfBound(leftSpot.r + 1, leftSpot.c - 1) || map[leftSpot.r + 1][leftSpot.c - 1] != 0) return false;
		if (map[leftSpot.r + 1][leftSpot.c] != 0) return false;
		if (isOutOfBound(leftSpot.r + 2, leftSpot.c) || map[leftSpot.r + 2][leftSpot.c] != 0) return false;
		if (isOutOfBound(leftSpot.r, leftSpot.c - 1) || map[leftSpot.r][leftSpot.c - 1] != 0) return false;

		return true;
	}
	else {
		if (isOutOfBound(leftSpot.r + 1, leftSpot.c - 1) || map[leftSpot.r + 1][leftSpot.c - 1] != 0) return false;
		if (map[leftSpot.r + 1][leftSpot.c] != 0) return false;
		if (isOutOfBound(leftSpot.r + 2, leftSpot.c) || map[leftSpot.r + 2][leftSpot.c] != 0) return false;
		if (isOutOfBound(leftSpot.r, leftSpot.c - 1) || map[leftSpot.r + 2][leftSpot.c - 1] != 0) return false;
		if (map[leftSpot.r - 1][leftSpot.c - 1] != 0) return false;

		return true;
	}
}
bool isPossibleGoRight() {
	if (bottom.r < 1) {
		if (map[bottom.r + 1][bottom.c + 1] == 0 && !isOutOfBound(rightSpot.r, rightSpot.c + 1)) return true;
		return false;
	}
	else if (bottom.r == 1) {
		if (isOutOfBound(rightSpot.r + 1, rightSpot.c + 1) || map[rightSpot.r + 1][rightSpot.c + 1] != 0) return false;
		if (map[rightSpot.r + 1][rightSpot.c] != 0) return false;
		if (isOutOfBound(rightSpot.r + 2, rightSpot.c) || map[rightSpot.r + 2][rightSpot.c] != 0) return false;

		return true;
	}
	else if (bottom.r == 2) {
		if (isOutOfBound(rightSpot.r + 1, rightSpot.c + 1) || map[rightSpot.r + 1][rightSpot.c + 1] != 0) return false;
		if (map[rightSpot.r + 1][rightSpot.c] != 0) return false;
		if (isOutOfBound(rightSpot.r + 2, rightSpot.c) || map[rightSpot.r + 2][rightSpot.c] != 0) return false;
		if (isOutOfBound(rightSpot.r, rightSpot.c + 1) || map[rightSpot.r][rightSpot.c + 1] != 0) return false;

		return true;
	}
	else {
		if (isOutOfBound(rightSpot.r + 1, rightSpot.c + 1) || map[rightSpot.r + 1][rightSpot.c + 1] != 0) return false;
		if (map[rightSpot.r + 1][rightSpot.c] != 0) return false;
		if (isOutOfBound(rightSpot.r + 2, rightSpot.c) || map[rightSpot.r + 2][rightSpot.c] != 0) return false;
		if (isOutOfBound(rightSpot.r, rightSpot.c + 1) || map[rightSpot.r + 2][rightSpot.c + 1] != 0) return false;
		if (map[rightSpot.r - 1][rightSpot.c + 1] != 0) return false;

		return true;
	}
}

void moveGolem(int cnt) {
	int centerR = golemList[cnt].r;
	int centerC = golemList[cnt].c;
	int exitDir = golemList[cnt].exitDir;

	while (true) {
		leftSpot = { centerR, centerC - 1 };
		rightSpot = { centerR, centerC + 1 };
		top = { centerR - 1, centerC };
		bottom = { centerR + 1, centerC };

		if (isPossibleGoBottom()) {
			centerR += 1;
		}
		else if (isPossibleGoLeft()) {
			centerR += 1;
			centerC -= 1;

			exitDir = (exitDir - 1) % 4;
		}
		else if (isPossibleGoRight()) {
			centerR += 1;
			centerC += 1;

			exitDir = (exitDir + 1) % 4;
		}
		else break;
	}

	// 더 움직일 수 없는데, r-1 < 1 이면 리셋!
	if (top.r < 1) {
		isSuccessed = false;
		fill(map.begin(), map.end(), vector<int>(71, 0));
		fill(exitMap.begin(), exitMap.end(), vector<int>(71, 0));
		return;
	}
	else {
		isSuccessed = true;
		map[centerR][centerC] = cnt;
		int r, c, exitR, exitC;

		for (int d = 0; d < 4; d++)
		{
			r = centerR + exitDr[d];
			c = centerC + exitDc[d];

			map[r][c] = cnt;
		}

		// 최종 출구 표시
		exitR = centerR + exitDr[exitDir];
		exitC = centerC + exitDc[exitDir];
		exitMap[exitR][exitC] = 1;
		
		golemList[cnt].r = centerR;
		golemList[cnt].c = centerC;

		return;
	}
}

void moveFairyDFS(int r, int c) {
	if (maxRow < r) {
		maxRow = r;
	}

	int nextR, nextC;
	int currentValue = map[r][c];

	for (int dir = 0; dir < 4; dir++)
	{
		nextR = r + exitDr[dir];
		nextC = c + exitDc[dir];

		if (!isOutOfBound(nextR, nextC) && map[nextR][nextC] != 0 && visited[nextR][nextC] != 1) {
			if (map[nextR][nextC] == currentValue) {
				visited[nextR][nextC] = 1;
				moveFairyDFS(nextR, nextC);
				visited[nextR][nextC] = 0;
			}
			else if (exitMap[r][c] == 1) {
				visited[nextR][nextC] = 1;
				moveFairyDFS(nextR, nextC);
				visited[nextR][nextC] = 0;
			}
		}
	}
	return;
}

int main() {
    cin >> R >> C >> K;
	for (int i = 1; i <= K; i++)
	{
		cin >> golemList[i].c >> golemList[i].exitDir;
	}

	for (int i = 1; i <= K; i++)
	{
		moveGolem(i);

		maxRow = 0;
		if(isSuccessed) moveFairyDFS(golemList[i].r, golemList[i].c);

		total += maxRow;

	}

	cout << total << endl;

    return 0;
}