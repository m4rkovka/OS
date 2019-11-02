#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

int main() {
	srand(time(NULL));
	int MAX, THREAD_MAX;
	std::cin >> MAX >> THREAD_MAX;

	std::ofstream out("test");
	out << MAX << "\n" << THREAD_MAX << "\n";
	for (int i = 0; i < MAX; i++) {
		int rand_cnt = rand() % 29;
		for (int j = 0; j < rand_cnt; j++) {	
			out << (char) ('a' + (rand() % 26));
		}
		out << "\n";
	}

	return 0;
}