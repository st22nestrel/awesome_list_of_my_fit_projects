/**
 * @name IOS proj2 - Faneuil hall problem
 * @author Timotej Ponek (xponek00)
 * @file proj2.c
 * @date 6.5.2020
*/

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <limits.h> 
#include <errno.h>
#include <stdlib.h>

#define MMAP(pointer) {(pointer) = mmap(NULL, sizeof(*pointer), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);}
#define UNMAP(pointer) {munmap((pointer), sizeof((pointer)));}
#define MSLEEP(sec) {((sec>0) ? (usleep( (rand() % sec) * 1000)) : (usleep(0)));}


//zdielane premenne
int *ne = NULL;//entered
int *nc = NULL;//
int *nb = NULL; //in building
int *judge = NULL;
int *action_n = NULL;
int *PC = NULL;//certifikatov celkovo
FILE *fp;

//semafory
sem_t *mutex = NULL;
sem_t *noJudge = NULL;
sem_t *checked = NULL;
sem_t *confirmed = NULL;
sem_t *wait_log = NULL;

/**
 * @brief mapuje premenne do sdilene pameti a otvara semafory
 * @return int 0 ak ok, 1 - chyba pri otvarani semaforu, 2 - chyba pri vytvarani sdilene promenne
 */
int mmap_start(){
    fp = fopen("proj2.out", "w");
    if(fp == NULL){
        fprintf(stderr, "Chyba pri otvarani suboru\n");
    }

    MMAP(ne);
    if (errno != 0) return 2;

    MMAP(nc);
    if (errno != 0) return 2;
    
    MMAP(nb);
    if (errno != 0) return 2;
    
    MMAP(judge);
    if (errno != 0) return 2;
    
    MMAP(action_n);
    if (errno != 0) return 2;
    
    MMAP(PC);
    if (errno != 0) return 2;
    
    if ((mutex = sem_open("/xponek00.ios.proj2.mutex", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) return 1;
    if ((noJudge = sem_open("/xponek00.ios.proj2.noJudge", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) return 1;
    if ((checked = sem_open("/xponek00.ios.proj2.checked", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) return 1;
    if ((confirmed = sem_open("/xponek00.ios.proj2.confirmed", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) return 1;
    if ((wait_log = sem_open("/xponek00.ios.proj2.wait_log", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) return 1;

    return 0;
}

/**
 * @brief rusi semafory a zdielane premenne
 */
void clean_map(){
    if(ne != NULL) UNMAP(ne);
    if(nc != NULL) UNMAP(nc);
    if(nb != NULL) UNMAP(nb);
    if(judge != NULL) UNMAP(judge);
    if(action_n != NULL) UNMAP(action_n);
    if(PC != NULL) UNMAP(PC);


        sem_close(mutex);
        sem_unlink("/xponek00.ios.proj2.mutex");
    
        sem_close(noJudge);
        sem_unlink("/xponek00.ios.proj2.noJudge");

        sem_close(checked);
        sem_unlink("/xponek00.ios.proj2.checked");
    
        sem_close(confirmed);
        sem_unlink("/xponek00.ios.proj2.confirmed");
    
        sem_close(wait_log);
        sem_unlink("/xponek00.ios.proj2.wait_log");


    if(fp != NULL)
        fclose(fp);
}

/**
 * @brief proces imigranta
 * @param IT sleeptime pred ziskanim certifikatu
 */
int imm_will(int IT, int i){
    int imm_id = i+1; //cislo imm

    while(1){
        sem_wait(wait_log);
        fprintf(fp, "%d \t: IMM %d\t: starts\n",(*action_n)++, imm_id);
        sem_post(wait_log);

        sem_wait(noJudge);
        
        sem_wait(wait_log);
        fprintf(fp, "%d \t: IMM %d\t: enters \t: %d \t: %d \t: %d\n",(*action_n)++, imm_id, ++(*ne), *nc, ++(*nb));
        sem_post(wait_log);

        sem_post(noJudge);
        sem_wait(mutex);

        sem_wait(wait_log);
        fprintf(fp, "%d \t: IMM %d\t: checks \t: %d \t: %d \t: %d\n",(*action_n)++, imm_id, *ne, ++(*nc), *nb);
        sem_post(wait_log);

        ++(*PC);

        if( *judge && *ne==*nc ){
            sem_post(checked);
        }
        else{
            sem_post(mutex);
        }

        sem_wait(confirmed);
        
        sem_wait(wait_log);
        fprintf(fp, "%d \t: IMM %d\t: wants certificate \t: %d \t: %d \t: %d\n",(*action_n)++, imm_id, *ne, *nc, *nb);
        sem_post(wait_log);

        MSLEEP(IT);

        sem_wait(wait_log);
        fprintf(fp, "%d \t: IMM %d\t: got certificate \t: %d \t: %d \t: %d\n",(*action_n)++, imm_id, *ne, *nc, *nb);
        sem_post(wait_log);
        sem_wait(noJudge);
        sem_wait(wait_log);
        fprintf(fp, "%d \t: IMM %d\t: leaves \t: %d \t: %d \t: %d\n",(*action_n)++, imm_id, *ne, *nc, --(*nb));
        sem_post(wait_log);
        sem_post(noJudge);
        break;
        }
        exit(0);
    }

/**
 * @brief proces generator imigrantov
 * @param PI pocet imigrantov
 * @param IG sleeptime pred vygenerovanim dalsieho imigranta
 * @param IT sleeptime pred ziskanim certifikatu
 * @return int 0 ak dobehne
 */
int imm_gen(int PI, int IG, int IT){
    int childrenPids[PI];
    int currentPid, i;
    for(i=0; i<PI; i++){
        MSLEEP(IG);
        switch(currentPid = fork()){
            case 0:
                imm_will(IT, i);
                exit (0);
                break;
            case -1:
                fprintf(stderr,"Error pri tvoreni procesu!\n");
                break;
            default:
                childrenPids[i] = currentPid;
                break;
        }
    }
    int returnStatus;
        for(int i = 0; i<PI; i++){
            waitpid(childrenPids[i], &returnStatus, 0);
            if (returnStatus == 1){
                fprintf(stderr, "Detsky proces skoncil s errorom.\n");   
            }
        }
       exit(0);

    }

/**
 * @brief proces sudca
 * @param JG sleeptime pred vstupom sudcu do budovy
 * @param PI pocet imigrantov
 * @param IT sleeptime vydavania certifikatov a pred odchodom sudcu z budovy
 */ 
int judge_gen(int JG, int PI, int JT){
    sleep(1);
    while(1){
    MSLEEP(JG);
    sem_wait(noJudge);
    sem_wait(mutex);

    sem_wait(wait_log);
    fprintf(fp, "%d \t: JUDGE\t: wants to enter \n",(*action_n)++);
    sem_post(wait_log);

    *judge = 1;
    sem_wait(wait_log);
    fprintf(fp, "%d \t: JUDGE\t: enters \t: %d \t: %d \t: %d\n",(*action_n)++, *ne, *nc, *nb);
    sem_post(wait_log);

    if( *ne > *nc){        
        sem_wait(wait_log);
        fprintf(fp, "%d \t: JUDGE\t: waits for imm \t: %d \t: %d \t: %d\n",(*action_n)++, *ne, *nc, *nb);
        sem_post(wait_log);
        
        sem_post(mutex);
        sem_wait(checked);
    }
    sem_wait(wait_log);
    fprintf(fp, "%d \t: JUDGE\t: starts confirmation \t: %d \t: %d \t: %d\n",(*action_n)++, *ne, *nc, *nb);   
    sem_post(wait_log);

    int token = *nc;
    *nc = *ne = 0;
    MSLEEP(JT);

    sem_wait(wait_log);
    fprintf(fp, "%d \t: JUDGE\t: ends confirmation \t: %d \t: %d \t: %d\n",(*action_n)++, *ne, *nc, *nb);
    sem_post(wait_log);
    
    if(token){
        //prvadepodne je to zoptimalizovane na -1 ::()
        while (token--)
        {   
            sem_post(confirmed);
        }
    }
    MSLEEP(JT);
    sem_wait(wait_log);
    fprintf(fp, "%d \t: JUDGE\t: leaves \t: %d \t: %d \t: %d\n",(*action_n)++, *ne, *nc, *nb);
    sem_post(wait_log);

    *judge = 0;
    sem_post(mutex);
    sem_post(noJudge);
    if(PI == *PC){
        sem_wait(wait_log);
        fprintf(fp, "%d \t: JUDGE\t: finishes\n",(*action_n)++);
        sem_post(wait_log);
        
        break;
        }
    }
    exit(0);
}


/**
 * @brief konvertuje string na int
 * @param argv char ktory bude zkonvertovany
 * @return zkonvertovane cislo ak ok, pri chybe ukonci program
 */ 
int convertor(char * argv){
    char *arg_check;
    long conversion = strtol(argv, &arg_check, 10);
    if (errno != 0 || *arg_check != '\0' || conversion > INT_MAX) {
        fprintf(stderr, "Zadane nevalidne argumenty\n");
        exit (1);
    }
    int converted = conversion;
    return converted;
}

//##########################################################################################

int main(int argc, char* argv[]) {
    if(argc != 6 ){
        fprintf(stderr, "Zadany nespravny pocet argumentov (%d namiesto 6)\n", argc); //6 aj s nazvom programu -> fix
        return 1;
    }
    //arg convert
    int PI = convertor(argv[1]);
    if(PI < 1){
        fprintf(stderr, "Zadana nevalidna hodnota '%d'\n", PI);
        return 1;
    }
    int IG = convertor(argv[2]);
    if(IG < 0 || IG > 2000){
        fprintf(stderr, "Zadana nevalidna hodnota '%d'\n", IG);
        return 1;
    }
    int JG = convertor(argv[3]);
    if(JG < 0 || JG > 2000){
        fprintf(stderr, "Zadana nevalidna hodnota '%d'\n", IG);
        return 1;
    }
    int IT = convertor(argv[4]);
    if(IT < 0 || IT > 2000){
        fprintf(stderr, "Zadana nevalidna hodnota '%d'\n", IG);
        return 1;
    }
    int JT = convertor(argv[5]);
    if(JT < 0 || JT > 2000){
        fprintf(stderr, "Zadana nevalidna hodnota '%d'\n", IG);
        return 1;
    }

    int start_err = mmap_start();
    if (start_err){
        fprintf(stderr,"Error: semafor/sdilena promenna!\n");
        perror("perror: ");
        clean_map();
		return 1;    
    }
    *action_n = 1;
    srand(time(0)); 

    pid_t Generator = fork();
    setbuf(fp, NULL);
    if(Generator == -1)
	{
		fprintf(stderr,"Error pri tvoreni procesu!\n");
		return 1;
	}
    if (Generator == 0){ //child -> generator imigrantov
        imm_gen(PI, IG, IT);
    }
    else{
            pid_t Generator2 = fork();
            if(Generator2 == -1)
                {
                    fprintf(stderr,"Error pri tvoreni procesu!\n");
                    return 1;
                }
                if (Generator2 == 0){ //child -> proces sudcu
                    judge_gen(JG, PI, JT);
                }
                else{
                    int returnStatus = 0;
                    waitpid(Generator, &returnStatus, 0);
                    if (returnStatus == 1){
                        fprintf(stderr, "Detsky proces skoncil s errorom.\n");   
                    }    
                    waitpid(Generator2, &returnStatus, 0);
                    if (returnStatus == 1){
                        fprintf(stderr, "Detsky proces skoncil s errorom.\n");   
                    }
                }   
    }
    clean_map();
    return 0;
}