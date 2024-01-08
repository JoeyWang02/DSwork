#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTEX_NUM 20
typedef int InfoType;
typedef int VertexType;

//���Ľṹ
typedef struct ArcNode{
    int adjvex;//����ͷ��β�Ķ������ڵ�λ��
    struct ArcNode *nextarc;//ָ����һ������ָ��
    int visited;//���ķ��ʴ���
    //InfoType *Info;//�û������Ϣ��ָ��
}ArcNode;
//����Ľ��ṹ
typedef struct VNode{
    VertexType data;//������Ϣ
    ArcNode *firstarc;//ָ���һ�������ö���Ļ�
    int degree;//�ڵ����
}VNode,AdjList[MAX_VERTEX_NUM];
//ͼ�Ľṹ����
typedef struct{
    AdjList vertices;//ͷ�������
    int vexnum,arcnum;//ͼ��ǰ�Ķ������ͱ���
}ALGraph;

//ȡ��ַ
int LocateVex(ALGraph *G,int v);
//������ͼ
void CreatUDG(ALGraph *G);
//��ӡͼ
void PrintGraph(ALGraph G);
//��ͼG��������ȱ���
void DFS(ALGraph *G, int v);
//�ж�ŷ����·
int IfEuler(ALGraph *G);
//��ӡŷ����·
void PrintEuler(ALGraph *G);

//��������
//ȡ��ַ
int LocateVex(ALGraph *G,int v){
    int i;
    for(i=0;i<G->vexnum;i++)
        if(G->vertices[i].data==v)
            return i;
}
//������ͼ
void CreatUDG(ALGraph *G){
    int i,j,k,m,n;
    ArcNode *s,*t;
    printf("Enter the number of vertices and edges:\n");
    scanf("%d%d",&G->vexnum,&G->arcnum);
    getchar;//�س�
    for(i=0;i<G->vexnum;i++){//�γɶ���
        G->vertices[i].data=i;
        G->vertices[i].firstarc=NULL;
        G->vertices[i].degree=0;
    }
    printf("Enter Edges(vi vj):\n");
    for(k=0;k<G->arcnum;k++){
        //printf("Enter Edges(vi vj):");
        scanf("%d %d",&i,&j);
        m=LocateVex(G,i); // �ҵ���������������е�����ֵ��ֱ��ʹ��i��j �������Ϊ֮��Ĳ���Ͳ��Ҷ���ʹ��Adjlist���� 
        n=LocateVex(G,j);
        s=(ArcNode*)malloc(sizeof(ArcNode));//ͷ�巨  
        s->adjvex=j;
        s->visited=0;
        s->nextarc=G->vertices[m].firstarc;
        G->vertices[m].firstarc=s;
        G->vertices[m].degree++;
        t=(ArcNode*)malloc(sizeof(ArcNode));
        t->adjvex=i;
        t->visited=0;
        t->nextarc=G->vertices[n].firstarc;
        G->vertices[n].firstarc=t;
        G->vertices[n].degree++;
        
    }
}

//��ӡͼ
void PrintGraph(ALGraph G){
    printf("Graph:\n");
    for(int i=0;i<G.vexnum;i++){
        printf("  V%d",G.vertices[i].data);
        ArcNode *p=G.vertices[i].firstarc;
        while(p){
            printf("->V%d",G.vertices[p->adjvex].data);
            p=p->nextarc;
        }
        printf("\n");
    }
}

///�� //��ͼG��������ȱ���
void DFS(ALGraph *G, int v){
    int w;
    ArcNode *p;
    printf("%d ", G->vertices[v].data);
    p = G->vertices[v].firstarc;
    while(p){
        w = p->adjvex;
        //printf("w=%d",w);
        if(p->visited==0){
            p->visited=1;
            ArcNode *q=G->vertices[w].firstarc;
            while(q){
                if(q->adjvex==v) q->visited=1;
                q=q->nextarc;
            }
            DFS(G, w);
        }
        //printf("\n");
        p = p->nextarc;
    }
}

//�ж�ŷ����·
int IfEuler(ALGraph *G){//ͼ��ͨ�����ж���Ķȶ���ż��
    int flag=1;
    for(int i=0;i<G->vexnum;i++){
        if(G->vertices[i].degree%2!=0){
            flag=0;
            break;
        }
    }
    if(flag&&G->vexnum>0){
        printf("An Euler circuit.\n");
    }
    else{
        printf("Not an Euler circuit.\n");
        return 0;
    }
    return 1;
}

//��ӡŷ����·
void PrintEuler(ALGraph *G){
    printf("The Euler circuit of graph is:");
    DFS(G,0);
}

int main(){
    ALGraph G;
    CreatUDG(&G);
    PrintGraph(G);
    if(IfEuler(&G)==1) PrintEuler(&G);
    return 0;
}
