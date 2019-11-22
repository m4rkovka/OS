#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
//./имя_программы файл количество_строк количество_столбцов
int main(int argc, char **argv) {
	std::ofstream out(argv[1], std::ios::out);
	srand(time(0));
	int sign; // 0 - минус, 1 - плюс.
	double num;
	int m = atoi(argv[2]), n = atoi(argv[3]);
	out << m << " " << n << "\n";
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			sign = rand() % 2;
			num = rand() % 1000;
			out.width(5);
			out.setf(std::ios::left);
			out << (sign != 0 ? num : -num) << " "; 
		}
		out << "\n";
	}
	out << rand() % 11 << "\n";
	return 0;
}