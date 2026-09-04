#include <stdio.h>
#include <time.h>

int main() {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        time_t time_s = 0;
        long time_n = 0, prev_time_n = 0, last_cycle = 0;
        int loops = 0;
        const long freq_ns = (long)(1e9 / 700);

        while (1) {
		clock_gettime(CLOCK_MONOTONIC, &end);
		time_s = end.tv_sec - start.tv_sec;
		time_n = end.tv_nsec - start.tv_nsec;

                if (last_cycle > freq_ns) {
                        loops++;
                        last_cycle = 0;
                }
                else {
                        if (prev_time_n > time_n)
                                last_cycle += (long)(1e9 - prev_time_n) + time_n;
                        else
                                last_cycle += time_n - prev_time_n;
                }

                prev_time_n = time_n;

                if (time_n >= 0 && time_n <= 100 && time_s == 1)
                        break;
        }

        printf("%d loops in %d seconds\n", loops, (int)time_s);
}
