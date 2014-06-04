#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
//Author:LiuzhuoYun, 2014-6-1 ,implement the algorithm APCLUSTER Affinity Propagation Clustering 
#define R 31 //31  economic regions
#define Y 10 //statistic in 10 years
#define MAXI 100
#define L 0.5 //¦Ë

float data[R][Y];

int main()
{
    void loadData();
    void sim(float data[][Y],float s[][R],float p[]);
	void APcluster(float s[][R],float p[],int idx[]);
	int idx[R]; //the index of the cluster center for each data point,which shows the economic statistics in 10 years of an economic region of China	
	int i,j;
	float s[R][R],p[R];

	//////Initialize///
	for(i=0;i<R;i++)
	{
		for(j=0;j<Y;j++)
		   data[i][j]=0.0;
	}
	for(i=0;i<R;i++)
	{
		for(j=0;j<R;j++)
		   s[i][j]=0.0;
        p[i]=0.0;
        idx[i]=0;
	}
	/////load data to 2-d array from the txt file////	
	loadData();
	for(i=0;i<R;i++)
	{
		for(j=0;j<Y;j++)
		  printf("%f ",data[i][j]);
		printf("\n");
	}
	sim(data,s,p);
	APcluster(s,p,idx);
	
	//////output the result///
	for(i=0;i<R;i++)
	{
		
		printf("%d:%d\n",i,idx[i]);
	}
	getchar();

	return 0;
}

void loadData()
{
    FILE *fp;
    int i,j;
    //load  data 
	//fpR=fopen("res.txt","w+");
	fp=fopen("eco.txt","r");
	if(fp==NULL)
	{
		printf("\n cannot open file\n");
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
	
	fclose(fp);
} 
   
void APcluster(float s[][R],float p[],int idx[])
{ 
    float Max(float a[][R],float s[][R],int i);
    float maxSum(float z,float r[][R],int k);
    float lam=L;//¦Ë
    int i,j,k,k1;
    int times=0,mx=0;
	float a[R][R],r[R][R],e[R][R];
	float sum=0.0,tem=0.0,old=0.0,Rdis=0.0,Rdsum=0.0,Rdaver=0.0,Adis=0.0,Adsum=0.0,Adaver=0.0;
     //////Initialize///
	for(i=0;i<R;i++)
	{
		for(j=0;j<R;j++)
		{
		   a[i][j]=0.0;
		   r[i][j]=0.0;
		   e[i][j]=0.0;
        }	   
	}
	/////iterations////
	while(times<MAXI)
	{
	    times++;
	    ///Responsibility
       for(i=0;i<R;i++) 
       {
          k=0;
          for(k=0;k<R;k++) 
          {
              old=r[i][k];
              r[i][k]=s[i][k]-Max(a,s,i);  //r(i,k)<-s(i,k)-max{a(i,k')+s(i,k')}
              r[i][k]=(1-lam)*old+lam*r[i][k];
              Rdis=r[i][k]-old; //change of the values
              Rdsum=Rdsum+Rdis;
              
          }             
       }   
       Rdaver=Rdsum/R;  //calculate the average of the sum of the changes 
         
         ///availabilities 
        for(j=0;j<R;j++) 
       {
          k1=0;
          while(k1<R)
          {
              old=a[j][k1];
              if(j==k1)
              {
                  a[j][k1]=maxSum(0.0,r,k1);
              }    
              else
              {
                  tem=r[k1][k1]+maxSum(0.0,r,k1);
                  a[j][k1]=(0.0<tem)?0.0:tem;
              }    
              
              a[j][k1]=(1-lam)*old+lam*a[j][k1];
              Adis=a[j][k1]-old; //change of the values
              Adsum=Adsum+Adis;
              k1++;
          }   //while                   
       }  //for
       
        Adaver=Adsum/R;  //calculate the average of the sum of the changes    
        if((Rdaver<0.001)&&(Adaver<0.001))
          break;
     }
     
     for(i=0;i<R;i++)
     {
         mx=0;  
         for(j=0;j<R;j++)
         {
             e[i][j]=a[i][j]+r[i][j];
             if(e[i][j]>e[i][mx])
             {
                 mx=j;
             }    
         }  
         idx[i]=j;
     }      
}
///2014-6-3
void sim(float data[][Y],float s[][R],float p[])
{
    float median(float t[],int r);
    int i,k,j;
    float temp=0.0,min,t[R-1],tm1=0.0,tm2=0.0;
    for(i=0;i<R-1;i++)
    {
        t[i]=0.0;
    }
    for(i=0;i<R;i++)   
    {
        for(k=0;k<R;k++)
        {
            if(i!=k)
            {
            	temp=0;
                for(j=0;j<Y;j++)
                {
                	tm1=(float)(data[i][j]-data[k][j]);
                	tm2=(float)tm1*tm1;
                    temp=(float)temp+tm2;
                }    
                s[i][k]=-temp;
               // 
            }  //if  
        }    //for
    }    //for
    printf("s(29,30) %f \n",s[29][30]);
    for (k=0;k<R;k++)
    {
    	j=0;
        for(i=0;i<R;i++)
        {
        	if(i!=k)
        	{
        			t[j]=s[i][k];
        			j++;
        	}
            
        }   

        printf("\n"); 
        s[k][k]=median(t,R-1);
        p[k]=s[k][k];

     //   printf("p[%d]:%f ",k,p[k]); //test
    }
  	 //	   printf("\n"); //test
     //     for(i=0;i<R-1;i++)//test
     //      printf("t%d:%f  ",i,t[i]);//test
     //    printf("\n");//test
}

 float median(float t[],int r)
 {
     int i,j,k,mid;
     float tem,min;
     for(i=0;i<r;i++)
     {
         min=t[i];
         for(j=i;j<r;j++)
         {
             if(t[i]<min)
             {
                 min=t[i];
                 k=i;
             }    
             if(i!=k)
             {
                 tem=t[i];
                 t[i]=t[k];
                 t[k]=tem;
             }    
            
         }  
          mid=(int)(r)/2;
          
         // for(i=0;i<r;i++)
         //  printf("%d ",t[i]);
        //   printf("\n");
          return t[mid];  
     }    
 }    
 
 float Max(float a[][R],float s[][R],int i)
 {
     int j=0;
     float sum=0.0,max=(i>0)?(s[i][0]+a[i][0]):(s[i][1]+a[i][1]);
     while(j<R)
     {
         if(j==i) j++;
         else
         {
             sum=s[i][j]+a[i][j];
             if(sum>max)
             {
                 max=sum;
             }    
             j++;
         }    
     }  
     return max;  
 }    
 float maxSum(float z,float r[][R],int k)
 {
     int i=0;
     float max=z,sum=0.0;
     while(i<R)
     {
         if(i==k)i++;
         else
         {
             if(r[i][k]>max)
                max=r[i][k];
             sum=sum+max;
             i++;
         }    
     }    
     return sum;
 }    
