/*
*   DKU Operating System Lab
*           Lab2 (Vehicle production Problem)
*           Student id : 
*           Student name : 
*
*   lab2_sync.c :
*       - lab2 main file.
*       - must contains Vehicle production Problem function's declations.
*
*/

#include <aio.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab2_sync_types.h"

pthread_mutex_t mutex;
sem_t semaphore;
sem_t sem;
sem_t em;

int total_car;

/*
 * you need to implement Vehicle production Problem. 
 */

void lab2_sync_usage(char *cmd) {
	printf("\n Usage for %s : \n",cmd);
    printf("    -c: Total number of vehicles produced, must be bigger than 0 ( e.g. 100 )\n");
    printf("    -q: RoundRobin Time Quantum, must be bigger than 0 ( e.g. 1, 4 ) \n");
}

void lab2_sync_example(char *cmd) {
	printf("\n Example : \n");
    printf("    #sudo %s -c=100 -q=1 \n", cmd);
    printf("    #sudo %s -c=10000 -q=4 \n", cmd);
}

void print_result() {

}

int buy_car_from_queue(CQ *q, int car_num) {
	Node *buy_node;

	//printf("%d\n", q->balance);

	if(q->balance == 0) {
		printf("No car in Rail\n");
		return 0;
	}
	buy_node = q->front;

	if (buy_node->car_num == car_num) {
		q->front = buy_node->next;
		q->balance = q->balance - 1;
		free(buy_node);
	}
	return 1;
}

void initCarRail(CQ *q) {
	q->front = q->rear = NULL;
	q->balance = 0;
}

void make_car_to_queue(CQ *q, int car_num) {
	Node *new_node = (Node *)malloc(sizeof(Node));
	new_node->car_num = car_num;
	new_node->next = NULL;

	//printf("pass\n");
	if(q->balance == 0) {
		q->front = new_node;
	}
	else {
		q->rear->next = new_node;
	}
	q->rear = new_node;
	q->balance = q->balance + 1;	
}

int run_buy(CQ *q, int car_num) {
	int res;
	//printf("%d\n", q->balance);
	//printf("run through\n");
	res = buy_car_from_queue(q, car_num);
	return res;
}

void *consumer_a(void *q) {
	const int car_num = 1;
	int	cnt_car = total_car/5;
	int res = 0;

	CQ *my_q = (CQ *)q;
	
	do{
		//pthread_mutex_lock(&mutex);
		//sem_wait(&sem);
		//sem_wait(&em);
		//printf("pass 2\n");
		if(my_q->balance != 0) {
			if (my_q->front->car_num == car_num) {
				//sem_wait(&semaphore);
				res = run_buy(my_q, car_num);
				//printf("car - %d\n", cnt_car);
				//sem_post(&semaphore);
			}
			cnt_car = cnt_car - res;
			printf("cnt_car = %d\n", cnt_car);
		}
		//pthread_mutex_unlock(&mutex);
		//sem_post(&sem);
		//sem_post(&semaphore);
	}while(cnt_car > 0);

}

void *consumer_b(void *q) {
	const int car_num = 1;
	int cnt_car = total_car/5;
	int res;

	CQ *my_q = (CQ *)q;
	
	do{
		//sem_wait(&semaphore);
		//sem_wait(&sem);
		if(my_q->balance != 0) { 
            if (my_q->front->car_num == car_num) {
				//pthread_mutex_lock(&mutex);
                res = run_buy(my_q, car_num);
				//pthread_mutex_unlock(&mutex);
            }
			cnt_car = cnt_car - res;
		}
		//sem_post(&semaphore);
		//sem_post(&sem);
    }while(cnt_car > 0);

}

void *consumer_c(void *q) {
	const int car_num = 1;
	int cnt_car = total_car/5;
	int res;

	CQ *my_q = (CQ *)q;
	
	do{
		if(my_q->balance != 0) {
			if(my_q->front->car_num == car_num) {
				//pthread_mutex_lock(&mutex);
				res = run_buy(my_q, car_num);
				//pthread_mutex_unlock(&mutex);
			}
			cnt_car = cnt_car - res;
		}
	}while(cnt_car > 0);

}

void *consumer_d(void *q) {
	const int car_num = 1;
	int cnt_car = total_car/5;
    int res;

    CQ *my_q = (CQ *)q;

	do{
        if(my_q->balance != 0) {
            if(my_q->front->car_num == car_num) {
				//pthread_mutex_lock(&mutex);
                res = run_buy(my_q, car_num);
				//pthread_mutex_unlock(&mutex); 
			}   
			cnt_car = cnt_car - res;
		}
   }while(cnt_car > 0);

}

void *consumer_e(void *q) {
	const int car_num = 1;
	int cnt_car = total_car/5;
    int res;

    CQ *my_q = (CQ *)q;

	do{
        if(my_q->balance != 0) {
            if(my_q->front->car_num == car_num) {
				//pthread_mutex_lock(&mutex);
                res = run_buy(my_q, car_num);
				//pthread_mutex_unlock(&mutex);
            }      
			cnt_car = cnt_car - res;
		}
    }while(cnt_car > 0);

}

void *car_producer(void *q) {
	int rnd_num;

	CQ *my_q = (CQ *)q;

	//printf("%d\n", my_q->balance);

	//srand(time(NULL));

	for(int i = 0; i < total_car; i++) {
		//rnd_num = rand()%5;
		//if(i%5 == 0) rnd_num=1;
		rnd_num=1;
		//printf("pass\n");
		//else if(i%5 == 1) rnd_num = 2;
		//else if(i%5 == 2) rnd_num = 3;
		//else if(i%5 == 3) rnd_num = 4;
		//else rnd_num = 5;
		//pthread_mutex_lock(&mutex);
		//sem_wait(&semaphore);
		//sem_wait(&em);
		make_car_to_queue(my_q, rnd_num);
		//printf("pass3\n");
		//sem_post(&sem);
		//sem_post(&sem);
		//pthread_mutex_unlock(&mutex);
	}
}

int main(int argc, char* argv[]) {
	char op;
	int n; char junk;
	int time_quantum = 0;
	pthread_t pro_t;
	pthread_t con_t[5];
	total_car = 0;

	pthread_mutex_init(&mutex, NULL);
	sem_init(&semaphore, 0, 0);
	sem_init(&sem, 0, 1);

	if (argc <= 1) {
		lab2_sync_usage(argv[0]);
		lab2_sync_example(argv[0]);
		exit(0);
	}

	for (int i = 1; i < argc; i++) {
		if (sscanf(argv[i], "-c=%d%c", &n, &junk) == 1) {
			total_car = n;
		}
		else if (sscanf(argv[i], "-q=%d%c", &n, &junk) == 1) {
			time_quantum = n;
		}
		else {
			lab2_sync_usage(argv[0]);
			lab2_sync_example(argv[0]);
			exit(0);
		}
	}

	sem_init(&em, 0, total_car);

	CQ n_q;
	
	initCarRail(&n_q);

	pthread_create(&pro_t, NULL, car_producer, (void *)&n_q);
	pthread_create(&con_t[0], NULL, consumer_a, (void *)&n_q);
	pthread_create(&con_t[1], NULL, consumer_b, (void *)&n_q);
	pthread_create(&con_t[2], NULL, consumer_c, (void *)&n_q);
	pthread_create(&con_t[3], NULL, consumer_d, (void *)&n_q);
	//pthread_create(&con_t[4], NULL, consumer_e, (void *)&n_q);

	pthread_join(pro_t, NULL);
	pthread_join(con_t[0], NULL);
	pthread_join(con_t[1], NULL);
	pthread_join(con_t[2], NULL);
	pthread_join(con_t[3], NULL);
	//pthread_join(con_t[4], NULL);

	pthread_mutex_destroy(&mutex);
	sem_destroy(&semaphore);
	sem_destroy(&sem);
	sem_destroy(&em);

	printf("%d\n", n_q.balance);
	

	/*
	   * You need to implement Vehicle production Problem 
	   * If you want to use other header file, it's ok!
	   * If you have any question for this task, please contact me (ghwls03s@gmail.com)
	   * You may be need to use pthread_create function
	*/

	
	return 0;
}
