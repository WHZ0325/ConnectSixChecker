#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <vector>
#include <tuple>
#include <algorithm>
typedef std::tuple<int, int, int, int> quad;

char cmd[256];
inline void runCommand(const char s[], ...) {
	va_list arg;
	va_start(arg, s);
	vsnprintf(cmd, 255, s, arg);
	system(cmd);
	va_end(arg);
}

std::vector<quad> data; int map[20][20], cnt = 0;
inline void place(int x, int y, int p) {
	assert(0 <= x && x < 15);
	assert(0 <= y && y < 15);
	assert(0 <= p && p < 2);
	assert(!map[x][y]);
	map[x][y] = p + 1; ++cnt;
}
const char playerIcon[] = " @O";
inline void print() {
	putchar(' '); putchar(' ');
	for(int i = 0; i < 15; ++i) printf(" %02d", i + 1); putchar('\n');
	for(int i = 0; i < 15; ++i) {
		printf("%02d", i + 1);
		for(int j = 0; j < 15; ++j) {
			putchar(' ');
			putchar(playerIcon[map[i][j]]);
			putchar(' ');
		}
		putchar('\n');
	}
}
inline int check() {
	if(cnt < 6) return -1;
	for(int i = 0; i < 15; ++i) {
		for(int j = 0; j < 15; ++j) {
			if(!map[i][j]) continue;
			if(i + 5 < 15) {
				int color = map[i][j]; bool same = true;
				for(int k = i + 1; k <= i + 5; ++k) {
					if(map[k][j] ^ color) {
						same = false;
						break;
					}
				}
				if(same) return color;
			}
			if(j + 5 < 15) {
				int color = map[i][j]; bool same = true;
				for(int k = j + 1; k <= j + 5; ++k) {
					if(map[i][k] ^ color) {
						same = false;
						break;
					}
				}
				if(same) return color;
			}
			if(i + 5 < 15 && j + 5 < 15) {
				int color = map[i][j]; bool same = true;
				for(int k = 1; k <= 5; ++k) {
					if(map[i + k][j + k] ^ color) {
						same = false;
						break;
					}
				}
				if(same) return color;
			}
			if(i + 5 < 15 && j - 5 >= 0) {
				int color = map[i][j]; bool same = true;
				for(int k = 1; k <= 5; ++k) {
					if(map[i + k][j - k] ^ color) {
						same = false;
						break;
					}
				}
				if(same) return color;
			}
		}
	}
	return (cnt ^ 225) ? -1 : 0;
}

inline bool outOfRange(int x, int y) {
	return x < 0 || 15 <= x || y < 0 || 15 <= y;
}

int rnd = 0; bool type[2]; char *player[2];
inline void writeToPlayer(int p) {
	FILE *fp = fopen(".tmp.txt", "w");
	puts("Write to program: ");
	fprintf(fp, "%d\n", (rnd + 1) >> 1);
	printf("%d\n", (rnd + 1) >> 1);
	for(int i = 0, end = data.size(); i < end; ++i) {
		if(!i && p) continue;
		auto &d = data[i];
		fprintf(fp, "%d %d %d %d\n", std::get<0>(d), std::get<1>(d), std::get<2>(d), std::get<3>(d));
		printf("%d %d %d %d\n", std::get<0>(d), std::get<1>(d), std::get<2>(d), std::get<3>(d));
	}
	fclose(fp);

	FILE *sh = fopen(".run.sh", "w");
	fprintf(sh, "./%s < .tmp.txt > .out.txt &\n", player[p]);
	fputs("sleep 2\n", sh);
	fprintf(sh, "if [ \"$(ps -ef | grep ./%s | wc -l)\" != \"1\" ]\n", player[p]);
	fputs("then\n", sh);
	fprintf(sh, "\tpkill -9 %s\n", player[p]);
	fputs("fi\n", sh);
	fclose(sh);
	system("chmod u+x ./.run.sh");
	system("./.run.sh > /dev/null 2>&1");
}
inline bool readFromPlayer(int p) {
	writeToPlayer(p);

	FILE *fp = fopen(".out.txt", "r"); int x0, y0, x1, y1;
	if(fscanf(fp, "%d %d %d %d", &x0, &y0, &x1, &y1) ^ 4) {
		fclose(fp);
		return false;
	}
	else fclose(fp);
	if(outOfRange(x0, y0) || map[x0][y0] || outOfRange(x1, y1) || map[x1][y1]) return false;
	place(x0, y0, p); place(x1, y1, p);
	data.emplace_back(std::make_tuple(x0, y0, x1, y1));
	printf("Round %d: [Player %d]:\n", rnd, p + 1);
	return true;
}
inline bool readFromFirstPlayer() {
	writeToPlayer(0);

	FILE *fp = fopen(".out.txt", "r"); int x0, y0, x1, y1;
	if(fscanf(fp, "%d %d %d %d", &x0, &y0, &x1, &y1) ^ 4) {
		fclose(fp);
		return false;
	}
	else fclose(fp);
	if(outOfRange(x0, y0) || map[x0][y0] || (~x1) || (~y1)) return false;
	place(x0, y0, 0);
	data.emplace_back(std::make_tuple(x0, y0, x1, y1));
	puts("Round 1: [Player 1]:");
	return true;
}
inline void readFromUser(int user) {
	int x0, y0, x1, y1;
	do {
		printf("Round %d: [User %d]:\n", rnd, user + 1);
		scanf("%d %d %d %d", &x0, &y0, &x1, &y1);
		--x0; --y0; --x1; --y1;
	} while(outOfRange(x0, y0) || map[x0][y0] || outOfRange(x1, y1) || map[x1][y1]);
	place(x0, y0, user); place(x1, y1, user);
	data.emplace_back(std::make_tuple(x0, y0, x1, y1));
}
inline void readFromFirstUser() {
	++rnd;
	int x, y;
	do {
		puts("Round 1: [User 1]:");
		scanf("%d %d", &x, &y);
		--x; --y;
	} while(outOfRange(x, y) || map[x][y]);
	place(x, y, 0); data.emplace_back(std::make_tuple(x, y, -1, -1));
}

void beforeExit() {
	// puts("Before Exit.");
	for(int p = 0; p < 2; ++p) if(!type[p]) {
		FILE *sh = fopen(".run.sh", "w");
		fprintf(sh, "if [ \"$(ps -ef | grep ./%s | wc -l)\" != \"1\" ]\n", player[p]);
		fputs("then\n", sh);
		fprintf(sh, "\tpkill -9 %s\n", player[p]);
		fputs("fi\n", sh);
		fclose(sh);
		system("chmod u+x ./.run.sh");
		system("./.run.sh > /dev/null 2>&1");
	}
}

int main(int argc, char* argv[]) {
	atexit(beforeExit);
	if(argc == 3) {
		for(int i = 0; i < 2; ++i) {
			player[i] = argv[i + 1];
			type[i] = (strcmp(player[i], "user") == 0);
			if(!type[i]) {
				runCommand("g++ -std=c++11 %s.cpp -o %s", player[i], player[i]);
			}
		}
	}
	else type[0] = type[1] = true;
	int res; data.emplace_back(std::make_tuple(-1, -1, -1, -1));
	while((res = check()) == -1) {
		if(!rnd) {
			if(type[rnd & 1]) {
				readFromFirstUser();
			}
			else {
				++rnd;
				if(!readFromFirstPlayer()) {
					puts("Error: Reading From Player 1.");
					break;
				}
			}
		}
		else {
			if(type[rnd & 1]) {
				readFromUser((rnd++) & 1);
			}
			else {
				if(!readFromPlayer((rnd++) & 1)) {
					res = (rnd & 1) + 1;
					printf("Error: Reading From Player %d.\n", 3 - res);
					break;
				}
			}
		}
		print();
	}
	if(!res) {
		puts("The game has drawn.");
		return 0;
	}
	printf("The winner is %s %d.\n", type[res - 1] ? "User" : "Player", res);
	return res;
}
