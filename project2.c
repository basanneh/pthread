#include <pthread.h>

#include <stdlib.h>

#include <stdio.h>

#include <string.h>



#define MAX 10240

#define NUM_THREADS  10


int n1,n2;
int subTotal = 0;

char *s1,*s2;

FILE *fp;


static pthread_mutex_t mlock;





//read input file and generate string s1/s2 and length n1/n2

int readf(FILE *fp)

{

	if((fp=fopen("strings.txt", "r"))==NULL){

		printf("ERROR: can't open string.txt!\n");

		return 0;

	}

	s1=(char *)malloc(sizeof(char)*MAX);

	if(s1==NULL){

		printf("ERROR: Out of memory!\n");

		return -1;

	}

	s2=(char *)malloc(sizeof(char)*MAX);

	if(s1==NULL){

		printf("ERROR: Out of memory\n");

		return -1;

	}

	/*read s1 s2 from the file*/

	s1=fgets(s1, MAX, fp);

	s2=fgets(s2, MAX, fp);

	n1=strlen(s1);  /*length of s1*/

	n2=strlen(s2)-1; /*length of s2*/



	if(s1==NULL || s2==NULL || n1<n2)  /*when error exit*/

		return -1;

	return 0;

}



void num_substring(int t)
{

	int size = n1/NUM_THREADS;
	int part = t * (n1/NUM_THREADS);

	 for ( int i = part; i < part + size; i++) {
	           int len = strlen(s2) - 1;
		   int num = 0 ;
	    for (int j = 0; j < len; j++){
	         if (s1[i + j] != '\0' && s2[j] != '\0' && s1[i+j] == s2[j]){
                         num ++;
    		}
    }
       if (num == len){
		subTotal++;
	}
        }
	
       
}



void *calSubStringThread(void *threadid){

    long tid = (long)threadid;
    pthread_mutex_lock(&mlock);
	//Critical Section
       printf("This is thread %ld\n", tid);

       num_substring(tid);
	//End of Critical Section
    pthread_mutex_unlock(&mlock);


}



int main(int argc, char *argv[])

{
    pthread_t threads[NUM_THREADS];
	if(pthread_mutex_init(&mlock, NULL) !=0)
	{
		printf("mutex init failed\n");
		return 1;
	}
    int t, rc;

    int totalNum = 0;

	readf(fp);

	for(t=0; t<NUM_THREADS; t++){

        rc = pthread_create(&threads[t], NULL, calSubStringThread, (void *) (size_t)t);

        if (rc){

            printf("ERROR; return code from pthread_create() is %d\n", rc);

            exit(-1);

        }

}



    for(t=0; t<NUM_THREADS; t++){

        pthread_join(threads[t], NULL);

    }
	

	
 	printf("The number of substrings is: %d\n", subTotal);
	pthread_exit(NULL);
	pthread_mutex_destroy(&mlock);
	return 0;

}



