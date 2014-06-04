#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
//Author:LiuzhuoYun, 2014-6-1~2014-6-5 ,implement the algorithm APCLUSTER Affinity Propagation Clustering 
#define R 31 //31  economic regions
#define Y 10 //statistic in 10 years
#define MAXI 200
#define L 0.5 //¦Ë
#define Threshold 0.001

typedef enum {false = 0, true = 1} bool;
float data[R][Y];
char province[R][14]={"Beijing","Tianjing","Hebei","Shanxi","InnerMongolia","Liaoning","Jilin","Heilongjiang","Shanghai","Jiangsu","Zhejiang","Anhui","Fujian","Jiangxi","Shandong","Henan","Hubei","Hunan","Guangdong","Guangxi","Hainan","Chongqin","Sichuan","Guizhuo","Yunnan","Tibet","Shanxi","Gansu","Qinghai","Ningxia","Xinjiang"}; 

int main()
{
    void loadData();
    void sim(float data[][Y],float s[][R],float p[]);
	void APcluster(float s[][R],float p[],int idx[]);
	void outputRes(int in[],FILE *fp);
	int idx[R]; //the index of the cluster center for each data point,which shows the economic statistics in 10 years of an economic region of China	
	int i,j;
	float s[R][R],p[R];
	FILE *in;
    //input the result into this file
	in=fopen("res.txt","w+");
	
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
	/////load data to 2-d array from the txt file
	loadData();
	
	fprintf(in,"Show the economic data of 31 economic region of China in 10 years:\n ");
	for(i=0;i<R;i++) 
	{
		fprintf(in,"%15s ",province[i]);
		for(j=0;j<Y;j++)
		  fprintf(in,"%f ",data[i][j]);
		fprintf(in,"\n");
	}
	sim(data,s,p);
	APcluster(s,p,idx);
	
	//////output the result //test
	//for(i=0;i<R;i++)
	//{
		
	//	printf("%d:%d\n",i,idx[i]);
	//}
	outputRes(idx,in);
	fclose(in);
	
	getchar();

	return 0;
}

void loadData()
{
    FILE *fp;
    int i,j;
    //load  data 
	fp=fopen("eco.txt","r");
	if(fp==NULL)
	{
		printf("\n cannot open file\n");
	}
	for(i=0;i<R;i++)
	{
        for(j=0;j<Y;j++)
            fscanf(fp,"%f",&data[i][j]);

		fscanf(fp,"\n");
		if(feof(fp))
		{
				break;		
		}
		}
	
	fclose(fp);
} 

/////implement APcluser algorithm
void APcluster(float s[][R],float p[],int idx[])
{ 
    float Max(float a[][R],float s[][R],int i);
    float maxSum(float z,float r[][R],int k);
    float lam=L;//¦Ë
    int i,j,k,k1;
    int times=0,mx=0;
	float a[R][R],r[R][R],e[R][R];
	float sum=0.0,tem=0.0,old=0.0,Rdis=0.0,Rdsum=0.0,Rdaver=0.0,Adis=0.0,Adsum=0.0,Adaver=0.0;
	bool flag=true;
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
	while(flag)
	{
	    times++;
	    Rdsum=0.0;
	    Adsum=0.0;
	    ///Responsibility
       for(i=0;i<R;i++) 
       {
          k=0;
          for(k=0;k<R;k++) 
          {
              old=r[i][k];
              r[i][k]=s[i][k]-Max(a,s,i);       //formula1:r(i,k)<-s(i,k)-max{a(i,k')+s(i,k')}
              r[i][k]=(1-lam)*old+lam*r[i][k];  //formula2:r(new)=¦Ë*r(new)+(1-¦Ë)*r(old)
              Rdis=r[i][k]-old; //calculate the change 
             // printf("Rdis(%d,%d):%f ",i,k,Rdis);
              Rdsum=Rdsum+Rdis; //sum of the changes
          }             
       }   
       Rdaver=Rdsum/R;  //calculate the average of the sum of the changes 
          //printf("\nRdaver %f , ",Rdaver); //test
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
                  tem=r[k1][k1]+maxSum(0.0,r,k1);  //formula3: r(k,k)+sum of max{0,r(i',k)}
                  a[j][k1]=(0.0<tem)?0.0:tem;      //formula4:min{0,formula3}
              }    
              
              a[j][k1]=(1-lam)*old+lam*a[j][k1];  //formula:r(new)=¦Ë*r(new)+(1-¦Ë)*r(old)
              Adis=a[j][k1]-old;  //calculate the change 
              Adsum=Adsum+Adis;   //sum of the changes
              k1++;
          }   //while                   
       }  //for
       
        Adaver=Adsum/R;  //calculate the average of the sum of the changes  
		
	  for(i=0;i<R;i++)
     {
   
         for(j=0;j<R;j++)
         {
             e[i][j]=a[i][j]+r[i][j];

         }  
         
     }   
     //e(i,k)=maxk{e(i,k)},then make the exemplar decisions
	  for(i=0;i<R;i++)
     {
         mx=0;  
        
        for(j=0;j<R;j++)
         {
             if(e[i][j]>e[i][mx])
             {
               	  mx=j;
             }  
		 }  
		 	idx[i]=mx;  
     }     
        //printf("\nAdaver %f , ",Adaver); //test
        if(((Rdaver*Rdaver)<Threshold) && ((Adaver*Adaver) <Threshold)||(times>MAXI))	
          flag=false;
               
     }
     
printf("Iterators : %d\n ",times);
} 
///calculate the similarity matrix and diagonal array according to the data
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
    //printf("s(29,30) %f \n",s[29][30]); //test
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

     //   printf("\n");  //test
        s[k][k]=median(t,R-1);
        p[k]=s[k][k];

     //   printf("p[%d]:%f ",k,p[k]); //test
    }
  	 //	   printf("\n"); //test
     //     for(i=0;i<R-1;i++)//test
     //      printf("t%d:%f  ",i,t[i]);//test
     //    printf("\n");//test
}

//get the median value of array t
 float median(float t[],int r)
 {
     int i,j,k,mid;
     float tem,min;
     bool exchange=false;
     //Buble Sort
     for(i=0;i<r;i++)  
     {
         exchange=false;
         for(j=r-1;j>=i;j--)
         {
         	if(t[j+1]<t[j])
         	{
         	     tem=t[j+1];
                 t[j+1]=t[j];
                 t[j]=tem;
                 exchange=true;
         	}
         }
           	if(!exchange)
         		break;      
     }  
          mid=(int)(r)/2;
          
         // for(i=0;i<r;i++)
         //  printf("%d ",t[i]);
        //   printf("\n");
          return t[mid];      
 }    
 
////calculate the formula: max{a(i,k')+s(i,k')}
 float Max(float a[][R],float s[][R],int i)
 {
     int j=0;
     float sum=0.0,max=(i>0)?(s[i][0]+a[i][0]):(s[i][1]+a[i][1]);
     while(j<R)
     {
     	 sum=0.0;
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
 
 //calculate the sum of max{0,r(i',k)}
 float maxSum(float z,float r[][R],int k)
 {
     int i=0;
     float max=z,sum=0.0;
     while(i<R)
     {
     	max=z;
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
 
 void outputRes(int in[],FILE *fp)
 {
 	int i,j,k=0;
	bool flag[R];
 	int cmatrix[R][R];
 	//Initialize
 	for(i=0;i<R;i++)
 		flag[i]=false;
 	for(i=0;i<R;i++)
 		for(j=0;j<R;j++)
 			cmatrix[i][j]=0;
 		
 	for(i=0;i<R;i++)
 	{
 		for(j=0;j<R;j++)
 		{
 			if(in[i]==j)
 			{
			 	cmatrix[i][j]=1;
 			}
 			//printf("%d ",cmatrix[i][j]);//test
 			
 		}
 		 //printf("\n");  //test
 	}
	 	
	fprintf(fp,"\nClustering result: ");
 	for(j=0;j<R;j++)
 	{		
 		for(i=0;i<R;i++)
 		{
 			if(cmatrix[i][j]!=0)
 			{
 				if(flag[j]==false)  //find the first element of the cluster k
 				{
 					k++;
 					fprintf(fp,"\n"); 
 					fprintf(fp,"cluster%d:",k);
 					flag[j]=true;
 				
 				}
 					fprintf(fp,"%s  ",province[i]);
 				
 			}
 		}
 			
 	}
 	
}
 	

    
