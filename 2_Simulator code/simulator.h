# ifndef __SIMULATOR__
# define __SIMULATOR__
# define L 25
# define N 20
# define n 20

//structure for all the processes
struct Process
{
	int pid;      // process id
	int priority; // priority
	int AT;       // arrival time
	int BT;       // burst time
	int WT;       // waiting time
    int TAT;      // turnaround time
	int RT;       // remaining time
	int CH;       // check variable
    int ct;       // Current Time
    int CT;       //Completion time
};

extern void get_input(struct Process **ready, char *);
extern void sort_arrival_time(struct Process **ready);
extern void sort_arrival_burst_time(struct Process **ready);
extern void sort_arrival_priority_time(struct Process **ready);
extern void sort_priority(struct Process *ready, int size);
extern void sort_pid(struct Process *p, int size);
extern void copy_struct_info(struct Process *p, struct Process *q);
extern void display_queue(struct Process **p, int size, int num);
extern void wait_time_FCFS(struct Process *p, int size);
extern void turn_time_FCFS(struct Process *p, int size);
extern int FCFS(char *input);
extern int PS_NON_PREEMPTIVE(char *input);
extern int PS_PREEMPTIVE(char *input);
extern void wait_time_SJFNP(struct Process *p);
extern void turn_time_SJFNP(struct Process *p);
extern int SJF_NON_PREEMPTIVE(char *input);
extern void sort_SJFP(int readySJFP[2][50],int n1);
extern void completion_time(struct Process readySJFP[L][N],int h);
extern float turn_time_SJFP(struct Process readySJFP[L][N],int h);
extern float wait_time_SJFP(struct Process readySJFP[L][N],int h);
extern void get_input_SJFP(struct Process readySJFP[L][N], char input[]);
extern void display_queue_SJFP(struct Process readySJFP[L][N], int size, int num);
extern int SJF_PREEMPTIVE(char input[]);
extern int RR(char input[]);
extern void level_queueing(struct Process **ready);
extern int MLQ(char input[]);
extern void arrivalsortMLFQ();
extern void get_input_MLFQ(struct Process Queue1[L][N]);
extern int MLFQ();

# endif