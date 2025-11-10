// C program for implementation of Scheduling Simulation 
#include <stdio.h> 
#include <limits.h>
#include <stdlib.h>
#include "process.h"
#include "util.h"

// ------------------ ROUND ROBIN ------------------
void findWaitingTimeRR(ProcessType plist[], int n, int quantum) 
{ 
    int rem_bt[n]; // remaining burst time
    for (int i = 0; i < n; i++) 
        rem_bt[i] = plist[i].bt;

    int t = 0; // current time

    while (1) {
        int done = 1;

        for (int i = 0; i < n; i++) {
            if (rem_bt[i] > 0) {
                done = 0;

                if (rem_bt[i] > quantum) {
                    t += quantum;
                    rem_bt[i] -= quantum;
                } else {
                    t += rem_bt[i];
                    plist[i].wt = t - plist[i].bt - plist[i].art;
                    if (plist[i].wt < 0) plist[i].wt = 0;
                    rem_bt[i] = 0;
                }
            }
        }

        if (done == 1)
            break;
    }
} 

// ------------------ SHORTEST JOB FIRST ------------------
void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
    int rt[n]; // remaining time
    for (int i = 0; i < n; i++)
        rt[i] = plist[i].bt;

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    int check = 0;

    while (complete != n) {
        for (int j = 0; j < n; j++) {
            if ((plist[j].art <= t) && (rt[j] < minm) && rt[j] > 0) {
                minm = rt[j];
                shortest = j;
                check = 1;
            }
        }

        if (check == 0) {
            t++;
            continue;
        }

        rt[shortest]--;
        minm = rt[shortest] ? rt[shortest] : INT_MAX;

        if (rt[shortest] == 0) {
            complete++;
            check = 0;
            finish_time = t + 1;

            plist[shortest].wt = finish_time - plist[shortest].bt - plist[shortest].art;
            if (plist[shortest].wt < 0)
                plist[shortest].wt = 0;
        }

        t++;
    }
} 

// ------------------ FCFS (Provided) ------------------
void findWaitingTime(ProcessType plist[], int n)
{ 
    plist[0].wt = 0 + plist[0].art; 
  
    for (int i = 1; i < n; i++) 
        plist[i].wt = plist[i-1].bt + plist[i-1].wt; 
} 
  
// ------------------ TURNAROUND TIME ------------------
void findTurnAroundTime(ProcessType plist[], int n)
{ 
    for (int i = 0; i < n; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 

// ------------------ PRIORITY SORTING COMPARER ------------------
int my_comparer(const void *this, const void *that)
{ 
    const ProcessType *p1 = (const ProcessType *)this;
    const ProcessType *p2 = (const ProcessType *)that;

    // lower priority value = higher actual priority
    if (p1->pri < p2->pri) return -1;
    if (p1->pri > p2->pri) return 1;
    return 0;
} 

// ------------------ FCFS AVG TIME ------------------
void findavgTimeFCFS(ProcessType plist[], int n) 
{ 
    findWaitingTime(plist, n); 
    findTurnAroundTime(plist, n); 
    printf("\n*********\nFCFS\n");
}

// ------------------ SJF AVG TIME ------------------
void findavgTimeSJF(ProcessType plist[], int n) 
{ 
    findWaitingTimeSJF(plist, n); 
    findTurnAroundTime(plist, n); 
    printf("\n*********\nSJF\n");
}

// ------------------ RR AVG TIME ------------------
void findavgTimeRR(ProcessType plist[], int n, int quantum) 
{ 
    findWaitingTimeRR(plist, n, quantum); 
    findTurnAroundTime(plist, n); 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

// ------------------ PRIORITY AVG TIME ------------------
void findavgTimePriority(ProcessType plist[], int n) 
{ 
    // Sort processes by priority
    qsort(plist, n, sizeof(ProcessType), my_comparer);

    // Then apply FCFS logic
    findWaitingTime(plist, n);
    findTurnAroundTime(plist, n);

    printf("\n*********\nPriority\n");
}

// ------------------ PRINT METRICS ------------------
void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    for (int i = 0; i < n; i++) { 
        total_wt += plist[i].wt; 
        total_tat += plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

// ------------------ INIT PROCESSES ------------------
ProcessType * initProc(char *filename, int *n) 
{
    FILE *input_file = fopen(filename, "r");
    if (!input_file) {
        fprintf(stderr, "Error: Invalid filepath\n");
        fflush(stdout);
        exit(0);
    }

    ProcessType *plist = parse_file(input_file, n);
    fclose(input_file);
    return plist;
}
  
// ------------------ MAIN DRIVER ------------------
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;
    ProcessType *proc_list;
  
    if (argc < 2) {
        fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
        fflush(stdout);
        return 1;
    }
    
    // ---- FCFS ----
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeFCFS(proc_list, n);
    printMetrics(proc_list, n);
  
    // ---- SJF ----
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeSJF(proc_list, n); 
    printMetrics(proc_list, n);
  
    // ---- Priority ----
    n = 0; 
    proc_list = initProc(argv[1], &n);
    findavgTimePriority(proc_list, n); 
    printMetrics(proc_list, n);
    
    // ---- Round Robin ----
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeRR(proc_list, n, quantum); 
    printMetrics(proc_list, n);
    
    return 0; 
}
