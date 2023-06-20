#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int stack[1000][4],top=-1;
int tickets[1000],t_top=-1;
int count[1000][2]={0};

int m=-1;  //stores the number of lottery tickets

int search(int ticket_no);
void bubble_sort_2(int n,int arr[1000][3]);
void process_generator(int *n,int arrival_time[1000][3]);
void bubble_priority(int n,int arr[1000][3]);
void display_process(int n,int arr[1000][3]);
void push(int n0,int n1,int n2);
int lottery_scheduler(int arrival[1000][3],int n,int run_time);
void sort_stack();
int lottery_s();
int assign_tickets();
void display_count();

int main(){
    srand(time(0)); 
    int arrival[1000][3];
    int n;

    process_generator(&n,arrival);
    bubble_sort_2(n,arrival);
    bubble_priority(n,arrival);
    //int total_time=total_running_time(n,arrival);
    int running_time=100;
    display_process(n,arrival);

    printf("\n\n##### LOTTERY SCHEDULING #####\n");
    int n1=lottery_scheduler(arrival,n,running_time);
    printf("Total number of process switch=%d\n",n1);

    display_count();

    return 0;
}

int lottery_scheduler(int arrival[1000][3],int n,int run_time){
    int count=0;
    printf("\n****TOTAL RUNNING TIME=%d*****\n",run_time);
    int i,j,pos=0;
    int start=arrival[0][1],stop;
    for(i=0;i<n-1;i++){
        if(arrival[i][1]!=arrival[i+1][1]){
            stop=arrival[i+1][1]-1;

            for(j=pos;j<=i;j++){
                push(arrival[j][0],arrival[j][1],arrival[j][2]);
            }

            while(start<=stop && top!=-1 && start<=run_time){
                int tp=lottery_s();
                count++;
                printf("enrty_time=%d\texit_time=%d\n\n",start,start+tp);
                start+=tp;
            }

            pos=i+1;
            start=(arrival[i+1][1]>start)?arrival[i+1][1]:start;
        }
    }
    while(pos<n){
        push(arrival[pos][0],arrival[pos][1],arrival[pos][2]);
        pos++;
    }
    while(top>-1 && start<=run_time){
        int tp=lottery_s();
        count++;
        if((start+tp)>run_time)
            printf("enrty_time=%d\texit_time=%d PARTIALLY EXECUTED\n\n",start,run_time);
        else
            printf("enrty_time=%d\texit_time=%d\n\n",start,start+tp);
        start+=tp;
    }

    return count;
}

int search(int ticket_no){
    int process_id;
    int i=-1;
    for(i=0;i<=t_top;i++){
        if(tickets[i]>=ticket_no){
            process_id=i;
            break;
        }
    }
    return i;
}

void process_generator(int *n,int arrival_time[1000][3]){
    //arr[i] = [process_no   arrival_time   priority]
    //maximum number of processes
    *n = rand()%10+1;
    int i;
    for(i=0;i<*n;i++){
        arrival_time[i][1]=rand()%10;
        arrival_time[i][0]=i;
        arrival_time[i][2]=rand();
    }
}

void bubble_sort_2(int n,int arr[1000][3]){
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<(n-i-1);j++)
            if(arr[j][1]>arr[j+1][1]){
                int temp=arr[j][1];
                arr[j][1]=arr[j+1][1];
                arr[j+1][1]=temp;

                temp=arr[j][0];
                arr[j][0]=arr[j+1][0];
                arr[j+1][0]=temp;

                temp=arr[j][2];
                arr[j][2]=arr[j+1][2];
                arr[j+1][2]=temp;
            }
}

void bubble_priority(int n,int arr[1000][3]){
    int i,j,k;
    int pos=0;
    for(i=0;i<n-1;i++){
        if(arr[i][1]!=arr[i+1][1]){
            for(j=pos;j<=i;j++)
                for(k=pos;k<=i-j-1;k++)
                    if(arr[k][2]>arr[k+1][2]){
                        int temp=arr[k][1];
                        arr[k][1]=arr[k+1][1];
                        arr[k+1][1]=temp;

                        temp=arr[k][0];
                        arr[k][0]=arr[k+1][0];
                        arr[k+1][0]=temp;

                        temp=arr[k][2];
                        arr[k][2]=arr[k+1][2];
                        arr[k+1][2]=temp;
                    }
            pos=i+1;
        }
    }
}

void display_process(int n,int arr[1000][3]){
    printf("process_no\tarrival_timec\tPriority\n");
    int i;
    for(i=0;i<n;i++)
        printf("%d\t\t%d\t\t%d\n",arr[i][0],arr[i][1],arr[i][2]);
}

void push(int n0,int n1,int n2){
    ++top;    
    stack[top][0]=n0;
    stack[top][1]=n1;
    stack[top][2]=n2;
}

int lottery_s(){
    int choice,crun,rtime,priority;
    if(top==0)
    {
        choice=0;
        crun=stack[choice][0];
        rtime=2;
        priority=stack[choice][2];
        top--;
    }
    else{
        sort_stack();
        int m=assign_tickets();

        choice=((rand())%m)+1;

        int index=search(choice);

        if(index==-1)
            printf("\n****ERROR ENCOUNTERED*****\n");

        crun=stack[index][0];
        priority=stack[index][2];
        rtime=2;
    }
    printf("Running ~ Process id: %d for %d sec\n",crun,rtime);
    count[crun][0]++;
    count[crun][1]=priority;
    return rtime;
}

int assign_tickets(){
    int i,count=0;
    for(i=0;i<=top;i++){
        count+=stack[i][2];
        tickets[++t_top]=count;
    }
    return count;
}

void sort_stack(){
    int i,j;
    for(i=0;i<=top;i++)
        for(j=0;j<=(top-i-1);j++)
            if(stack[j][2]>stack[j+1][2]){
                int temp=stack[j][2];
                stack[j][2]=stack[j+1][2];
                stack[j+1][2]=temp;

                temp=stack[j][1];
                stack[j][1]=stack[j+1][1];
                stack[j+1][1]=temp;

                temp=stack[j][0];
                stack[j][0]=stack[j+1][0];
                stack[j+1][0]=temp;
            }
}

void display_count(){
    int i;
    printf("\n\n#### ANALYSIS####\n");
    for(i=0;i<1000;i++)
        if(count[i][0]!=0)
            printf("Process ID-->%d Sheduled %d times; Priority-->%d\n",i,count[i][0],count[i][1]);
}
