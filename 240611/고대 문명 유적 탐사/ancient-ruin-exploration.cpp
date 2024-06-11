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


//** 문제 이해 또 잘못함 <-- 이 문제에서 고생한 이유
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 가능한 회전의 방법 중 (1) '유물 1차 획득 가치를 최대화'하고, 그러한 방법이 여러가지인 경우 (2) 회전한 각도가 가장 작은 방법을 선택합니다.//
// 그러한 경우도 여러가지인 경우 (3) 회전 중심 좌표의 열이 가장 작은 구간을, 그리고 열이 같다면 행이 가장 작은 구간을 선택합니다.           //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * 다시 풀기
// ==> 각각의 케이스의 1차 획득가치에 대해서 아래의 정보들을 struct vector로 관리한다. 
// 1. 1차 획득 가치
// 2. 회전 각도 dir
// 3. 첫 시작 모서리 좌표(startR, startC) <- 회전 전
// => 각 턴마다 이를 모아놓고, sort 진행 & 첫번째 원소로 결정.
// ===> 해당 케이스에 대해 '1차 유물 획득 & 유물 연쇄 획득'까지 쭈욱 진행하고 map & 남은 벽조각 정보에 반영

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Spot {
	int r;
	int c;
};

struct Candidate {
	int firstScore = 0;
	int dir;
	int startR;
	int startC;
};


int K, M;
vector<vector<int>> map(5, vector<int>(5, 0));
vector<vector<int>> afterCurrCaseMap(5, vector<int>(5, 0));
vector<vector<int>> bestCaseMap(5, vector<int>(5, 0));
vector<int> pieceBox;
int currCaseScore = 0;
int bestCaseScore = 0;
vector<int> totalScoreList;
vector<Candidate> candidateList;
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

int getCandidateBFS(vector<vector<int>> currMap, int r, int c, vector<vector<int>>& visited) {
	int nextR, nextC;
	int newRelicCount = 0;
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
		newRelicCount = samePieceList.size();
		samePieceList.clear();
	}
	else samePieceList.clear();

	return newRelicCount;
}

bool getRelicsBFS(int r, int c, vector<vector<int>>& visited) {
	vector<Spot> samePieceList;
	queue<Spot> pieceQueue;
	pieceQueue.push({ r, c });
	visited[r][c] = 1;
	samePieceList.push_back({ r, c });
	
	int currValue = map[r][c];

	while (!pieceQueue.empty())
	{
		int currR = pieceQueue.front().r;
		int currC = pieceQueue.front().c;
		pieceQueue.pop();
		
		int nextR, nextC;
		for (int dir = 0; dir < 4; dir++)
		{
			nextR = currR + dr[dir];
			nextC = currC + dc[dir];

			if (!isOutOfBound(nextR, nextC) && visited[nextR][nextC] != 1 && map[nextR][nextC] == currValue) {
				visited[nextR][nextC] = 1;
				pieceQueue.push({ nextR, nextC });
				samePieceList.push_back({ nextR, nextC });
			}
		}
	}

	if (3 <= samePieceList.size()) {
		for (int i = 0; i < samePieceList.size(); i++)
		{
			shouldRemovePieceList.push_back(samePieceList[i]);
		}
		samePieceList.clear();
		return true;
	}
	
	samePieceList.clear();
	return false;
}

// 이 때, 더는 유물이 나오지 않을 때까지 반복시킬 것
void getRelics() {
	vector<vector<int>> rotated_map(map);

	Candidate bestCase = candidateList[0];
	int startR = bestCase.startR;
	int startC = bestCase.startC;

	for (int dir = 0; dir <= bestCase.dir; dir++)
	{
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				rotated_map[startR + c][startC + 3 - r - 1] = map[startR + r][startC + c];
			}
		}
		map = rotated_map;
	}

	bool isRelicExist = true;
	while (isRelicExist)
	{
		vector<vector<int>> visited(5, vector<int>(5, 0));
		isRelicExist = false;
		for (int r = 0; r < 5; r++)
		{
			for (int c = 0; c < 5; c++)
			{
				if (visited[r][c] != 1) {
					if (getRelicsBFS(r, c, visited)) {
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

			bestCaseScore += shouldRemovePieceList.size();

			sort(shouldRemovePieceList.begin(), shouldRemovePieceList.end(), spotCompare);

			int r, c;
			while (0 < shouldRemovePieceList.size())
			{
				r = shouldRemovePieceList[0].r;
				c = shouldRemovePieceList[0].c;

				//cout << "?\n";
				map[r][c] = pieceBox[0];
				//cout << "!\n";
				pieceBox.erase(pieceBox.begin());

				shouldRemovePieceList.erase(shouldRemovePieceList.begin());
			}
		}
		visited.clear();
	}
}

void rotateAndSimulation(int startR, int startC) {
	vector<vector<int>> currMap(map);
	vector<vector<int>> rotated_map(map);

	//90, 180, 270도 회전한 경우들 각각 테스트
	for (int dir = 0; dir < 3; dir++)
	{
		vector<vector<int>> visited(5, vector<int>(5, 0));

		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				rotated_map[startR + c][startC + 3 - r - 1] = currMap[startR + r][startC + c];
			}
		}

		// 각각의 회전한 Case마다 발견 가능한 유물들 1차만 탐색 & 후보 Get
		currCaseScore = 0;
		for (int r = startR; r < startR + 3; r++)
		{
			for (int c = startC; c < startC + 3; c++)
			{
				if(visited[r][c] != 1) currCaseScore += getCandidateBFS(rotated_map, r, c, visited);
			}
		}

		// 후보 자격 있는 경우는 list에 추가.
		if (3 <= currCaseScore) {
			candidateList.push_back({
				currCaseScore,
				dir,
				startR,
				startC
			});
		}
		
		visited.clear();
		currMap = rotated_map;
	}

	return;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 가능한 회전의 방법 중 (1) '유물 1차 획득 가치를 최대화'하고, 그러한 방법이 여러가지인 경우 (2) 회전한 각도가 가장 작은 방법을 선택합니다.//
// 그러한 경우도 여러가지인 경우 (3) 회전 중심 좌표의 열이 가장 작은 구간을, 그리고 열이 같다면 행이 가장 작은 구간을 선택합니다.           //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool candidateCompare(Candidate& candidate1, Candidate& candidate2) {
	if (candidate1.firstScore == candidate2.firstScore) {
		if (candidate1.dir == candidate2.dir) {
			if (candidate1.startC == candidate2.startC) {
				return candidate1.startR < candidate2.startR;
			}
			else return candidate1.startC < candidate2.startC;
		}
		else return candidate1.dir < candidate2.dir;
	}
	else return candidate1.firstScore > candidate2.firstScore;
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

		//베스트 케이스 없으면 종료
		if (candidateList.size() == 0) break;

		//베스트 케이스 찾기(sorting으로ㅇㅇ)
		sort(candidateList.begin(), candidateList.end(), candidateCompare);

		// 베스트 케이스에 대해 끝까지 유물 탐색 실행
		getRelics();

		if (bestCaseScore != 0) totalScoreList.push_back(bestCaseScore);

		candidateList.clear();
	}

	for (int i = 0; i < totalScoreList.size(); i++)
	{
		cout << totalScoreList[i] << " ";
	}
	cout << endl;
	return 0;
}