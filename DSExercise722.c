#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTEX_NUM 20
typedef int InfoType;
typedef int VertexType;

//弧的结构
typedef struct ArcNode{
    int adjvex;//弧的头或尾的顶点所在的位置
    struct ArcNode *nextarc;//指向下一个弧的指针
    int visited;//弧的访问次数
    //InfoType *Info;//该弧相关信息的指针
}ArcNode;
//顶点的结点结构
typedef struct VNode{
    VertexType data;//顶点信息
    ArcNode *firstarc;//指向第一条依附该顶点的弧
    int degree;//节点次数
}VNode,AdjList[MAX_VERTEX_NUM];
//图的结构定义
typedef struct{
    AdjList vertices;//头结点数组
    int vexnum,arcnum;//图当前的顶点数和边数
}ALGraph;

//取地址
int LocateVex(ALGraph *G,int v);
//建无向图
void CreatUDG(ALGraph *G);
//打印图
void PrintGraph(ALGraph G);
//对图G做深度优先遍历
void DFS(ALGraph *G, int v);
//判断欧拉回路
int IfEuler(ALGraph *G);
//打印欧拉回路
void PrintEuler(ALGraph *G);

//基本操作
//取地址
int LocateVex(ALGraph *G,int v){
    int i;
    for(i=0;i<G->vexnum;i++)
        if(G->vertices[i].data==v)
            return i;
}
//建无向图
void CreatUDG(ALGraph *G){
    int i,j,k,m,n;
    ArcNode *s,*t;
    printf("Enter the number of vertices and edges:\n");
    scanf("%d%d",&G->vexnum,&G->arcnum);
    getchar;//回车
    for(i=0;i<G->vexnum;i++){//形成顶点
        G->vertices[i].data=i;
        G->vertices[i].firstarc=NULL;
        G->vertices[i].degree=0;
    }
    printf("Enter Edges(vi vj):\n");
    for(k=0;k<G->arcnum;k++){
        //printf("Enter Edges(vi vj):");
        scanf("%d %d",&i,&j);
        m=LocateVex(G,i); // 找到输入结点的在数组中的索引值。直接使用i，j 会出错。因为之后的插入和查找都是使用Adjlist数组 
        n=LocateVex(G,j);
        s=(ArcNode*)malloc(sizeof(ArcNode));//头插法  
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

//打印图
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

///改 //对图G做深度优先遍历
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

//判断欧拉回路
int IfEuler(ALGraph *G){//图连通且所有顶点的度都是偶数
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

//打印欧拉回路
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
