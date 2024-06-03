// 11:20 ~ 

// 단, 벽면의 숫자는 충분히 많이 적혀 있어 생겨날 조각의 수가 부족한 경우는 없다고 가정해도 좋습니다.

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Spot {
   int r;
   int c;
};

int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

int K, M;
vector<vector<int>> map(5, vector<int>(5, 0));
vector<int> pieceList(300, 0);
vector<Spot> samePieceList;
int pieceCount = 0;
int totalCount = 0;
bool atLeastOneRelicExist = false;

bool isOutOfBound(int r, int c) {
   if (r < 0 || 4 < r || c < 0 || 4 < c) return true;
   return false;
}

void searchSameBlockBFS(int r, int c, vector<vector<int>> &visited) {
   queue<Spot> spotQueue;
   spotQueue.push({ r, c });
   visited[r][c] = 1;

   pieceCount = 0;

   int currR, currC, nextR, nextC;
   while (!spotQueue.empty()) {
      currR = spotQueue.front().r;
      currC = spotQueue.front().c;
      spotQueue.pop();

samePieceList.push_back({ currR, currC});
         pieceCount += 1;

         for (int dir = 0; dir < 4; dir++)
         {
            nextR = currR + dr[dir];
            nextC = currC + dc[dir];

            if (!isOutOfBound(nextR, nextC) && map[currR][currC] == map[nextR][nextC] && visited[nextR][nextC] != 1) {
               visited[nextR][nextC] = 1;
               spotQueue.push({ nextR, nextC });
            }
         }
   }

   return;
}

bool compare(Spot spot1, Spot spot2) {
   if (spot1.c != spot2.c) {
      return spot1.c < spot2.c;
   } else return spot1.r > spot2.r;
}

void changeBlocks(vector<vector<int>>& temp_map) {
   int r, c;

   sort(samePieceList.begin(), samePieceList.end(), compare);
   
   for (int i = 0; i < samePieceList.size(); i++)
   {
      r = samePieceList[i].r;
      c = samePieceList[i].c;

      temp_map[r][c] = pieceList[0];
      pieceList.erase(pieceList.begin());
   }

   return;
}

void exploreRelics(vector<vector<int>> &temp_map) {
   vector<vector<int>> visited(5, vector<int>(5, 0));
   bool isRelicExist = false;

   while (true) {
      isRelicExist = false;
      for (int r = 0; r < 5; r++)
      {
         for (int c = 0; c < 5; c++)
         {
            searchSameBlockBFS(r, c, visited);

            if (pieceCount < 3) samePieceList.clear();
            else {
               isRelicExist = true;
               atLeastOneRelicExist = true;

               totalCount += pieceCount;
               changeBlocks(temp_map);
            }
         }
      }

      map = temp_map;

      if (!isRelicExist) break;
   }
   visited.clear();

   return;
}

void makeBoundary(int startR, int startC) {
   vector<vector<int>> temp_map(map);

   for (int r = 0; r < 3; r++)
   {
      for (int c = 0; c < 3; c++)
      {
         temp_map[startR + c][startC + 3 - r - 1] = map[startR + r][startC + c];
      }
   }

   exploreRelics(temp_map);

   return;
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

   for (int i = 0; i < M; i++)
   {
      cin >> pieceList[i];
   }

   for (int i = 0; i < K; i++)
   {
      atLeastOneRelicExist = false;

      for (int r = 0; r < 3; r++)
      {
         for (int c = 0; c < 3; c++)
         {
            makeBoundary(r, c);
            if (atLeastOneRelicExist) break;
         }
         if (atLeastOneRelicExist) break;
      }

      if (!atLeastOneRelicExist) break;
   }

   cout << totalCount << endl;

   return 0;
}