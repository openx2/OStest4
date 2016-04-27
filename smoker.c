/*
 * Filename:    smoker.c
 * copyright:   2016 chenxin
 * Function:    create 3 subprocess and each has one of ingredients to make cigarette
 */
#include "myipc.h"

int main(int argc,char* argv[])
{
    int pid;
    int* status;

    if(argv[1]!=NULL)
        rate = atoi(argv[1]);
    else
        rate = 1;

    signal(SIGINT,(sighandler_t)sigcat);

    ipc_init();

    //创建拥有烟草的吸烟者子进程
    pid = fork();
    while(pid == 0) {
        P(PG_Csyn_sem);
        P(PG_Cmtx_sem);

        printf("%d 吸烟者拥有烟草T，取到纸张%c，胶水%c.开始吸烟...\n",
            getpid(),paper_ptr[*cgetP_ptr],glue_ptr[*cgetG_ptr]);

        sleep(rate);
        V(PG_Cmtx_sem);
        V(PG_Psyn_sem);
    }

    pid = fork();
    while(pid == 0) {
        P(TG_Csyn_sem);
        P(TG_Cmtx_sem);

        printf("%d 吸烟者拥有纸张P，取到烟草%c，胶水%c.开始吸烟...\n",
            getpid(),tobacco_ptr[*cgetT_ptr],glue_ptr[*cgetG_ptr]);

        sleep(rate);
        V(TG_Cmtx_sem);
        V(TG_Psyn_sem);
    }

    pid = fork();
    while(pid == 0) {
        P(TP_Csyn_sem);
        P(TP_Cmtx_sem);

        printf("%d 吸烟者拥有胶水G，取到烟草%c，纸张%c.开始吸烟...\n",
            getpid(),tobacco_ptr[*cgetT_ptr],paper_ptr[*cgetP_ptr]);

        sleep(rate);
        V(TP_Cmtx_sem);
        V(TP_Psyn_sem);
    }
    
    wait(status);
    return EXIT_SUCCESS;
}
