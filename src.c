#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

int bmh(char *str, int str_len, char *pattern, int pat_len) {
    int table[CHAR_MAX + 1];

    if (str_len < pat_len || pat_len <= 0 || !str || !pattern) {
        return -1;
    }

    for (register int i = 0; i < CHAR_MAX + 1; ++i) {
        table[i] = pat_len;
    }

    for (register int i = 1; i < pat_len; ++i) {
        if (table[(int) pattern[pat_len - i - 1]] != pat_len) {
            continue;
        }
        else {
            table[(int) pattern[pat_len - i - 1]] = i;
        }
    }
    for (register int i = 0; i < str_len; ++i) {
        int match = 0;
        for (register int j = pat_len - 1; j >= 0; --j) {
            if (str[i + j] != pattern[j] && !match) {
                i += table[(int) str[i + j]] - 1;
                break;
            }
            else if(str[i + j] != pattern[j] && match) {
                i += table[(int) pattern[pat_len - 1]] - 1;
                match = 0;
                break;
            }
            else {
                match = 1;
            }
        }
        if (match) {
            while (i != 0 && str[i - 1] != ' ') {
                i--;
            }
            return i;
        }
    }
    return -1;
}

int main() {
	char temp_ch;
	int fd1[2], fd2[2], temp_fork, cnt = 0;
	int read_size, write_size;

	if (pipe(fd1) == -1 || pipe(fd2) == -1) {
		printf("Can\'t create pipe");
		exit(-1);
	}

	temp_fork = fork();
	if (temp_fork == -1) {
		printf("Can\'t create child");
		exit(-1);
	}
	else if (temp_fork > 0) {
		close(fd1[0]);
		while (read(2, &temp_ch, sizeof(char)) == sizeof(char)) {
			write_size = write(fd1[1], &temp_ch, sizeof(char));
			if (write_size != sizeof(char)) {
				printf("Can\'t write char\n");
				exit(-1);
			}
			cnt++;
		}
		write_size = write(fd2[1], &cnt, sizeof(int));
		if (write_size != sizeof(int)) {
			printf("Can\'t write size of string\n");
			exit(-1);
		}
		cnt = read_size = write_size = 0;

		while (read(2, &temp_ch, sizeof(char)) == sizeof(char)) {
			write_size = write(fd1[1], &temp_ch, sizeof(char));
			if (write_size != sizeof(char)) {
				printf("Can\'t write char\n");
				exit(-1);
			}
			cnt++;
		}
		write_size = write(fd2[1], &cnt, sizeof(int));
		if (write_size != sizeof(int)) {
			printf("Can\'t write size of pattern\n");
			exit(-1);
		}
		cnt = read_size = 0;

		wait(NULL);

		read_size = read(fd2[0], &cnt, sizeof(int));
		if (read_size != sizeof(int)) {
			printf("Can\'t read result");
			exit(-1);
		}
		printf("%d\n", cnt);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
	}
	else {
		close(fd1[1]);
		int str_size, pat_size, result;

		read_size = read(fd2[0], &str_size, sizeof(int));
		if (read_size != sizeof(int)) {
			printf("Can\'t read size of string");
			exit(-1);
		}
		char *str = (char *) malloc(sizeof(char *) * str_size);
		read_size = read(fd1[0], str, sizeof(char) * str_size);
		if (read_size != sizeof(char) * str_size) {
			printf("Can\'t read string\n");
			exit(-1);
		}

		read_size = read(fd2[0], &pat_size, sizeof(int));
		if (read_size != sizeof(int)) {
			printf("Can\'t read size of pattern");
			exit(-1);
		}
		char *pattern = (char *) malloc(sizeof(char *) * pat_size);
		read_size = read(fd1[0], pattern, sizeof(char) * pat_size);
		if (read_size != sizeof(char) * pat_size) {
			printf("Can\'t read pattern\n");
			exit(-1);
		}

		result = bmh(str, str_size - 1, pattern, pat_size - 1);
		write_size = write(fd2[1], &result, sizeof(int));
		if (write_size != sizeof(int)) {
			printf("Can\'t write result\n");
			exit(-1);
		}
		close(fd1[0]);
		close(fd2[0]);
		close(fd2[1]);
	}

	return 0;
}
