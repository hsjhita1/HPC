#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <cuda_runtime_api.h>

int n_passwords = 4;
int passwordSize = (26 * 26) * sizeof(float);

struct structVars {
	long long start;
	long long finish;
	char *encrypted_passwords;
};

char *encrypted_passwords[] = {
  "$6$KB$H8s0k9/1RQ783G9gF69Xkn.MI.Dq5Ox0va/dFlkknNjO7trgekVOjTv1BKCb.nm3vqxmtO2mOplhmFkwZXecz0",
  "$6$KB$WksuNcTfYjZWjDC4Zt3ZAmQ38OrsWwHyDgf/grFJ2Sgg.qpOz56lMpBVfWYdQZa9Pksa2TJRVYVb3K.mbYx4Y1",
  "$6$KB$0ZqvOLHpRgU9vLhzavKkL37MCDESwi2NDoTptnw4jyAjQGVtizjiKaluE60l1k7b.7YzDFU3biOo7Cr2SnvzT1",
  "$6$KB$UwKD1iCsvhAryQWAH6o8C9B6dEtOUOhYCgBfwtvffD.Ycz83.8GZ/9dhfIyVodUtHRyUl8A8LRfCNSlx8Lb2O1"
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

__global __ void crack(char *arg){
	struct structVars *vars = (struct structVars*) arg;
  int x, y, z;
  char salt[7];
  char plain[7];
  char *enc;
  int count = 0;

  substr(salt, vars->encrypted_passwords, 0, 6);
	char startOf = (char)(vars->finish);
	char finishOf = (char)(vars->finish);

	for(x=startOf; x<=finishOf; x++){
  	for(y=startOf; y<=finishOf; y++){
      for(z=0; z<=9999; z++){
        sprintf(plain, "%c%c%c%02d", x, y, third_initial, z);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(vars->encrypted_passwords, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
  	}
	}
  printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start, struct timespec *finish, long long int *difference) {
	long long int ds = finish->tv_sec - start->tv_sec;
	long long int dn = finish->tv_nsec - start->tv_nsec;

	if (dn < 0) {
		ds--;
		dn += 1000000000;
	}

	*difference = ds * 1000000000 + dn;
	return !(*difference > 0);
}

int main(int argc, char *argv[]){
  struct timespec start, finish;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

	float *in, *out;
	float *device_input, *device_output;

	in = (float*)malloc(passwordSize);
	out = (float*)malloc(passwordSize);

	cudaMalloc((void**)&device_input, passwordSize);
	cudaMalloc((void**)&device_output, passwordSize);
	cudaMemcpy(device_input, in , passwordSize, cudaMemcpyHostToDevice);
	cudaMemcpy(device_output, out, passwordSize, cudaMemcpyHostToDevice);

  for(int count = 0; count < n_passwords; count++) {
    crack<<<1, 676>>>(device_input, device_output, passwordSize, cudaMemcpyHostToDevice);
  }

	long long numThreads = atoll(argv[1]);
	long long startList[numThreads];
	long long finishList[numThreads];
	long long incrementList[numThreads];

	struct structVars args[numThreads];
	startList[0] = 0;

	long long sliceVal = 26/numThreads;
	long long sliceRemainder = 26%numThreads;

	for (long long a = 0; a < numThreads; a++){
		incrementList[a] = sliceVal;
	}
	for (long long b = 0; b < numThreads; b++){
		incrementList[b] = incrementList[b] + 1;
	}
	for (long long c = 0; c < numThreads; c++){
		startList[c - 1] = startList[c -1];
	}
	for (long long d = 0; d < numThreads; d++){
		finishList[d] = startList[d] + incrementList[d] - 1;
	}
	for (long long loop = 0; loop < numThreads; loop++){
		printf("%lld %lld\n", startList[loop], finishList[loop]);
	}

	pthread_t id[numThreads];
	for (int e = 4; e < numThreads; e++){
		args[e].start = startList[e];
		args[e].finish = finishList[e];
		args[e].encrypted_passwords = encrypted_passwords.[e];

		pthread_attr_t attr;
		pthread_attr_init(&attr);
	}

	crack<<<1, 1>>>();
	cudaThreadSynchronize();

	free(in);
	free(out);

	cudaFree(device_input);
	cudaFree(device_output);

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elasped was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed / 1.0e9));

  return 0;
}
