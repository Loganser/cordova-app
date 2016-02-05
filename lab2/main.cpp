#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <sys/wait.h>

 #define RECEIVE_BUFFER_SIZE 20

int f(int x) {
	return 2*x;
}

int g(int x) {
	return x+sqrt(x);
}

void helper(int (*g)(int), char letter, int inpipe[2], int outpipe[2]) {
	char input[RECEIVE_BUFFER_SIZE], result_buf[RECEIVE_BUFFER_SIZE];
	int arg, result;
	close(inpipe[1]);close(outpipe[0]);
	read(inpipe[0], input, RECEIVE_BUFFER_SIZE);
	arg = atoi(input);
	result = g(arg);
	//sprintf(result_buf, "%d\n", result);
	write(outpipe[1], (const void*)result_buf, strlen(result_buf));
}

int main() {
	int cnt1, cnt2;
	char fresult[100], gresult[100];
	pid_t pidf, pidg;
	int fpipein[2], fpipeout[2], gpipein[2], gpipeout[2];
	char x[10], y[10];
	int xx, yy;
	setbuf(stdout, NULL);
	printf("Enter x:\n");
	scanf("%s", x);
	printf("Enter y:\n");
	scanf("%s", y);
	if (pipe(fpipein) || pipe(fpipeout) || pipe(gpipein) ||	pipe(gpipeout)) {
		printf("Failed\n");
		exit(-1);
	}
	pidf = fork();
	if (pidf == 0) {
            printf("f ended\n");
		pidg = fork();
		if (pidg == 0) {
                    printf("g ended\n");
			// parent
			close(fpipein[0]);close(gpipein[0]);
			close(fpipeout[1]);close(gpipeout[1]);
			write(fpipein[1], (const void *)x, strlen(x));
			write(gpipein[1], (const void *)y, strlen(y));

			cnt1 = read(fpipeout[0], fresult, RECEIVE_BUFFER_SIZE);
			cnt2 = read(gpipeout[0], gresult, RECEIVE_BUFFER_SIZE);
			fresult[cnt1] = 0;
			gresult[cnt2] = 0;
			xx = atoi(fresult);
			yy = atoi(gresult);
			printf("\nxx=%d\nyy=%d\nresult=%d\n", xx, yy, xx * yy);
			wait(NULL);
		} else {
			// child 2
                        printf("g here\n");
			helper(g, 'G', gpipein, gpipeout);
		}
	} else {
            printf("f here\n");
		// child 1
		helper(f, 'F', fpipein, fpipeout);
	}
	return 0;
}