#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define p1 100
#define r1 100
int np=0;
int p,r,z;
int avail[p1][r1],need[p1][r1],allot[p1][r1];
int maxm[p1][r1];
int check[p1] = {0}, sequence[p1];

pthread_mutex_t m;

void *NeedMatrix()
{
    for (int i=0;i<p;i++)
        for (int j=0;j<r;j++)
            need[i][j] = maxm[i][j] - allot[i][j];
}

void *GiveAccess()
{
    pthread_mutex_lock(&m);
    printf("Process %d is in safe state and is granted access to resource \n",z);
    pthread_mutex_unlock(&m);
}

void *CheckSafe()
{ 
    int count=0;
    while (count<p)
    {
	int found=0;
        for (z=0;z<p;z++)
        {
            if (check[z]==0)
            {   int j;
                for (j=0;j<r;j++)
                    if (need[z][j] > avail[np][j])
                        break;
                if (j==r)
                {   
                    np++;
                    GiveAccess();
                    for (int k=0;k<r;k++)
                    {   
                        avail[np][k] = avail[np-1][k]+allot[z][k];
                    }
                    sequence[count++] = z;
                    check[z] = 1;
                    found = 1;
                }
            }
        }
        if (found==0)
        {
            printf("System is not in safe state\n");
            return 0;
		  break;
        }
    }
    printf("System is in safe state, Safe sequence is: ");
    for (int i=0;i<p;i++)
        printf("%d,",sequence[i]);
}

int main()
{
    //pthread_t a;
    pthread_mutex_init(&m,NULL);
    //pthread_create(&a,NULL,GiveAccess,NULL);
    //pthread_join(a,NULL)
    int i=0,j=0;
    printf("Please enter the number of Processes : ");
    while(p<100)
    {
        scanf("%d",&p);
        if(p>p1)
            printf("Please enter value below %d : ",p1);
        else
        break;
    }

    printf("Please enter the number of Resources : ");
    scanf("%d",&r);

    printf("Enter the resource allocation Matrix : ");
    for (i=0;i<p;i++)
    {    
        for (j=0;j<r;j++)
            scanf("%d",&allot[i][j]);
    }

     printf("Enter the maximum resource Matrix : ");
    for (i=0;i<p;i++)
    {    
       for (int j=0;j<r;j++)
            scanf("%d",&maxm[i][j]);
    }

    printf("Enter the available resources : ");
    for(i=0;i<r;i++)
        scanf("%d",&avail[0][i]);

    NeedMatrix();
    CheckSafe();

    printf("\nAlloc\t\t Max\t\t Avail\t\t Need\n");
    printf("\n=====\t\t ===\t\t =====\t\t ====\n");
    for(i=0;i<p;i++)
    {
        for (j=0;j<r;j++)
            printf("%d  ",allot[i][j]);
        printf("\t");
        for (int j=0;j<r;j++)
            printf("%d  ",maxm[i][j]);
        printf("\t");
        for(j=0;j<r;j++)
            printf("%d  ",avail[i][j]);
        printf("\t");
        for(j=0;j<r;j++)
            printf("%d  ",need[i][j]);
        printf("\n");
    }
}
