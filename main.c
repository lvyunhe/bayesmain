#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#include"hash2.c"
#include"time.h"

int cid[20000];
float pcid[20000];
int c_count=0;

void get_data1(char* file){
	FILE* fp;
	char string[10000];
	if((fp=fopen(file,"r"))==NULL){
		printf("can not open the file!\n");
		exit(0);
	}
	char *k;
	char *v;
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		if(!feof(fp)){
			k=strtok(string,"\t");
			v=strtok(NULL,"\t");
			cid[c_count]=atoi(k);
			pcid[c_count]=atof(v);
			c_count+=1;
		}
	}
	fclose(fp);
}

//读入预测文件
void main(){
	double start,finish;
	start=clock();
			
	FILE *fp;
	char arr[1024];
	char file[20];
	printf("输入测试文件\n");
	scanf("%s",file);
	//读取预测word
	if((fp=fopen(file,"r"))==NULL){
		printf("error");
		exit(0);
	}


	hash_table1_init();
	keep_data2("script2");

	get_data1("script1");

	char *a[100];
	char *title;
	float count=0;
	float Rcount=0;
	while(!feof(fp)){
		fgets(arr,1024,fp);
		if(!feof(fp)){
			char *ch=strtok(arr,"\n");
			char *buf=strtok(arr,"\t");
			int matchcid,rcid;
			while(buf!=NULL){
				title=buf;
				buf=strtok(NULL,"\t");
				rcid=atoi(title);
				if(rcid!=0&&rcid>=10000)
					matchcid=rcid; //当word中有数字时不准确
			}
			printf("%s	真正的类目：%d ",title,matchcid);
			count++;
	        int i=0;
	        char *buffer=strtok(title," ");
	        while(buffer!=NULL){
				a[i++]=buffer;
		        buffer=strtok(NULL," ");
			}
			int wordcount=i;
			int k,j;
			int R_cid;
	        float pmax=0,sum;
					for(k=0;k<c_count;k++){
						sum=pcid[k];
						for(j=0;j<wordcount;j++){
							Listnode *p2=hash_table1_lookup(a[j]);
							while(p2){
								if(p2->Cid==cid[k]){
									sum=sum+(p2->Value);
									break;
								}
								else
									p2=p2->next;
							}
						}
							if(sum<pmax){
								pmax=sum;
								R_cid=cid[k];
						}
					}
			printf("预测的类目：%d\n",R_cid);
			if(R_cid==matchcid)
				Rcount++;
		}
	}
	float R=(Rcount/count)*100;
	printf("正确率：%f%\n",R);
	finish=clock();
	printf("运行时间：%f seconds\n",(finish-start)/CLOCKS_PER_SEC);
}
	


/*
void main(){
	char str[100];
	FILE *fp;
	char* a[100];
	int i,j,m,count=1;
	fp=fopen("test","r");
	while(!feof(fp)){
		fgets(str,sizeof(str),fp);
		int length=strlen(str);
	//统计word数目count
	for(i=0;i<length;i++){
		if(str[i]=='\t'){
			count++;
			i++;
		}
	}
	if(!feof(fp)){
	      	a[0]=strtok(str,"\t");
			for(m=1;m<count;m++)
			{a[m]=strtok(NULL,"\t");}
		}
	}

	//将词典p(cid)存入哈希表
	hash_table_init();
	keep_data1("script1");
	
	//将词典p(term/cid)存入哈希表
	hash_table1_init();
	keep_data2("script2");
    int k;
	float pmax=-100;
	int rcid;
	for(k=0;k<MAX_SIZE;++k){
		if(hashTable[k]){
			Node* p1=hashTable[k];
			while(p1){
				float sum=p1->Value;
				for(j=0;j<count;j++){
					//如果单词wordj在词典2中存在，找到wordj地址
					if(hash_table1_lookup(a[j])){
						Listnode *p2=hash_table1_lookup(a[j]);
			//找到两个词典中相同的cid项求出p(cid)*p(wordj/cid)(j=0:count-1)
						while(p2){
							if((hash_table1_lookup(a[j]))->Cid==atoi(p1->Key)){
								sum=sum+((hash_table1_lookup(a[j]))->Value);
								break;
							}
							else
								p2=p2->next;
						}
				}
				}
				//将最大的概率和存入pmax，并记录正确的cid
						if(sum>pmax){
					pmax=sum;
					rcid=atoi(p1->Key);
					}
				p1=p1->next;
			}
		}
	}
	printf("正确的类目cid:%d  概率：log(p(cid/title))=%f\n",rcid,pmax);
	hash_table_release();
	hash_table1_release();
}
*/
