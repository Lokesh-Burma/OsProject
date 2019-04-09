#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <stdbool.h>

#define const_p 100
#define const_r 100

int p=3,r=3,unsafe;
int allot[const_p][const_r]={{1,2,3},{4,5,6},{7,8,9}},
	need[const_p][const_r],
	avail[const_p][const_r]={1,2,3},
    maxm[const_p][const_r]={{2,3,4},{5,6,7},{8,9,10}};
int sequence[const_p];
int Process_number,n[const_p];

pthread_mutex_t lock;
pthread_t thread[const_p];

void NeedMatrix();
int CheckSafe();
void *GiveAccess(void *arg);
void custom();

int main()
{
    int start = 0;
    while(1)
    {
    	if(start==1)
    		return false;
        system("clear");

        printf("\n\t\t\tIMPLEMENTATION OF BANKERS ALGORITHM\n");
        printf("\t\t\t***********************************\n");

        pthread_mutex_init(&lock,NULL);
        int choice=0;

        printf("\n\t1. Enter default values\n\t2. Press any number to Enter custom values \n\tChoose : \n\t");
        scanf("%d",&choice);
        if(choice==1)
        {
            int p=3,r=3,RS=100;
            int allot[const_p][const_r]={{1,2,3},{4,5,6},{7,8,9}};
            int avail[const_p][const_r]={1,2,3};
            int maxm[const_p][const_r]={{2,3,4},{5,6,7},{8,9,10}};
        }

        if(choice!=1)
        	custom();
     
        printf("\n\n\t\t\t\t\t*RESULT*");
        printf("\n\t\t\t\t\t********\n");

        NeedMatrix();
        unsafe=CheckSafe();

        if (unsafe==false)
            goto end;

        for(int i=0;i<p;i++)
        	n[i]=i;

        for(int i=0;i<p;i++)
        {   
            pthread_create(&thread[i],NULL,GiveAccess,(void *)(&n[i]));
        }

        for(int i=0;i<p;i++)
        {
            pthread_join(thread[i],NULL);
        }

        printf("\n\n\tProcess\t\t Alloc\t\t Max\t\t Avail\t\t Need");
        printf("\n\t=======\t\t =====\t\t ===\t\t =====\t\t ====\n\n");
        for(int i=0;i<p;i++)
        {
            printf("\tP%d\t\t",i);
            for(int j=0;j<r;j++)
                printf("%d ",allot[i][j]);
            printf("\t\t");
            for(int j=0;j<r;j++)
                printf("%d ",maxm[i][j]);
            printf("\t\t");
            for(int j=0;j<r;j++)
                printf("%d ",avail[i][j]);
            printf("\t\t");
            for(int j=0;j<r;j++)
                printf("%d ",need[i][j]);
            printf("\n");
        }
        end:
        printf("\n\n\t Press 0 to enter Again, 1 to Exit : \n\t");
        scanf("%d",&start);
    }
}

void custom(){

	printf("\tPlease enter the number of threads : ");
    while(1)
    {
        scanf("%d",&p);
        if(p>const_p || p<=0)
            printf("\n\t\t!!!Please enter value between 0 and %d !!! : ",const_p);
        else
        	break;
    }

    printf("\n\tPlease enter the number of Resources : ");
    while(1)
    {
        scanf("%d",&r);
         if(r>const_r || r<=0)
            printf("\n\t\t!!! Please enter value between 0 and %d !!! : ",const_r);
        else
        	break;
    }

    for (int i=0;i<p;i++)
    {    
    	printf("\n\tEnter the resource allocation for thread%d : ",i);
        for (int j=0;j<r;j++)
        {
            while(1)
            {
                scanf("%d",&allot[i][j]);
                if(allot[i][j]<0)
                    printf("\n\t\t!!! Incorrect value detected,Please enter last value again !!! : ");
                else
                    break;
            }
        }
    }

    for (int i=0;i<p;i++)
    {    
    	printf("\n\tEnter the maximum resource for thread%d : ",i);
       for (int j=0;j<r;j++)
       {
            while(1)
            {
                scanf("%d",&maxm[i][j]);
                if(maxm[i][j]<allot[i][j])
                    printf("\n\t\t!!! Maximum value must be more than allocated value,Please replace wrong value!!! : ");
                else if(maxm[i][j]<0)
                    printf("\n\t\t!!! Incorrect value detected,Please replace wrong value again !!! : ");
                else
                    break;
            }
       }
    }

    printf("\n\tEnter the available resources : ");
    for(int i=0;i<r;i++)
    {
        while(1)
        {
            scanf("%d",&avail[0][i]);
            if(avail[0][i]<0)
                printf("\n\t\t!!! Incorrect value detected,Please replace wrong value !!! : ");
            else
                break;
        }
    }
}

void NeedMatrix()
{
    for (int i=0;i<p;i++)
        for (int j=0;j<r;j++)
            need[i][j] = maxm[i][j] - allot[i][j];
}

int CheckSafe()
{ 
    int count=0;
    int check[const_p] = {0};
    while (count<p)
    {
	int found=1;
        for (int i=0; i<p; i++)
        {
            if (check[i]==0)
            {   
                int j;
                for (j=0;j<r;j++)
                    if (need[i][j] > avail[count][j])
                    {
                        found = 0;
                        break;
                    }
                if (j==r)
                {   
                    for (int k=0; k<r; k++)
                    	avail[count+1][k] = avail[count][k] + allot[i][k];
                    sequence[count] = i;
                    check[i] = 1;
                    found = 1;
                    count++;
                }
            }
        }
        if (found==0)
        {
            printf("\n\t\t\t!!! Oops,System is not in safe state :( !!!\n");
            return false;
        }
    }
    printf("\n\tSystem is in safe state :), Safe sequence is : ");
    for (int i=0;i<p;i++)
        printf("%d ",sequence[i]);
    return true;
}

void *GiveAccess(void *arg)
{
    int Process_number = *((int *) arg);
    int i=0;
    
    while(1)
    {
        pthread_mutex_lock(&lock);

        printf("\n\n\t!!!Thread%d : p_id %lu is in safe state resource granted!!!\n",Process_number, pthread_self());
        sleep(1);
        printf("\tResources needed are : ");
        for(int j=0; j<r ;j++)
        	printf("%d ",need[Process_number][j]);
        sleep(1);
        printf("\n\tResources available are : ");
        for(int j=0; j<r ;j++)
        	printf("%d ",avail[Process_number][j]);
        sleep(1);
        printf("\n\t!!resources are released!!! \n");
        if(i<p-1)
            i++;
        else
            i=0;
        
        pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
    }
}

