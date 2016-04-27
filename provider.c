/*
 * Filename:    provider.c
 * copyright:   2016 chenxin
 * Function:    provide two random ingredients to smokers
 */
#include "myipc.h"

int main(int argc,char* argv[])
{
    int i,r,pid;
    int* status;

    if(argv[1]!=NULL)
        rate = atoi(argv[1]);
    else
        rate = 1;

    srand(time(0));

    signal(SIGINT,(sighandler_t)sigcat);

    ipc_init();
    for(i=0;i<2;i++) {
        pid = fork();
        while(pid == 0) {
            r = rand()%3;
            switch(r) {
            case 0:
                P(TG_Psyn_sem);
                P(TG_Pmtx_sem);

                tobacco_ptr[*pputT_ptr] = 'T';
                glue_ptr[*pputG_ptr] = 'G';
                printf("%d 供应商提供：烟草T，胶水G\n",getpid());

                sleep(rate);
                
                V(TG_Pmtx_sem);
                V(TG_Csyn_sem);
                break;
            case 1:
                P(PG_Psyn_sem);
                P(PG_Pmtx_sem);

                paper_ptr[*pputP_ptr] = 'P';
                glue_ptr[*pputG_ptr] = 'G';
                printf("%d 供应商提供：纸张P，胶水G\n",getpid());

                sleep(rate);
                
                V(PG_Pmtx_sem);
                V(PG_Csyn_sem);
                break;
            case 2:
                P(TP_Psyn_sem);
                P(TP_Pmtx_sem);

                paper_ptr[*pputP_ptr] = 'P';
                tobacco_ptr[*pputT_ptr] = 'T';
                printf("%d 供应商提供：烟草T，纸张P\n",getpid());

                sleep(rate);
                
                V(TP_Pmtx_sem);
                V(TP_Csyn_sem);
                break;
            }
        }
    }
    wait(status);
    return EXIT_SUCCESS;
}
