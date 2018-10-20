#include<windows.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>
#include<time.h>
#include<stdlib.h>
#define Disk_Size 1023 
#define Inode_Num 100
#define Block_Maxnum 300 
#define Text_Maxsize 3000
#define Child_Maxnum 6
int inode_bitmap[Inode_Num];         //Inodeλͼ
int block_bitmap[Block_Maxnum];         //Blockλͼ
struct super_block{
    int free_inode;                   //����Inode��Ŀ
    int free_block;                   //����Block��Ŀ
} super_block;
typedef struct Inode{
    int isFileOrNot;         //�ж����ļ�����Ŀ¼,4byte;0ΪĿ¼,1Ϊ�ļ�
    char name[26];                            //�ļ���Ŀ¼����26byte
    int block_num;              //��ռ�õĴ��̿�����4byte
    int block_pos[7];                               //ռ�õĴ��̿��ַ��7*4=28byte
    int index;                           //��inode���еı�ţ�4byte
    int parent;                                //�������ڵ㣬4byte
    int child_num;                             //�ӽڵ������4byte
    int child[Child_Maxnum];                        //�������ڵ㣬6*4=24byte
    char ctime[26];                        //�ļ�����ʱ��,26byte
    int role                              //�ļ������ߵĽ�ɫ 
} inode;
typedef struct block{
    char content[Disk_Size+1];
} block;
int curr;                                   //��ǰ����ֵ
char current[512];                           //��ǰ·��
int current_num[128];                         //��ǰ����ֵ·��
int level=1;                                  //��ǰ�������
time_t t;
int admin;                                    //��ǰ�û�Ȩ�� 
int block_used=0;
int inode_used=0;
bool init (inode **iNodeList);
bool create_inode(char *name, inode **iNodeList, int seq);
bool input(char *text, block **blockList,inode **iNodeList, int seq);
void destroy(char *name,inode **iNodeList);
void destroy_file(int num, inode **iNodeList);
void remove_array(int *a, int amount, int num);
void destroy_folder(int num, inode **iNodeList);                          //��ǰ·��
void path_update(inode *iNodeList);
int open(inode *iNodeList, char *name);
void show(inode *iNodeList,block *blockList,int n);
void back(inode *iNodeList);
void menu(void);
int main(){
    inode *iNodeList = NULL;
    block *blockList=NULL;
    int option;
	init(&iNodeList);
	printf("\n\n");
    printf("                file_system \n");
    printf("------------------------------------------------------\n");
	printf(" Please choose your role :(0:admin  1:user)\n");
	scanf("%d",&admin); 
	system("cls");
    do{
    	
        printf("        Please input your option:");
        menu();
        printf("��ǰ·����%s  ",current);
        if(admin) printf("�û���ɫ��user\n");
        else printf("�û���ɫ��administrator\n");
        printf("ѡ�������"); 
        scanf("%d", &option);
        getchar();
        printf("\n");
        switch(option){
	        case 0:
	            break;
	        case 1:
				printf(" Please choose your role :(0:admin  1:user)\n");
				scanf("%d",&admin); 
				break;
	        case 2:{
	            int i;
	            if(super_block.free_inode == 0 ){
	                printf("Not enough storage space!\n");
	                break;
	            }
	            if(iNodeList[curr].child_num >= Child_Maxnum){
	                printf("Subfiles is full!\n");
	                break;
	            }
	            for(i = 0; i < Inode_Num; i++){
	                if(!inode_bitmap[i] ){
	                    char folder_name[26], text[Text_Maxsize];
	                    printf("Please input the name of folder:");
	                    scanf("%s", &folder_name);
	                    getchar();
	                    if(create_inode(&folder_name, &iNodeList, i)){
	                        iNodeList[i].isFileOrNot = 0;
	                        iNodeList[i].index = i;
	                        iNodeList[i].role = admin;
	                        iNodeList[curr].child[iNodeList[curr].child_num] = i;
	                        iNodeList[curr].child_num++;
	                        iNodeList[i].child_num = 0;
	                        super_block.free_inode--;
	                        inode_bitmap[i] = 1;
	                        printf("Creating Success!\n");
	                        break;
	                    }
	                    else
	                        break;
	                }
	            }
	            break;
	        }
	        case 3:{//create file
	            int i;
	            if(super_block.free_inode == 0 && super_block.free_block == 0){
	                printf("Not enough storage space!\n");
	                break;
	            }
	            if(iNodeList[curr].child_num >= Child_Maxnum){
	                printf("Subfiles is full!\n");
	                break;
	            }
	            for(i = 0; i < Inode_Num; i++){
	                if(!inode_bitmap[i])
	                {
	                    char file_name[28], text[Text_Maxsize];
	                    printf("Please input the name of file:");
	                    scanf("%s", &file_name);
	                    getchar();
	                    if(create_inode(&file_name, &iNodeList, i)){
	                        iNodeList[i].isFileOrNot = 1;
	                        iNodeList[i].index = i;
	                        iNodeList[i].role = admin;
	                        iNodeList[curr].child[iNodeList[curr].child_num] = i;
	                        iNodeList[curr].child_num++;
	                        super_block.free_inode--;
	                        inode_bitmap[i] = 1;
	                    }
	                    else{
	                    	break;
						}
	                        
	                    printf("Please input text: (End by $)\n");
	                    char c=0;int cnt=0;
	                    while(c!='$'){
	                    	scanf("%c",&c);
	                    	if(c=='$') break;
	                    	text[cnt++]=c;
						}
	                    text[cnt]='\0';
	                    getchar();
	                    printf("\n");
	                    if(input(text, &blockList, &iNodeList,i))
	                        printf("Creating Success!\n");
	                    else
	                        printf("Failed to create!\n");
	                    break;
	                }
	            }
	            break;
	        }
	        case 4:{//���ļ���
	            char name[28],text[3000];
	            int n;
	            printf("Please input name:");
	            scanf("%s",name);
	            getchar();
	            printf("\n");
	            n=open(iNodeList,name);
	            if(n==-1||iNodeList[n].isFileOrNot==1){
	                printf("folder does not exist!\n");
	                break;
	            }
	            if(iNodeList[n].role<admin){
	            	printf("permission denied\n");
	            	break;
				}
	            show(iNodeList,blockList,n);
	            printf("Open success!\n");
	            break;
	        }
	        case 5:{//���ļ�
	            char name[28],text[Text_Maxsize];;
	            int n;
	            printf("Please input name:");
	            scanf("%s",name);
	            getchar();
	            printf("\n");
	            n=open(iNodeList,name);
	            if(n==-1||iNodeList[n].isFileOrNot==0){
	                printf("File does not exist!\n");
	                break;
	            }
	            show(iNodeList,blockList,n);
	            printf("Open success!\n");
	            if(iNodeList[n].role<admin){
	            	printf("Not enough privileges to write!\n");
	            	break;
				}
	            printf("Update the text or not ?(y/n)");
	            char s[2];
	            scanf("%s",s);
	            if(s[0]=='y'||s[0]=='Y'){
	            	printf("Please input text: (End by $)\n");
	                    char c=0;int cnt=0;
	                    while(c!='$'){
	                    	scanf("%c",&c);
	                    	if(c=='$') break;
	                    	text[cnt++]=c;
						}
	                    text[cnt]='\0';
	                    getchar();
	                    printf("\n");
	                    if(input(text, &blockList, &iNodeList,n))
	                        printf("Update Success!\n");
	                    else
	                        printf("Failed to Update!\n");
	                    break;
				}
				else getchar();
	            break;
	        }
	        case 6://������һ��
	            back(iNodeList);
	            break;
	        case 7:
	            list(iNodeList);
	            break;
	        case 8:{//ɾ��
	            char name[28];
	            printf("Please input name:");
	            scanf("%s",name);
	            getchar();
	            destroy(name,&iNodeList);
	            printf("Delete success!\n");
	            break;
	        }
	        case 9:{
	            int i;
	            for(i=0; i<Inode_Num; i++){
	                if(!inode_bitmap[i])
	                    printf("-");
	                else
	                    printf("0");
	            }
	            break;
	        }
	        case 10:{
	            int i;
	            for(i=0; i<Block_Maxnum; i++){
	                if(!block_bitmap[i])
	                    printf("-");
	                else
	                    printf("0");
	            }
	            break;
	        }
	        case 11:{
	            char name[28];
	            int n;
	            printf("Please input name:");
	            scanf("%s",name);
	            getchar();
	            printf("\n");
	            n=open(iNodeList,name);
	            if(n==-1){
	                printf("File does not exist!\n");
	                break;
	            }
	            if(!iNodeList[n].isFileOrNot)
	                printf("name:%s\ncreation time:%s\n",iNodeList[n].name,iNodeList[n].ctime);
	            else
	                printf("name:%s\ncreation time:%snsize:%dKB\n",iNodeList[n].name,iNodeList[n].ctime,iNodeList[n].block_num);
	            break;
	        }
        }
        getchar();
        system("cls");
    }
    while(option != 0);
    printf("\n--------------------Welcome again!--------------------\n");
}
/*��ʼ�������ļ�ϵͳ*/
bool init (inode **iNodeList){
    super_block.free_inode = Inode_Num;
    super_block.free_block = Block_Maxnum;
    int i;
    for(i = 0; i < Inode_Num; i++)
        inode_bitmap[i] = 0;
    for(i = 0; i < Block_Maxnum; i++)
        block_bitmap[i] = 0;
    inode_bitmap[0] = 1;
    curr = 0;
    strcpy(current, "Root//"); 
    current_num[0] = 0;
    *iNodeList=(inode *)malloc(sizeof(inode));
    (*iNodeList)->isFileOrNot = 0;
    strcpy((*iNodeList)->name, "Root");
    (*iNodeList)->index = curr;
    (*iNodeList)->parent = curr;
    (*iNodeList)->child_num=0;
    time(&t);
    strcpy((*iNodeList)->ctime,ctime(&t));
    return true;
}
/*�ڵ�ǰ·�����½�һ���ļ��л��ļ������������еĲ����ظ�*/ 
bool create_inode(char *name, inode **iNodeList, int seq){
    int i;
    if(strlen(name) > 28){
        printf("File name is too long!\n");
        return false;
    }
    for(i = 0; i < ((*iNodeList)[curr].child_num); i++){
        if(!strcmp(name, (*iNodeList)[(*iNodeList)[curr].child[i]].name)){	
            printf("iNodeListhe same file name!\n");
        	return false;
        }
    }
    if(seq>=inode_used)
    {
        *iNodeList=(inode *)realloc(*iNodeList,(seq+1)*sizeof(inode));
        inode_used=seq;
    }
    strcpy((*iNodeList)[seq].name , name);
    (*iNodeList)[seq].parent = curr;
    time(&t);
    strcpy((*iNodeList)[seq].ctime,ctime(&t));
    return true;
}
/*Ϊһ���Ѿ��������ļ�д�����ݣ�ע������һ���ļ����ռ��7�����п�*/ 
bool input(char *text, block **blockList, inode **iNodeList,int seq){
    if(strlen(text) >= 7 * Disk_Size)
    {
        printf("File size is too large!\n");
        return false;
    }
    int m = strlen(text) / Disk_Size ;
    int n = strlen(text) % Disk_Size ;
    (*iNodeList)[seq].block_num = m + 1;
    int j, w = 0;
    for(j = 0; j <= m ; j++){
        if(super_block.free_block == 0){
            printf("Not enough storage space!\n");
            return false;
        }
        do{
            if(!block_bitmap[w]){
                if(w>=block_used){
                    *blockList=(block *)realloc(*blockList,(w+1)*sizeof(block));
                    block_used=w;
                }
                if(j < m){
                    strncpy((*blockList+w)->content, text + j  * (Disk_Size), Disk_Size );
                    ((*blockList+w)->content)[Disk_Size]='\0';
                }
                else if(j == m){
                    strncpy((*blockList+w)->content, text + j  * (Disk_Size) , n );
                    ((*blockList+w)->content)[n]='\0';
                }
                (*iNodeList)[seq].block_pos[j] = w;
                super_block.free_block--;
                block_bitmap[w] = 1;
                break;
            }
            w++;
        }
        while(w < Block_Maxnum);
    }
    return true;
}
/*������ɾ��һ���ļ����ļ���*/ 
void destroy(char *name,inode **iNodeList){
    int i=(*iNodeList+curr)->child_num;
    while(i>0){
    	
        if(!strcmp(name,(*iNodeList)[(*iNodeList)[curr].child[i-1]].name)){
            if(!(*iNodeList)[(*iNodeList)[curr].child[i-1]].isFileOrNot)
                destroy_folder((*iNodeList)[curr].child[i-1],iNodeList);
            else
                destroy_file((*iNodeList)[curr].child[i-1],iNodeList);
            return;
        }
        i--;
    }
    printf("File not exist!\n");
}
/*ɾ��һ���ļ� 
*/ 
void destroy_file(int num, inode **iNodeList){
    inode_bitmap[num] = 0;
    int i = (*iNodeList)[num].block_num - 1;
    do{
        block_bitmap[(*iNodeList)[num].block_pos[i]] = 0;
        i--;
    }while(i > 0);
    remove_array((*iNodeList)[(*iNodeList)[num].parent].child, (*iNodeList)[(*iNodeList)[num].parent].child_num, num);
	(*iNodeList)[(*iNodeList)[num].parent].child_num--;
}
/*�ڸ�i�ڵ�ĺ����б���ɾ��num����ڵ�*/ 
void remove_array(int *a, int amount, int num){
    int i;
    for(i = 0; i <= amount - 1; i++){
        if(a[i] == num){
            do{
                a[i] = a[i + 1];
                i++;
            }
            while(i <= amount - 2);
        }
    }
}
/*�ݹ�ɾ��һ���ļ��м����������ļ� 
*/ 
void destroy_folder(int num, inode **iNodeList){
    inode_bitmap[num] = 0;
    (*iNodeList)[(*iNodeList)[num].parent].child_num--;
    remove_array((*iNodeList)[(*iNodeList)[num].parent].child, (*iNodeList)[(*iNodeList)[num].parent].child_num, num);
    if((*iNodeList)[num].isFileOrNot == 1){
        destroy_file(num, (*iNodeList));
    }
    int i = (*iNodeList)[num].child_num - 1;
    while(i>=0){
        destroy_folder((*iNodeList)[num].child[i], iNodeList);
        i--;
    }
}
/*���µ�ǰ·�� 
*/ 
void path_update(inode *iNodeList){
    int i = 0;
    memset(current, 0, sizeof (char) * 512);
    do{
        strcat(current, iNodeList[current_num[i]].name);
        strcat(current, "/");
        i++;
    }
    while(i<level);
}
/*�����ִ�һ���ļ���Ŀ¼������i�ڵ���
 �������ֲ����ڷ���-1 
*/ 
int open(inode *iNodeList, char *name){
    int i, n;
    for(i = 0; i < iNodeList[curr].child_num; i++){
        if(!strcmp(iNodeList[iNodeList[curr].child[i]].name, name)){
            n = iNodeList[curr].child[i];
            return n;
        }
    }
    return -1;
}
/*����һ��Ŀ¼������Ŀ¼������һ���ļ������ʾ���ļ����ݡ�
  n��Ҫ������i�ڵ��� 
*/
void show(inode *iNodeList,block *blockList,int n){
    int i;
    if(iNodeList[n].isFileOrNot==0){
        curr=n;
        current_num[level]=n;
        ++level;
        path_update(iNodeList);
    }
    if(iNodeList[n].isFileOrNot==1)
        for(i=0; i<iNodeList[n].block_num; i++)
            printf("%s\n",blockList[iNodeList[n].block_pos[i]].content);
}
/*������һ��Ŀ¼ 
*/ 
void back(inode *iNodeList){
    curr=iNodeList[curr].parent;
    level--;
    path_update(iNodeList);
}
/*�г���ǰĿ¼�����е��ļ��к��ļ��� 
*/ 
void list(inode *iNodeList){
    char file[1000]=" ";
    char folder[1000]=" ";
    int i=iNodeList[curr].child_num;
    if(i==0){
        printf("Empty!\n");
        return;
    }
    i--;
    do{
        if(iNodeList[iNodeList[curr].child[i]].isFileOrNot){
            strcat(file,iNodeList[iNodeList[curr].child[i]].name);
            strcat(file,"\t");
        }
        else{
            strcat(folder,iNodeList[iNodeList[curr].child[i]].name);
            strcat(folder,"\t");
        }
        i--;
    }
    while(i>=0);
    printf("folder:%s\nfile:%s",folder,file);
}
/*��ʾ���ܲ˵��ļ� 
*/ 
void menu(){
    printf("\n\n");
    printf("Menu for file_system \n");
    printf("------------------------------------------------------\n");
    printf("                  1. change role  \n");
    printf("2. create new folder          3. create new file\n");
    printf("4. open folder                5.open file\n");
    printf("              6. back            \n");
    printf("7. list                       8. delete\n");
    printf("9. inode graph                10.block graph\n");
    printf("              11. detail\n");
    printf("0. Exit\n");
    printf("------------------------------------------------------\n");
}
 
