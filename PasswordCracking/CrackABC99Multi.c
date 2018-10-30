#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$H8s0k9/1RQ783G9gF69Xkn.MI.Dq5Ox0va/dFlkknNjO7trgekVOjTv1BKCb.nm3vqxmtO2mOplhmFkwZXecz0",
  "$6$KB$WksuNcTfYjZWjDC4Zt3ZAmQ38OrsWwHyDgf/grFJ2Sgg.qpOz56lMpBVfWYdQZa9Pksa2TJRVYVb3K.mbYx4Y1",
  "$6$KB$0ZqvOLHpRgU9vLhzavKkL37MCDESwi2NDoTptnw4jyAjQGVtizjiKaluE60l1k7b.7YzDFU3biOo7Cr2SnvzT1",
  "$6$KB$UwKD1iCsvhAryQWAH6o8C9B6dEtOUOhYCgBfwtvffD.Ycz83.8GZ/9dhfIyVodUtHRyUl8A8LRfCNSlx8Lb2O1"
};

void multiCore(){//method to create multiple cores
  pthread_t t1, t2;

  void *kernel_function_1();
  void *kernel_function_2();

  pthread_create(&t1, NULL, kernel_function_1, NULL);
  pthread_create(&t2, NULL, kernel_function_2, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
}

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void *kernel_function_1(){
  int x, y, z;
  char salt[7];
  char plain[7];
  char *enc;
  int count = 0;

  char *encrypted_passwords[] = {
    "$6$KB$H8s0k9/1RQ783G9gF69Xkn.MI.Dq5Ox0va/dFlkknNjO7trgekVOjTv1BKCb.nm3vqxmtO2mOplhmFkwZXecz0",
    "$6$KB$WksuNcTfYjZWjDC4Zt3ZAmQ38OrsWwHyDgf/grFJ2Sgg.qpOz56lMpBVfWYdQZa9Pksa2TJRVYVb3K.mbYx4Y1",
    "$6$KB$0ZqvOLHpRgU9vLhzavKkL37MCDESwi2NDoTptnw4jyAjQGVtizjiKaluE60l1k7b.7YzDFU3biOo7Cr2SnvzT1",
    "$6$KB$UwKD1iCsvhAryQWAH6o8C9B6dEtOUOhYCgBfwtvffD.Ycz83.8GZ/9dhfIyVodUtHRyUl8A8LRfCNSlx8Lb2O1"
  };

  int t; // new loop counter for the third initial

  substr(salt, encrypted_passwords[0], 0, 6);
  for(int i=0;i<n_passwords;i<i++) {
    for(x='A'; x<='M'; x++){
      for(y='A'; y<='M'; y++){
        for(t = 'A'; t <= 'M'; t++){ // new for loop for the third initial
          for(z=0; z<=99; z++){
            sprintf(plain, "%c%c%c%02d", x, y, t, z);
            enc = (char *) crypt(plain, salt);
            count++;
            if(strcmp(encrypted_passwords[i], enc) == 0){
              printf("#%-8d%s %s\n", count, plain, enc);
            } else {
              printf(" %-8d%s %s\n", count, plain, enc);
            }
          }
        }
      }
    }
  }

  printf("%d solutions explored\n", count);
}

void *kernel_function_2(){
  int x, y, z;
  char salt[7];
  char plain[7];
  char *enc;
  int count = 0;

  char *encrypted_passwords[] = {
    "$6$KB$H8s0k9/1RQ783G9gF69Xkn.MI.Dq5Ox0va/dFlkknNjO7trgekVOjTv1BKCb.nm3vqxmtO2mOplhmFkwZXecz0",
    "$6$KB$WksuNcTfYjZWjDC4Zt3ZAmQ38OrsWwHyDgf/grFJ2Sgg.qpOz56lMpBVfWYdQZa9Pksa2TJRVYVb3K.mbYx4Y1",
    "$6$KB$0ZqvOLHpRgU9vLhzavKkL37MCDESwi2NDoTptnw4jyAjQGVtizjiKaluE60l1k7b.7YzDFU3biOo7Cr2SnvzT1",
    "$6$KB$UwKD1iCsvhAryQWAH6o8C9B6dEtOUOhYCgBfwtvffD.Ycz83.8GZ/9dhfIyVodUtHRyUl8A8LRfCNSlx8Lb2O1"
  };

  int t; // new loop counter for the third initial

  substr(salt, encrypted_passwords[0], 0, 6);
  for(int i=0;i<n_passwords;i<i++) {
    for(x='N'; x<='Z'; x++){
      for(y='N'; y<='Z'; y++){
        for(t = 'N'; t <= 'Z'; t++){ // new for loop for the third initial
          for(z=0; z<=99; z++){
            sprintf(plain, "%c%c%c%02d", x, y, t, z);
            enc = (char *) crypt(plain, salt);
            count++;
            if(strcmp(encrypted_passwords[i], enc) == 0){
              printf("#%-8d%s %s\n", count, plain, enc);
            } else {
              printf(" %-8d%s %s\n", count, plain, enc);
            }
          }
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

  int i;

  multiCore();

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elasped was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed / 1.0e9));

  return 0;
}
