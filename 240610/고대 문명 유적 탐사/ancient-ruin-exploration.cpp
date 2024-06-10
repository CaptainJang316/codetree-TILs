// 12:25 ~ 12:40, 1:15 ~ 2:40, 3:00 ~ 

// '가능한 회전의 방법 중' (1) 유물 1차 획득 가치를 최대화하고, 
// 그러한 방법이 여러가지인 경우 (2) 회전한 각도가 가장 작은 방법을 선택합니다. 
// 그러한 경우도 여러가지인 경우 (3) 회전 중심 좌표의 열이 가장 작은 구간을, 
// 그리고 열이 같다면 행이 가장 작은 구간을 선택합니다.

// 조각이 사라진 위치에는 
// 유적의 벽면에 적혀있는 순서대로 새로운 조각이 생겨납니다. <-- queue로 관리
// 새로운 조각은 (1) 열 번호가 작은 순으로 조각이 생겨납니다. 
// 만약 열 번호가 같다면 (2) 행 번호가 큰 순으로 조각이 생겨납니다. 
// 단, 벽면의 숫자는 충분히 많이 적혀 있어 생겨날 조각의 수가 부족한 경우는 없다고 가정해도 좋습니다.

// 단, 유적의 벽면에 써 있는 숫자를 사용한 이후에는 다시 사용할 수 없으므로 이후 부터는 남은 숫자부터 순서대로 사용합니다. 
// 즉, 이후에는 아래 그림에서 8번째로 적혀있는 수인 1부터 다시 사용이 가능합니다.

// 각 턴마다 획득한 유물의 가치의 총합을 출력하는 프로그램을 작성해야 합니다. 
// 단, 아직 K번의 턴을 진행하지 못했지만, 탐사 진행 과정에서 어떠한 방법을 사용하더라도 유물을 획득할 수 없었다면 모든 탐사는 '그 즉시 종료'됩니다. 
// 이 경우 얻을 수 있는 유물이 존재하지 않음으로, '종료되는 턴에 아무 값도 출력하지 않음'에 유의합니다.

// 단, 초기에 주어지는 유적지에서는 탐사 진행 이전에 유물이 발견되지 않으며, 첫 번째 턴에서 탐사를 진행한 이후에는 항상 유물이 발견됨을 가정해도 좋습니다.

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Spot {
	int r;
	int c;
};


int K, M;
vector<vector<int>> map(5, vector<int>(5, 0));
vector<vector<int>> afterCurrCaseMap(5, vector<int>(5, 0));
vector<vector<int>> bestCaseMap(5, vector<int>(5, 0));
vector<int> pieceBox;
vector<int> pieceBox_temp;
int currCaseScore = 0;
int bestCaseScore = 0;
vector<int> totalScoreList;
vector<Spot> shouldRemovePieceList;

int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

bool isOutOfBound(int r, int c) {
	if (r < 0 || 5 <= r || c < 0 || 5 <= c) return true;
	return false;
}

// 조각이 사라진 위치에는 
// 유적의 벽면에 적혀있는 순서대로 새로운 조각이 생겨납니다. <-- queue로 관리
// 새로운 조각은 (1) 열 번호가 작은 순으로 조각이 생겨납니다. 
// 만약 열 번호가 같다면 (2) 행 번호가 큰 순으로 조각이 생겨납니다. 
// 단, 벽면의 숫자는 충분히 많이 적혀 있어 생겨날 조각의 수가 부족한 경우는 없다고 가정해도 좋습니다.
bool spotCompare(Spot& Spot1, Spot& Spot2) {
	if (Spot1.c == Spot2.c) return Spot1.r > Spot2.r;
	else return Spot1.c < Spot2.c;
}

bool bfs(vector<vector<int>> &currMap, int r, int c, vector<vector<int>> &visited) {
	int nextR, nextC;
	bool isRelicExist = false;
	vector<Spot> samePieceList;
	queue<Spot> pieceQueue;
	pieceQueue.push({ r, c });
	visited[r][c] = 1;
	samePieceList.push_back({ r, c });

	int currValue = currMap[r][c];

	while (!pieceQueue.empty()) {
		int currR = pieceQueue.front().r;
		int currC = pieceQueue.front().c;
		pieceQueue.pop();

		for (int dir = 0; dir < 4; dir++)
		{
			nextR = currR + dr[dir];
			nextC = currC + dc[dir];

			if (!isOutOfBound(nextR, nextC) && visited[nextR][nextC] != 1 && currMap[nextR][nextC] == currValue) {
				visited[nextR][nextC] = 1;
				samePieceList.push_back({ nextR, nextC });
				pieceQueue.push({ nextR, nextC });
			}
		}
	}

	if (3 <= samePieceList.size()) {
		isRelicExist = true;

		for (int i = 0; i < samePieceList.size(); i++)
		{
			shouldRemovePieceList.push_back(samePieceList[i]);
		}
		samePieceList.clear();
	}
	else samePieceList.clear();

	return isRelicExist;
}

// 이 때, 더는 유물이 나오지 않을 때까지 반복시킬 것
void getRelics(vector<vector<int>> rotated_map, int startR, int startC) {
	//현재, 회전만 되어 있는 시점.

	bool isRelicExist = true;

	pieceBox_temp = pieceBox;

	while (isRelicExist)
	{
		vector<vector<int>> visited(5, vector<int>(5, 0));
		isRelicExist = false;
		for (int r = 0; r < 5; r++)
		{
			for (int c = 0; c < 5; c++)
			{
				if (visited[r][c] != 1) {
					if (bfs(rotated_map, r, c, visited)) {
						isRelicExist = true;
					}
				}
			}
		}
		//for (int r = startR; r < startR + 3; r++) <--- 범위를 일부러 좁게 설정했는데, 그것 땜에 오히려 예외가 발생함.(처음 시작점은 맞아도, 그게 어디까지 번질지 모른다. 좁게 설정한 범위 밖에 그 영향으로 새로 유물이 생기면, 감지를 못함)
		//{
		//	for (int c = startC; c < startC + 3; c++)
		//	{
		//		if (visited[r][c] != 1) {
		//			if (bfs(rotated_map, r, c, visited)) {
		//				isRelicExist = true;
		//			}
		//		}
		//	}
		//}

		if (isRelicExist) {
			currCaseScore += shouldRemovePieceList.size();

			sort(shouldRemovePieceList.begin(), shouldRemovePieceList.end(), spotCompare);

			int r, c;
			while (0 < shouldRemovePieceList.size())
			{
				r = shouldRemovePieceList[0].r;
				c = shouldRemovePieceList[0].c;

				rotated_map[r][c] = pieceBox_temp[0];
				pieceBox_temp.erase(pieceBox_temp.begin());

				shouldRemovePieceList.erase(shouldRemovePieceList.begin());
			}
		}
		visited.clear();
	}

	afterCurrCaseMap = rotated_map;
}

void rotateAndSimulation(int startR, int startC) {
	vector<vector<int>> currMap(map);
	vector<vector<int>> rotated_map(map);

	//90, 180, 270도 회전한 경우들 각각 테스트
	for (int i = 0; i < 3; i++)
	{
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				rotated_map[startR + c][startC + 3 - r - 1] = currMap[startR + r][startC + c];
			}
		}

		// 각각의 회전한 Case마다 발견 가능한 유물들 끝까지 탐색 & 발견된 유물 조각 갯수 Get
		currCaseScore = 0;
		getRelics(rotated_map, startR, startC);

		// 현재 bestCaseScore보다 새로 Get한 유물수가 많으면 Update
		if (bestCaseScore < currCaseScore) {
			bestCaseScore = currCaseScore;
			//bestCaseMap = rotated_map; <---- 사용되는 vector들이 많을 때, 적절치 못한 걸 사용하게 되는 실수 발생(단순 잘못 기입이 아니라, 작업 후의 상태를 기입한 map이 따로 없었다)
			bestCaseMap = afterCurrCaseMap;
		}

		currMap = rotated_map;
	}
}

int main() {
	cin >> K >> M;
	for (int r = 0; r < 5; r++)
	{
		for (int c = 0; c < 5; c++)
		{
			cin >> map[r][c];
		}
	}

	int piece;
	for (int i = 0; i < M; i++)
	{
		cin >> piece;
		pieceBox.push_back(piece);
	}

	for (int i = 0; i < K; i++)
	{
		//매 턴마다 해당 턴의 bestCaseScore 초기화
		bestCaseScore = 0;

		//매 턴마다 가능한 모든 방법 탐색
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				rotateAndSimulation(r, c);
			}
		}

		//이번 턴에서 bestCaseScore에 변화 없으면 break;
		if (bestCaseScore == 0) break;

		map = bestCaseMap;

		pieceBox.erase(pieceBox.begin(), pieceBox.begin() + bestCaseScore);

		totalScoreList.push_back(bestCaseScore);
	}

	for (int i = 0; i < totalScoreList.size(); i++)
	{
		cout << totalScoreList[i] << " ";
	}
	cout << endl;
	return 0;
}