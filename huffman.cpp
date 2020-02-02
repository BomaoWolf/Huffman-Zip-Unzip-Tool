#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define error 0

typedef char ElemType;
typedef char** HFMCode;

typedef struct {				//�����ļ�������ݺ����Ӧ����� ����Ȩ��
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
		fscanf(fp,"%c",&b[i]);    //���ļ������ݴ���������
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
	bc=(char*)malloc(n*sizeof(char));									//��ʱ�������������Ŀռ�
	bc[n-1]='\0';
	for(i=1;i<=n;++i){																	//���ĸ���
		start=n-1;
		for(c=i,f=HT[i].parent;f!=0;c=f,f=HT[f].parent){
			if(HT[f].lchild==c){
				bc[--start]='0';
			}
			else{
				bc[--start]='1';
			}
		}
		HC[i]=(char*)malloc(sizeof(char)*(n-start));				//n-start����һ������������ռ�õĿռ� ���˷������ռ�
		strcpy(HC[i],&bc[start]);									//Ϊʲô��start-1
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
	int i,z;										//z����ʱ���� ���ڱ��潻�������� 
	s1=0;
	s2=0;
	HT[0].weight=3000;
	for(i=1;i<=t;++i){
		if(HT[i].weight<HT[s2].weight&&HT[i].parent==0){
			s2=i;
			if(HT[s2].weight<HT[s1].weight){						//s1��С
				z=s2;
				s2=s1;
				s1=z;
			}
		}		
	}
}


/*int CountWeight(LinkList &L,Num *a){        //�����ļ��и��ַ���Ƶ�� ���Ȩ��ֱ�ӷ�������
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
		j=p->data;									//jΪ�ж� �жϺ�����û����ͬ���ַ�       ����!!!!!!!!!
		while(p->data!=NULL){
			if(j==p->next->data){					//�������ͬ���ַ� Ȩ��++ ������������ɾ��
				++w;
				p->next=p->next->next;
			}		
			p=p->next;
		}
		L->next=L->next->next;						//ɾ���ոռ�¼���ַ� 
	 	a[i].data=j;
		a[i].weight=w;
/*		HT[i].chara=j;								//�Ѽ�¼���ַ���Ȩ�ط�������
		HT[i].lchild=0;
		HT[i].rchild=0;
		HT[i].parent=0;
		HT[i].weight=w;
		++i;										//��¼�м�����ͬ���ַ�
	}
	printf("?");
	--i;
	return i;
}*/

void CreateHFMTree(HFMTree &HT,int &m,int &n,Num l[]){
	int i,s1,s2;
//	n=CountWeight(L,a);                                         //ͳ��ǰn���ַ���������Ȩ�أ���Ȩ�ط�������a��
	m=n*2-1;
	HT=(HFMTree)malloc(sizeof(HFMNode)*(m+1));									//??????????
	for(i=1;i<=n;++i){										//��ǰn��Ȩ�ط�������
		HT[i].lchild=0;
		HT[i].rchild=0;
		HT[i].parent=0;
//		HT[i].weight=a[i].weight;
		HT[i].weight=l[i].weight;
	}
	for(i=n+1;i<=m;++i){									  //�Ѻ���Ŀռ�ȫ���ÿ�
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
	int i,j,t,asc,count,z[8],nzone;							//	asc��code�ַ�����ÿ8����Ӧ������ z����ʱ���Ȩ�صı��� 
	char print[8],c[3000];								//code��print����c���鶼����0��ʼ�� c��ѹ������ַ�����
	FILE *fp;
	fp=fopen("./text.huf","w");
	i=0;
	asc=0;
	count=0;
	nzone=0;
	t=1;
	while(t<=n){
		c[count++]=l[t].data;
		c[count++]=l[t].weight-33+'!';									//��Ȩ��תΪһ���ַ�
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
	int i,j,m,t,z;										//j����Ҷ���ж��� m�������ܽ��
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
	++i;										//�����м�ķָ�����
	CreateHFMTree(ST,m,j,l);					//���ñ������Ϣ�½�һ��������ԭ����һ��
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
	int n,m,end,length,nzone;							//end�Ǽ�¼code[]���м������루����Ľ���λ��
	float a;
	HFMTree HT;
	Num l[3000];
	text b[3000];
	a=OpenText(b,"./text.txt");						//�����aֻͳ���ж��ٸ��ַ� 
	n=Rate(b,l,a);							//n�����ַ�������
	printf("\n���Ƶ�ʳɹ�!\n");
	CreateHFMTree(HT,m,n,l);
	printf("\n�������������ɹ�!\n");
	CreateHFMCode(HT,HC,n,m);
	length=Coding(b,l,HT,HC,a,code,n);
  	CodePrintf(l,HC,a,n,code,length);
	DeCoding(HT,code,n,length,l);	
	nzone=zip(l,code,length,n);
	unzip("./text.huf");
	efficient(a,nzone);
}
