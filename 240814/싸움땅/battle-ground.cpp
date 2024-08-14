// 5:28 ~ 6:16, 8:20 ~ 9:05, 9:35 ~ 


// 인기 게임인 싸움땅은 다음과 같은 방식으로 진행됩니다. 
// 게임은 n * n 크기의 격자에서 진행되며, 각각의 격자에는 무기들이 있을 수 있습니다. 
// 
// 초기에는 '무기들이 없는' 빈 격자에 플레이어들이 위치하며 각 플레이어는 초기 능력치를 가집니다. 
// 각 플레이어의 초기 능력치는 모두 다릅니다. 게임은 다음과 같은 방식으로 진행됩니다.

// 하나의 라운드는 다음의 과정에 걸쳐 진행됩니다.
// 
// 1 - 1. '첫 번째 플레이어부터 순차적으로' 본인이 향하고 있는 방향대로 '한 칸만큼 이동'합니다.
// 만약 해당 방향으로 나갈 때 '격자를 벗어나는 경우에는' '정반대 방향으로 방향을 바꾸어서 1만큼 이동'합니다.

// 2-1. 만약 '이동한 방향에 플레이어가 없다면' 해당 칸에 총이 있는지 확인합니다. 
// 총이 있는 경우, 해당 플레이어는 총을 획득합니다. 
// 플레이어가 이미 총을 가지고 있는 경우에는 놓여있는 총들과 플레이어가 가지고 있는 총 가운데 '공격력이 더 쎈 총을 획득'하고,' 나머지 총들은 해당 격자에 둡니다.'

// 2-2-1. 만약 이동한 방향에 플레이어가 있는 경우에는 두 플레이어가 싸우게 됩니다. 
// 해당 플레이어의 '초기 능력치 + 가지고 있는 총의 공격력의 합'을 비교하여 더 큰 플레이어가 이기게 됩니다. 
// 만일 이 수치가 '같은 경우'에는 플레이어의 '초기 능력치가 높은 플레이어가 승리'하게 됩니다. 
// 이긴 플레이어는 각 플레이어의 '초기 능력치 + 가지고 있는 총의 공격력의 합'의 '차이'만큼을 포인트로 획득하게 됩니다.

// 2 - 2 - 2. 진 플레이어는 '본인이 가지고 있는 총을 해당 격자에 내려놓고', 해당 플레이어가 원래 가지고 있던 방향대로 한 칸 이동합니다.
// 만약 이동하려는 칸에 다른 플레이어가 있거나, 격자 범위 밖인 경우에는 -> 오른쪽으로 90도씩 회전하여 '빈 칸이 보이는 순간' 이동합니다. <-- 빈칸 나올 때까지 시계방향 90도
// 만약 해당 칸에 총이 있다면, 해당 플레이어는 '가장 공격력이 높은 총을 획득'하고 나머지 총들은 해당 격자에 내려 놓습니다.

// 2-2-3. 이긴 플레이어는 승리한 칸에 떨어져 있는 총들과 원래 들고 있던 총 중 '가장 공격력이 높은 총을 획득'하고, 나머지 총들은 해당 격자에 내려 놓습니다.
// 
// 위 과정을 1번부터 n번 플레이어까지 순차적으로 한 번씩 진행하면 1 라운드가 끝나게 된다.

// k 라운드 동안 게임을 진행하면서 각 플레이어들이 획득한 포인트를 출력하는 프로그램을 작성해보세요.


//첫 번째 줄에 n, m, k가 공백을 사이에 두고 주어집니다.n은 격자의 크기, m은 플레이어의 수, k는 라운드의 수를 의미합니다.
//
//이후 n개의 줄에 걸쳐 격자에 있는 총의 정보가 주어집니다.각 줄에는 각각의 행에 해당하는 n개의 수가 공백을 사이에 두고 주어집니다.
// 숫자 0은 빈 칸, 0보다 큰 값은 총의 공격력을 의미합니다.
//
//이후 m개의 줄에 걸쳐 플레이어들의 정보 x, y, d, s가 공백을 사이에 두고 주어집니다. 
// (x, y)는 플레이어의 위치, d는 방향, s는 플레이어의 초기 능력치를 의미하고 각 플레이어의 초기 능력치는 모두 다릅니다.
// 방향 d는 0부터 3까지 순서대로 ↑, →, ↓, ←을 의미합니다.
//
//각 플레이어의 위치는 겹쳐져 주어지지 않으며, 플레이어의 초기 위치에는 총이 존재하지 않습니다

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Player
{
	int r, c, d, s;
	int g = 0;
};

int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

int n, m, k;
vector<vector<int>> pMap(21, vector<int>(21, 0));
vector< Player> p(31);
vector<vector<priority_queue<int, vector<int>>>> map(21, vector<priority_queue<int, vector<int>>>(21, priority_queue<int, vector<int>>()));
vector<int> score(31, 0);

bool isOutOfBound(int r, int c) {
	return r < 1 || n < r || c < 1 || n < c;
}

void changeGun(int i, int nr, int nc) {
	if (p[i].g > 0) map[nr][nc].push(p[i].g);

	if (map[nr][nc].size() > 0) {
		p[i].g = map[nr][nc].top();
		map[nr][nc].pop();
	}

	pMap[nr][nc] = i;
	p[i].r = nr;
	p[i].c = nc;
}

void loseMove(int i) {
	int r = p[i].r;
	int c = p[i].c;
	int d = p[i].d;

	// 이동하기 전에 총부터 내려놓고 출발한다.
	map[r][c].push(p[i].g);
	p[i].g = 0;

	int nr = r + dr[d];
	int nc = c + dc[d];

	if (isOutOfBound(nr,nc) || pMap[nr][nc] > 0)
	{
		for (int n = 0; n < 3; n++)
		{
			p[i].d = d = (d + 1) % 4;
			nr = r + dr[d];
			nc = c + dc[d];

			if (!isOutOfBound(nr,nc) && pMap[nr][nc] == 0)
			{
				changeGun(i, nr, nc);
				break;
			}
		}
	}
	else changeGun(i, nr, nc);

	return;
}

int main() {
    cin >> n >> m >> k;
	for (int r = 1; r <= n; r++)
	{
		for (int c = 1; c <= n; c++)
		{
			int v;
			cin >> v;
			map[r][c].push(v);
		}
	}

	for (int i = 1; i <= m; i++)
	{
		cin >> p[i].r >> p[i].c >> p[i].d >> p[i].s;
	}

	for (int l = 0; l < k; l++)
	{
		for (int i = 1; i <= m; i++)
		{
			int r = p[i].r;
			int c = p[i].c;
			int d = p[i].d;
			
			int nr = r + dr[d];
			int nc = c + dc[d];

			if (isOutOfBound(nr,nc))
			{
				p[i].d = d = (d + 2) % 4;
				nr = r + dr[d];
				nc = c + dc[d];
			}

			pMap[r][c] = 0; // <-- 이거 잘 캐치했다. 순서상, 일단 현재 자리부터 지워주고 진행해야 하는 경우임.
			p[i].r = nr;
			p[i].c = nc;

			if (0 < pMap[nr][nc])
			{
				int n = pMap[nr][nc];
				if (p[i].s + p[i].g > p[n].s + p[n].g) {
					score[i] += (p[i].s + p[i].g) - (p[n].s + p[n].g);
					loseMove(n);
					changeGun(i, nr, nc);
				}
				else if(p[i].s + p[i].g == p[n].s + p[n].g)
				{
					if (p[i].s > p[n].s) {
						score[i] += (p[i].s + p[i].g) - (p[n].s + p[n].g);
						loseMove(n);
						changeGun(i, nr, nc);
					}
					else
					{
						score[n] += (p[n].s + p[n].g) - (p[i].s + p[i].g);
						loseMove(i);
						changeGun(n, nr, nc);
					}
				}
				else
				{
					score[n] += (p[n].s + p[n].g) - (p[i].s + p[i].g);
					loseMove(i);
					changeGun(n, nr, nc);
				}
			}
			else changeGun(i, nr, nc);
		}
	}

	for (int i = 1; i <= m; i++)
	{
		cout << score[i] << " ";
	}
	cout << '\n';

    return 0;
}