#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//Author:LiuzhuoYun, 2014-6-1 ,implement the algorithm APCLUSTER Affinity Propagation Clustering 
#define R 31 //31  economic regions
#define Y 10 //statistic in 10 years
typedef struct
{
	int idx[R]; //the index of the cluster center for each data point,which shows the economic statistics in 10 years of an economic region of China
	double dpsim;//the sum of the similarities of the data points to their cluster centers.
	double expref;// the sum of the preferences of the identified cluster centers.
	double netsim;//sum of the data point similarities and preferences
	
}APinfo;

float data[R][Y];

int main()
{
	
	FILE *fp;
	int i,j;
	
		for(i=0;i<R;i++)
		{
			for(j=0;j<Y;j++)
			   data[i][j]=0.0;
		}
	//load the file 
		
	//fpR=fopen("res.txt","w+");
	fp=fopen("eco.txt","r");
	if(fp==NULL)
	{
		printf("\n cannot open file\n");
		return 1;
	}
	for(i=0;i<R;i++)
	{
        for(j=0;j<Y;j++)
                fscanf(fp,"%f",&data[i][j]);
		//fprintf(fpR,"%f ",data[i][j]);
		fscanf(fp,"\n");
		if(feof(fp))
		{
				break;		
		}
		//		fprintf(fpR,"\n");
				
		}
		printf("Line10:%f %f %f %f %f %f %f %f %f %f\n",data[10][0],data[10][1],data[10][2],data[10][3],data[10][4],data[10][5],data[10][6],data[10][7],data[10][8],data[10][9]);
	fclose(fp);
	printf("%d  %d\n",R,Y);
	for(i=0;i<R;i++)
	{
		for(j=0;j<Y;j++)
		  printf("%f ",data[i][j]);
  		  printf("\n");
	}	
//	fclose(fpR);
	getchar();

	return 0;
}
