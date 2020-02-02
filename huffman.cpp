#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define error 0

typedef char ElemType;
typedef char** HFMCode;

typedef struct {				//保存文件里的数据和其对应的序号 还有权重
	float weight;
	char data;
	float rate;
}Num;

typedef struct {
	char data;
	int flag;
}text;

typedef struct{
	int parent;
	int weight;
	int lchild,rchild;  
}HFMNode,*HFMTree;

int OpenText(text *b,char *filename){
	int i=1;
	FILE *fp;
	fp=fopen(filename,"r");
	if(fp==NULL){
		printf("Read the file failed!\n");
		return error;
	}
	while(!feof(fp)){
		fscanf(fp,"%c",&b[i]);    //把文件的数据存在链表中
		b[i].flag=0;
		++i;
	}
	--i;
	fclose(fp);
	fp=NULL;
	printf("Read the file successed!\n\n");
 	return i;
}

int Rate(text b[3000],Num *l,int a){
	int i,j,t,z;
	z=0;
	for(i=1;i<a;++i){
		if(b[i].flag==0){
			++z;
			l[z].data=b[i].data;
			l[z].weight=1;
			j=i;
			while(j<a){
				++j;
				if(l[z].data==b[j].data){
					++l[z].weight;
					b[j].flag=1;
				}
			}
		}
	}
	for(t=1;t<=z;++t){
		l[t].rate=l[t].weight/a;
		printf("The rate of %c is %f, the times in article is %.f\n",l[t].data,l[t].rate,l[t].weight);
	}
	return z;
}

void CreateHFMCode(HFMTree &HT,	HFMCode &HC,int n,int m){
	int i,start,f,c;
	char * bc;
	HC=(HFMCode)malloc(sizeof(char*)*(n+1));
	bc=(char*)malloc(n*sizeof(char));									//临时储存哈夫曼编码的空间
	bc[n-1]='\0';
	for(i=1;i<=n;++i){																	//第四个！
		start=n-1;
		for(c=i,f=HT[i].parent;f!=0;c=f,f=HT[f].parent){
			if(HT[f].lchild==c){
				bc[--start]='0';
			}
			else{
				bc[--start]='1';
			}
		}
		HC[i]=(char*)malloc(sizeof(char)*(n-start));				//n-start就是一个哈夫曼编码占用的空间 不浪费其他空间
		strcpy(HC[i],&bc[start]);									//为什么是start-1
	}
	free(bc);
}

int Coding(text b[3000],Num l[3000],HFMTree HT,HFMCode HC,int a,char code[3000],int n){
	int i,j;
	code[0]=0;
	for(i=1;i<=a;++i){
		for(j=1;j<=n;j++){
			if(b[i].data==l[j].data){
				strcat(code,HC[j]);
				break;
			}
		}
	}
	return strlen(code);
}

void Select(int &s1,int &s2,HFMTree &HT,int t){
	int i,z;										//z是临时数据 用于保存交换的数据 
	s1=0;
	s2=0;
	HT[0].weight=3000;
	for(i=1;i<=t;++i){
		if(HT[i].weight<HT[s2].weight&&HT[i].parent==0){
			s2=i;
			if(HT[s2].weight<HT[s1].weight){						//s1最小
				z=s2;
				s2=s1;
				s1=z;
			}
		}		
	}
}


/*int CountWeight(LinkList &L,Num *a){        //计算文件中各字符的频率 变成权重直接放入数组
	LinkList p;
	int w;
	int i=1;
	char j;
	p=(LinkList)malloc(sizeof(LNode));
	p=NULL;
	printf("!");
	while(L->data!=NULL){
		w=1;
		p=L->next;
		j=p->data;									//j为判断 判断后面有没有相同的字符       这里!!!!!!!!!
		while(p->data!=NULL){
			if(j==p->next->data){					//如果有相同的字符 权重++ 把它从链表中删掉
				++w;
				p->next=p->next->next;
			}		
			p=p->next;
		}
		L->next=L->next->next;						//删除刚刚记录的字符 
	 	a[i].data=j;
		a[i].weight=w;
/*		HT[i].chara=j;								//把记录的字符和权重放在树里
		HT[i].lchild=0;
		HT[i].rchild=0;
		HT[i].parent=0;
		HT[i].weight=w;
		++i;										//记录有几个不同的字符
	}
	printf("?");
	--i;
	return i;
}*/

void CreateHFMTree(HFMTree &HT,int &m,int &n,Num l[]){
	int i,s1,s2;
//	n=CountWeight(L,a);                                         //统计前n个字符的数量和权重，把权重放入数组a中
	m=n*2-1;
	HT=(HFMTree)malloc(sizeof(HFMNode)*(m+1));									//??????????
	for(i=1;i<=n;++i){										//把前n个权重放入树中
		HT[i].lchild=0;
		HT[i].rchild=0;
		HT[i].parent=0;
//		HT[i].weight=a[i].weight;
		HT[i].weight=l[i].weight;
	}
	for(i=n+1;i<=m;++i){									  //把后面的空间全部置空
		HT[i].lchild=0;
		HT[i].rchild=0;
		HT[i].parent=0;
		HT[i].weight=0;
	}
	for(i=n+1;i<=m;++i){
		Select(s1,s2,HT,i-1);
		HT[s1].parent=i;
		HT[s2].parent=i;
		HT[i].lchild=s1;
		HT[i].rchild=s2;
		HT[i].weight=HT[s1].weight+HT[s2].weight;
	}
}

void CodePrintf(Num l[],HFMCode HC,int a,int n,char *code,int length){
	int i,j,t;
	//for(i=1,t=1;i<=n;++i){
	//	for(j=0;HC[i][j]!='\0';++j){
	//		code[t++]=HC[i][j];
	//	}	
	//}
	//--t;
	printf("\n");
	for(i=1;i<=n;++i){
		printf("The Haffman code of %c is %s\n",l[i].data,HC[i]);
	}
	i=0;
	printf("\nThe haffman code is ");
	while(i<=length){
		printf("%c",code[i]);
		++i;
	}
	printf("\n\n");
}

void DeCoding(HFMTree HT,char code[3000],int n,int length,Num l[3000]){
	int i,m;
//	HFMNode p;
//	p=HT[m];
	for(i=0;i<length;){
		m=n*2-1;	
		while(HT[m].lchild||HT[m].rchild){
			if(code[i]=='0'){
				m=HT[m].lchild;
			}
			else if(code[i]=='1'){
				m=HT[m].rchild;
			}
			++i;
		}
		printf("%c",l[m].data);
	}
	printf("\n");
}

int zip(Num l[3000],char code[3000],int length,int n){
	int i,j,t,asc,count,z[8],nzone;							//	asc是code字符串中每8个对应的数字 z是临时存放权重的变量 
	char print[8],c[3000];								//code和print还有c数组都是以0开始的 c是压缩后的字符数组
	FILE *fp;
	fp=fopen("./text.huf","w");
	i=0;
	asc=0;
	count=0;
	nzone=0;
	t=1;
	while(t<=n){
		c[count++]=l[t].data;
		c[count++]=l[t].weight-33+'!';									//把权重转为一个字符
		++t;
	}
	c[count++]=252;
	while(i<length){
		for(j=0;j<8&&i<length;++j,++i){
			print[j]=code[i];
			asc+=(print[j]-48)*(pow(2,(7-j)));
		}
		++nzone;
		if(i>=length){
			while(j<8){
				print[j]='0';
				++j;
			}
		}
		c[count++]=asc-33+'!';
	}
	++nzone;
	i=0;
	while(i<count){
		fprintf(fp,"%c",c[i]);
		++i;
	}
	fclose(fp);
	return nzone;
}

void unzip(char *filename){
	HFMTree ST;
	float total;
	int i,j,m,t,z;										//j代表叶子有多少 m是树的总结点
	Num l[3000];
	text a[3000];
	char textcode[3000]={'\0'};
	char putin[9]={'\0'};
	t=0;
	j=1;
	total=OpenText(a,filename);
	for(i=1;a[i].data!=(char)252;++i){
		if(i%2==1){
			l[j].data=a[i].data;
		}
		else{
			l[j].weight=a[i].data;
			++j;
		}
	}
	--j;
	++i;										//跳过中间的分隔符号
	CreateHFMTree(ST,m,j,l);					//利用保存的信息新建一个树（和原来的一样
	z=0;
	while(a[i].data){
		for(t=1;z<8;++z){
			putin[z]=(int)a[i].data/pow(2,7-z)+48;
			a[i].data=a[i].data%(int)pow(2,7-z);
			++t;
		}
		strcat(textcode,putin);
	}
	--t;
	DeCoding(ST,textcode,j,t,l);
}

void efficient(int zone,int nzone){
	float n;
	n=(float)nzone/zone;
	printf("%f\n",n);
}

void main(){
	HFMCode HC;
	char code[3000];
	int n,m,end,length,nzone;							//end是记录code[]中有几个编码（编码的结束位置
	float a;
	HFMTree HT;
	Num l[3000];
	text b[3000];
	a=OpenText(b,"./text.txt");						//这里的a只统计有多少个字符 
	n=Rate(b,l,a);							//n代表字符的种类
	printf("\n获得频率成功!\n");
	CreateHFMTree(HT,m,n,l);
	printf("\n创建哈夫曼树成功!\n");
	CreateHFMCode(HT,HC,n,m);
	length=Coding(b,l,HT,HC,a,code,n);
  	CodePrintf(l,HC,a,n,code,length);
	DeCoding(HT,code,n,length,l);	
	nzone=zip(l,code,length,n);
	unzip("./text.huf");
	efficient(a,nzone);
}
