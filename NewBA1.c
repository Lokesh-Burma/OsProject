#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int p=5;
int r=4;
int avail[5] = {1, 5, 2, 0};
int need[5][4];
int allot[5][4] = {{0, 0, 1, 2},
             	{1, 0, 0, 0},
	     	{1, 3, 5, 4},
	     	{0, 6, 3, 2},
	     	{0, 0, 1, 4}};

int maxm[5][4] = {{0, 1, 1, 2},
             	 {1, 7, 5, 0},
            	 {2, 3, 5, 6},
              	 {0, 6, 5, 2},
              	 {0, 6, 5, 6}};

void *calculateNeed()
{
    for (int i = 0 ; i < p ; i++)
        for (int j = 0 ; j < r ; j++)
            need[i][j] = maxm[i][j] - allot[i][j];
}

void *IsSafe()
{ 
    int finish[5] = {0,0,0,0,0};
    int safeSeq[p];
    int work[r];
    for (int i = 0; i < r; i++)
        work[i] = avail[i];
    int count = 0;
    while (count < p)
    {
	int found = 0;
        for (int z = 0; z < p; z++)
        {
            if (finish[z] == 0)
            {
                int j;
                for (j = 0; j < r; j++)
                    if (need[z][j] > work[j])
                        break;
                if (j == r)
                {
                    for (int k = 0 ; k < r ; k++)
                        work[k] += allot[z][k];
                    safeSeq[count++] = z;
                    finish[z] = 1;
                    found = 1;
                }
            }
        }
        if (found == 0)
        {
            printf("System is not in safe state\n");
          //  return 0;
		break;
        }
    }
    printf("System is in safe state.\nSafe sequence is: \n");
    for (int i = 0; i < p ; i++)
        printf("%d  ",safeSeq[i]);
    //return 1;
}

int main()
{
int i=0;
int j=0;
printf("Default Mode\n");
for(i=0;i<r;i++)
printf("%d\t",avail[i]);

printf("\nMaximum Resources for processes\n");
for(i=0;i<p;i++)
{
for(j=0;j<r;j++)
printf("%d\t ",maxm[i][j]);
printf("\n");
}
printf("Resources allocated to processes\n");
for(i=0;i<p;i++)
{
for(j=0;j<r;j++)
printf("%d\t ",allot[i][j]);
printf("\n");
}
calculateNeed();
printf("Resources needed to processes\n");
for(i=0;i<p;i++)
{
for(j=0;j<r;j++)
printf("%d\t",need[i][j]);
printf("\n");
}
IsSafe();

}
