#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//Author:LiuzhuoYun, 2014-6-1 ,implement the algorithm APCLUSTER Affinity Propagation Clustering 
#define R 31 //31  economic regions
#define Y 10 //statistic in 10 years
typedef struct
{
	int idx[R]; //the index of the cluster center for each data point,which shows the economic statistics in 10 years of an economic region of China
	float dpsim;//the sum of the similarities of the data points to their cluster centers.
	float expref;// the sum of the preferences of the identified cluster centers.
	float netsim;//sum of the data point similarities and preferences
	
}APinfo;

float data[R][Y];

int main()
{
	
	FILE *fp,*fpR;
	int i,j;
		for(i=0;i<R;i++)
		{
			for(j=0;j<Y;j++)
			data[i][j]=0.0;
		}
	//load the file 
		
	fpR=fopen("res.txt","w");
	fp=fopen("eco.txt","r+");
	if((fp==NULL)||(fpR==NULL))
	{
		printf("\n cannot open file\n");
		return 1;
	}
		for(i=0;i<R;i++)
		{
			for(j=0;j<Y;j++)
			{
		
			//	if (fread(&data[i][j],sizeof(float),1,fp)!=1)
			//	{
			fscanf(fp,"%f",&data[i][j]);
			fprintf(fpR,"%f ",data[i][j]);
			printf("%f,",data[i][j]);
					if(feof(fp))
					{
						fclose(fp);
						return;
					
					}
				//	printf("file read error!\n");
			//	}
				
			}
				fprintf(fpR,"\n");
				printf("\n");
		}
		fclose(fp);
		
	fclose(fpR);
	getchar();

	return 0;
}
