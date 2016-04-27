/*
 * Filename:    myipc.h
 * copyright:   2016 chenxin
 * Function:    declaration of the function prototypes and global variables in IPC mechanism of smoker problem
 */
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/sem.h> 
#include <sys/msg.h> 
#include <signal.h>

#define BUFSZ 256

typedef void (*sighandler_t)(int);
void sigcat(void); //信号处理函数

int get_ipc_id(char *proc_file,key_t key);
char* set_shm(key_t shm_key,int shm_num,int shm_flag);
int set_msq(key_t msq_key,int msq_flag);
int set_sem(key_t sem_key,int sem_val,int sem_flag);

int P(int sem_id); //wait
int V(int sem_id); //signal

void ipc_init(void); 

typedef union semuns {
    int val;
} Sem_uns;

typedef struct msgbuf {
    long mtype;
    char mtext[1];
} Msg_buf;

//共享内存中的T容器有关变量
key_t tobacco_key;
int tobacco_num;
char* tobacco_ptr;

//共享内存中的P容器有关变量
key_t paper_key;
int paper_num;
char* paper_ptr;

//共享内存中的G容器有关变量
key_t glue_key;
int glue_num;
char* glue_ptr;

//放产品到T容器内的共享指针
key_t pputT_key;
int pputT_num;
int* pputT_ptr;

//从T容器内取产品的共享指针
key_t cgetT_key;
int cgetT_num;
int* cgetT_ptr;

//放产品到G容器内的共享指针
key_t pputG_key;
int pputG_num;
int* pputG_ptr;

//从G容器内取产品的共享指针
key_t cgetG_key;
int cgetG_num;
int* cgetG_ptr;

//放产品到P容器内的共享指针
key_t pputP_key;
int pputP_num;
int* pputP_ptr;

//从P容器内取产品的共享指针
key_t cgetP_key;
int cgetP_num;
int* cgetP_ptr;

//T&G容器中的生产者的同步信号量和互斥信号量
key_t TG_Psyn_key;
key_t TG_Pmtx_key;
int TG_Psyn_sem;
int TG_Pmtx_sem;

//P&G容器中的生产者的同步信号量和互斥信号量
key_t PG_Psyn_key;
key_t PG_Pmtx_key;
int PG_Psyn_sem;
int PG_Pmtx_sem;

//T&P容器中的生产者的同步信号量和互斥信号量
key_t TP_Psyn_key;
key_t TP_Pmtx_key;
int TP_Psyn_sem;
int TP_Pmtx_sem;

//T&G容器中的消费者的同步信号量和互斥信号量
key_t TG_Csyn_key;
key_t TG_Cmtx_key;
int TG_Csyn_sem;
int TG_Cmtx_sem;

//P&G容器中的消费者的同步信号量和互斥信号量
key_t PG_Csyn_key;
key_t PG_Cmtx_key;
int PG_Csyn_sem;
int PG_Cmtx_sem;

//T&P容器中的消费者的同步信号量和互斥信号量
key_t TP_Csyn_key;
key_t TP_Cmtx_key;
int TP_Csyn_sem;
int TP_Cmtx_sem;

int sem_val;
int sem_flg;
int shm_flg;
int rate;
