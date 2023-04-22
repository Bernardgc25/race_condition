#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//global variables

pthread_mutex_t myLock;


int twinprimecount = 0;
int counter = 0;
const int pstep = 100000;
const int four = 4;



//function prototype
int check_prime(int);
void *twinprimes(void *arg);
int alldigit(char *s);
int nondash(char *s);

int main(int argc, char *argv[])
{

	//thread object
	pthread_t thd[four];
	void *status;
	pthread_attr_t attr;
	pthread_mutex_init(&myLock, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int err;
	
	char *threadmode = argv[1];
	
    //check params
    if(argc > 3)
	{
    	printf("Error, invalid command line arguments.");
    	goto done;
    }
    //argc == 1
    else if (argc < 2){
        printf("Usage: /twinPrimes <-s|-p> <limitValue>");
        goto done;
    }
    //argc == 3
    else
    {
        if(argv[2] == NULL)
           {
           printf("Error, invalid command line arguments.");
           goto done;
           }

        else
        {
            //error, argv[1] == alphabet
        	if(nondash(argv[1]))
            {
        		//error, argv[2] == not digit
                if (alldigit(argv[2]))
                {
                printf("Error, invalid command line arguments.");
                goto done;
                }

                if(!alldigit(argv[2]))
                {
                printf("Error, invalid limit value");
                goto done;
                }
            }
        	//valid, argv[1]
            else
            {
            	if(strcmp(threadmode, "-s") == 0)
            	{
            		//check arg[2]
                    if(!alldigit(argv[2]))
                    {
                        printf("Error, invalid limit value");
                        goto done;
                    }

                    else
                    {
                        if(atoi(argv[2])< 100 )
                        {
                        	printf("Error, limit must be > 100.");
                        goto done;
                        }

                    	//execute SEQUENTIAL
                        else
                        {
                            printf("Twin Primes Program");
                            printf("\n");
                            printf("Please wait. Running...");
                            printf("\n");

                            int i = 0;
                            	if (pthread_mutex_init(&myLock, NULL) != 0)
                            	{
                            		printf("\n mutex init failed\n");
                            		return 1;
                            	}

                            		err = pthread_create(&(thd[i]), &attr , &twinprimes, argv[2]);
                            		printf("running thread %d....",i+1);
                            		printf("\n");

                             	if (err != 0)
                             	{
                             		printf("\ncan't create thread :[%s]", strerror(err));
                             	}
                             		pthread_attr_destroy(&attr);
                             		pthread_join(thd[i], &status);
                             		pthread_mutex_destroy(&myLock);

                             		goto result;
                        }
                    }
            	}

            	else if(strcmp(threadmode, "-t") == 0)
            	{
            		//check arg[2]
                    if(!alldigit(argv[2]))
                    {
                    printf("Error, invalid limit value");
                    goto done;
                    }

                    else
                    {
                        if( atoi(argv[2])< 100 )
                        {
                        printf("Error, limit must be > 100.");
                        goto done;
                        }

                        //execute PARALLEL
                        else
                        {
                            printf("Twin Primes Program");
                            printf("\n");
                            printf("Please wait. Running...");
                            printf("\n");

                           	int j;

                           		if (pthread_mutex_init(&myLock, NULL) != 0)
                                {
                           			printf("\n mutex init failed\n");
                                    return 1;
                                }

								for (j = 0; j < four; j++)
								{
									err = pthread_create(&(thd[j]), &attr, &twinprimes, argv[2]);
									printf("running thread %d....", j + 1);
									printf("\n");

									if (err != 0)
									{
										printf("\ncan't create thread :[%s]", strerror(err));
									}
								}

										pthread_attr_destroy(&attr);

                                    	for(int j = 0; j<four; j++)
                                    	{
                                    		pthread_join(thd[j], &status);
                                    	}
                                    		pthread_mutex_destroy(&myLock);
                                    		goto result;
                        }
                    }

            	}

            	else
            	{
            		   printf("Error, invalid command line arguments.");
            		                        goto done;
            	}

            }


        }

	}

    result:
		printf("\n");
		printf("Count of Twin Primes between 1 and %d", atoi(argv[2]) );
	    printf("\n");
	    printf("is %d",twinprimecount-1 );

	done:
		printf("\n");


pthread_exit(NULL);
//return 0;

}
//check if number is prime
int check_prime(int n)
{
    if(n == 1)
    {
    	return 0;
    }

    for(int i = 2; i < n; i++)
    {
        if(n % i == 0)
        {
        	//number is not prime
        	return 0;
        }
    }

    //number is prime
    return 1;
}


//check twinprime
void *twinprimes(void *arg)
{

	int limit = atoi(arg);    			//argv[2]
	int idx;
	int currIdx;

	//update counter at every 10000
	psteploop:
	  
		idx = counter;
	    pthread_mutex_lock(&myLock);
	    counter += pstep;
		pthread_mutex_unlock(&myLock);
	    currIdx = counter;
	   



	   //twinprimeloop
	while (idx <= limit)
	{

	   if(check_prime(idx) && check_prime(idx + 2))
	   {
	  	 //printf("{%d, %d}\n", i, i + 2);
	   	pthread_mutex_lock(&myLock);
	   	twinprimecount++;
	   	pthread_mutex_unlock(&myLock);
	   }

	   else
	   {
		   if (idx == currIdx)
		   {
		   //counter = idx;
			   goto psteploop;
	       }

	   }
	idx++;

	}

	pthread_exit((void *) 0);
    //return NULL;
}


//argv[2] all digits, return 1
int alldigit(char *s)
{
	int i=0;

	//printf("CHARACTER: ");
	while (*(s+i))
	{

		if (*(s+i) >= '0' && *(s+i) <= '9')
		{
			//printf("%c", *(s+i));
			i++;
		}
		
		else
		{
			return 0;
		}
		}

	return 1;

}

//argv[2] without dash
int nondash(char *s)
{
	int i=0;

		if (*(s+i) == '-')
		{
		return 0;
		}

		else
		{
			return 1;
		}


}

