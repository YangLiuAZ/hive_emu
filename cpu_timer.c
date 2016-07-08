#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <linux/limits.h>
#include <sys/times.h>

#include "cpu_timer.h"
typedef long long int num;
void readone(FILE* input1, num *x) { fscanf(input1, "%lld ", x); }
void readstr(FILE* input1,char *x) {  fscanf(input1, "%s ", x);}
void readchar(FILE* input1,char *x) {  fscanf(input1, "%c ", x);}
double utimeRead(pid_t pid){
num pid_now;
char tcomm[100];
char state;
FILE *input = 0;
num ppid;
num pgid;
num sid;
num tty_nr;
num tty_pgrp;

num flags;
num min_flt;
num cmin_flt;
num maj_flt;
num cmaj_flt;
num utime;
num stimev;
char pid_dir[20];
long  tickspersec;
tickspersec = sysconf(_SC_CLK_TCK);
sprintf(pid_dir,"/proc/%d/stat",pid);
 input = fopen(pid_dir, "r"); 
    if(!input) {
      perror("open");
      return -1;
    }
  fscanf(input,"%lld ",&pid_now);
  fscanf(input, "%s ",tcomm);
  readchar(input,&state);
  fscanf(input,"%lld ",&ppid);
  fscanf(input,"%lld ",&pgid);
  fscanf(input,"%lld ",&sid);
  fscanf(input,"%lld ",&tty_nr);
  fscanf(input,"%lld ",&tty_pgrp);
  fscanf(input,"%lld ",&flags);
  fscanf(input,"%lld ",&min_flt);
  fscanf(input,"%lld ",&cmin_flt);
  fscanf(input,"%lld ",&maj_flt);
  fscanf(input,"%lld ",&cmaj_flt);
  fscanf(input,"%lld ",&utime);
  fscanf(input,"%lld ",&stimev);
fclose(input);
return ((double)utime)/tickspersec;
}
