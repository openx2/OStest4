/*
 * Filename:    myipc.c
 * copyright:   2016 chenxin
 * Function:    a series of functions which build the IPC mechanism of smoker problem
 */
#include "myipc.h"

int get_ipc_id(char* proc_file,key_t key)
{
    FILE *pf;
    int i,j;
    char line[BUFSZ],colum[BUFSZ];
    if((pf = fopen(proc_file,"r")) == NULL) {
        perror("Proc file not open");
        exit(EXIT_FAILURE);
    }
    fgets(line,BUFSZ,pf);
    while(!feof(pf)) {
        i = j = 0;
        fgets(line,BUFSZ,pf);
        while(line[i] == ' ') i++;
        while(line[i] != ' ') colum[j++] = line[i++];
        colum[j] = '\0';
        if(atoi(colum)!=key) continue;
        j = 0;
        while(line[i] == ' ') i++;
        while(line[i] != ' ') colum[j++] = line[i++];
        colum[j] = '\0';
        i = atoi(colum);
        fclose(pf);
        return i;
    }
    fclose(pf);
    return -1;
}

int P(int sem_id)
{
    struct sembuf buf;
    buf.sem_op = -1;
    buf.sem_num = 0;
    buf.sem_flg = SEM_UNDO;
    if((semop(sem_id,&buf,1)) < 0) {
        perror("down error");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int V(int sem_id)
{
    struct sembuf buf;
    buf.sem_op = 1;
    buf.sem_num = 0;
    buf.sem_flg = SEM_UNDO;
    if((semop(sem_id,&buf,1)) < 0) {
        perror("up error");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int set_sem(key_t sem_key,int sem_val,int sem_flg)
{
    int sem_id;
    Sem_uns sem_arg;
    if((sem_id = get_ipc_id("/proc/sysvipc/sem",sem_key)) < 0) {
        if((sem_id = semget(sem_key,1,sem_flg)) < 0) {
            perror("semaphore create error");
            exit(EXIT_FAILURE);
        }
        sem_arg.val = sem_val;
        if(semctl(sem_id,0,SETVAL,sem_arg) < 0) {
            perror("semaphore set error");
            exit(EXIT_FAILURE);
        }
    }
    return sem_id;
}

char* set_shm(key_t shm_key,int shm_num,int shm_flg)
{
    int i,shm_id;
    char* shm_buf;
    if((shm_id = get_ipc_id("/proc/sysvipc/shm",shm_key)) < 0) {
        if((shm_id = shmget(shm_key,shm_num,shm_flg)) < 0) {
            perror("shareMemory set error");
            exit(EXIT_FAILURE);
        }
        if((shm_buf = (char *)shmat(shm_id,0,0)) < (char *)0) {
            perror("get shareMemory error");
            exit(EXIT_FAILURE);
        }
        for(i = 0; i<shm_num; i++)
            shm_buf[i] = 0;
    }
    else if((shm_buf = (char *)shmat(shm_id,0,0)) < (char *)0) {
        perror("get shareMemory error");
        exit(EXIT_FAILURE);
    }
    return shm_buf;
}

int set_msq(key_t msq_key,int msq_flg)
{
    int msq_id;

    if((msq_id = get_ipc_id("/proc/sysvipc/msg",msq_key)) < 0) {
        if((msq_id = msgget(msq_key,msq_flg)) < 0) {
            perror("messageQueue set error");
            exit(EXIT_FAILURE);
        }
    }
    
    return msq_id;
}

void sigcat()
{
    //释放供应商同步信号量
    semctl(TG_Psyn_sem,0,IPC_RMID);
    semctl(PG_Psyn_sem,0,IPC_RMID);
    semctl(TP_Psyn_sem,0,IPC_RMID);
    //释放供应商互斥信号量
    semctl(TG_Pmtx_sem,0,IPC_RMID);
    semctl(PG_Pmtx_sem,0,IPC_RMID);
    semctl(TP_Pmtx_sem,0,IPC_RMID);
    //释放吸烟者同步信号量
    semctl(TG_Csyn_sem,0,IPC_RMID);
    semctl(PG_Csyn_sem,0,IPC_RMID);
    semctl(TP_Csyn_sem,0,IPC_RMID);
    //释放吸烟者互斥信号量
    semctl(TG_Cmtx_sem,0,IPC_RMID);
    semctl(PG_Cmtx_sem,0,IPC_RMID);
    semctl(TP_Cmtx_sem,0,IPC_RMID);


    //供应商释放指向放产品位置的指针
    shmdt(pputT_ptr);
    shmdt(pputP_ptr);
    shmdt(pputG_ptr);
    
    //吸烟者释放指向拿产品位置的指针
    shmdt(cgetT_ptr);
    shmdt(cgetP_ptr);
    shmdt(cgetG_ptr);
    
    //释放指向缓冲区的指针
    shmdt(tobacco_ptr);
    shmdt(paper_ptr);
    shmdt(glue_ptr);

    exit(0);
}

void ipc_init()
{
    tobacco_key = 101;
    tobacco_num = 1;
    paper_key = 102;
    paper_num = 1;
    glue_key = 103;
    glue_num = 1;
    pputT_key = 104;
    pputT_num = 1;
    pputG_key = 105;
    pputG_num = 1;
    pputP_key = 106;
    pputP_num = 1;
    cgetT_key = 107;
    cgetT_num = 1;
    cgetG_key = 108;
    cgetG_num = 1;
    cgetP_key = 109;
    cgetP_num = 1;
    shm_flg = IPC_CREAT | 0664;

    tobacco_ptr = (char *)set_shm(tobacco_key,tobacco_num,shm_flg);
    paper_ptr = (char *)set_shm(paper_key,paper_num,shm_flg);
    glue_ptr = (char *)set_shm(glue_key,glue_num,shm_flg);
    pputT_ptr = (int *)set_shm(pputT_key,pputT_num,shm_flg);
    pputP_ptr = (int *)set_shm(pputP_key,pputP_num,shm_flg);
    pputG_ptr = (int *)set_shm(pputG_key,pputG_num,shm_flg);
    cgetT_ptr = (int *)set_shm(cgetT_key,cgetT_num,shm_flg);
    cgetP_ptr = (int *)set_shm(cgetP_key,cgetP_num,shm_flg);
    cgetG_ptr = (int *)set_shm(cgetG_key,cgetG_num,shm_flg);

    TP_Psyn_key = 201;
    PG_Psyn_key = 202;
    TG_Psyn_key = 203;
    TP_Pmtx_key = 204;
    PG_Pmtx_key = 205;
    TG_Pmtx_key = 206;
    TP_Csyn_key = 301;
    PG_Csyn_key = 302;
    TG_Csyn_key = 303;
    TP_Cmtx_key = 304;
    TP_Cmtx_key = 305;
    TP_Cmtx_key = 306;
    sem_flg = IPC_CREAT | 0664;

    sem_val = tobacco_num;
    PG_Psyn_sem = set_sem(PG_Psyn_key,sem_val,sem_flg);

    sem_val = paper_num;
    TG_Psyn_sem = set_sem(TG_Psyn_key,sem_val,sem_flg);

    sem_val = glue_num;
    TP_Psyn_sem = set_sem(TP_Psyn_key,sem_val,sem_flg);

    sem_val = 0;
    TP_Csyn_sem = set_sem(TP_Csyn_key,sem_val,sem_flg);
    PG_Csyn_sem = set_sem(PG_Csyn_key,sem_val,sem_flg);
    TG_Csyn_sem = set_sem(TG_Csyn_key,sem_val,sem_flg);

    sem_val = 1;
    TP_Pmtx_sem = set_sem(TP_Pmtx_key,sem_val,sem_flg);
    PG_Pmtx_sem = set_sem(PG_Pmtx_key,sem_val,sem_flg);
    TG_Pmtx_sem = set_sem(TG_Pmtx_key,sem_val,sem_flg);

    sem_val = 1;
    TP_Cmtx_sem = set_sem(TP_Cmtx_key,sem_val,sem_flg);
    PG_Cmtx_sem = set_sem(PG_Cmtx_key,sem_val,sem_flg);
    TG_Cmtx_sem = set_sem(TG_Cmtx_key,sem_val,sem_flg);
}
