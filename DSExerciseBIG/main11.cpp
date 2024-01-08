#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 999999
#define MAXSIZE 50
//typedef enum
//{
//    false,
//    true
//} bool;

//登陆
void login();

//游客模式
void vistor();

//管理员登陆
void adminLogin();

//管理员模式
void admin();

//首次使用程序的初始化
void init();

//增加场所
void addPlace();

//删除场所
void delPlace();

//添加道路
void addPath();

//删除道路
void delPath();

//修改场所信息
void modPlace();

//查看全部场所
void showInfo();

//求两景点最短距离
void Floyd(int start, int end);

//查看某场所详细介绍
void inquiryInfo(int n);

//安全地退出
void quit();

//读取地图
void readMap();

//保存地图
void rewriteMap();

typedef struct vertex Vertex;
struct vertex
{
    char name[100];
    //char info[1000];
};
typedef int Edge;
struct map
{
    int placeNum;
    int pathNum;
    vertex place[100];
    Edge path[100][100];
};

struct map ZJUMap;
int flag, a, b, num, temp;
bool isAdmin = false;
FILE *pMap;
/*
void writeFile()
{
    pMap = fopen("C:\\Coding\\Projects\\DSExerciseBIG\\distances.csv", "ab");
    if (pMap == NULL)
    {
        fputs("Can't open file \n", stderr);
        exit(1);
    }
    rewind(pMap);
    //将文件内部的位置 指针重新指向一个流（ 数据流/文件）的开头
    
    int size = sizeof(ZJUMap);
    fwrite(&ZJUMap, size, 1, pMap);
    fclose(pMap);
    printf("\n success renew!\n");
}*/

void writeFile(){
    FILE *fp;
    int i, j;
    fp = fopen("C:\\Coding\\Projects\\DSExerciseBIG\\distances.csv", "w"); // 打开CSV文件 = fopen("C:\\Coding\\Projects\\DSExerciseBIG\\distances.csv", "w");
    if(fp == NULL){
        printf("Cannot open file.\n");
        exit(1);
    }
    for(i=0;i<ZJUMap.placeNum;i++) fprintf(fp,",%s",ZJUMap.place[i].name);
    fprintf(fp, "\n");
    for (i = 0; i<ZJUMap.placeNum; i++) {
        for (j = 0; j<ZJUMap.placeNum-1; j++) {
            if(j==0) fprintf(fp, "%s,",ZJUMap.place[i].name);
            if(ZJUMap.path[i][j]>0&&ZJUMap.path[i][j]<INF) fprintf(fp, "%d,", ZJUMap.path[i][j]);
            else fprintf(fp,"0,");
        }
        if(ZJUMap.path[i][j]!=INF) fprintf(fp, "%d\n", ZJUMap.path[i][j]);
        else fprintf(fp, "0\n");
    }
    fclose(fp);
    printf("\n success renew!\n");
}

void readFile() {
    FILE* fp;
    char buffer[1024];
    char* token;
    int row_count = 0;
    int col_count = 0;
    int col=0;
    fp = fopen("C:\\Coding\\Projects\\DSExerciseBIG\\distances.csv", "r");
    if (fp == NULL) {
        printf("Failed to open file\n");
        exit(1);
    }
    while (fgets(buffer, 1024, fp) != NULL && row_count < 100) {
        col_count = 0;
        token = strtok(buffer, ",");
        while (token != NULL && col_count < 100) {
            if (row_count == 0) {
                strcpy(ZJUMap.place[col_count].name, token);
            }
            else if (col_count == 0) {
                strcpy(ZJUMap.place[row_count - 1].name, token);
            } 
            else {
                ZJUMap.path[row_count - 1][col_count - 1] = atoi(token);
                if(ZJUMap.path[row_count - 1][col_count - 1]==0)ZJUMap.path[row_count - 1][col_count - 1]=INF;
            }
            col_count++;
            token = strtok(NULL, ",");
        }
        if (row_count == 0) {
            ZJUMap.placeNum = col_count;
        } 
        else {
            ZJUMap.pathNum = col_count - 1;
        }
        row_count++;
    }
    fclose(fp);
}

void print_two_dimension()
{
	int i, j;
	for(i=1; i<ZJUMap.pathNum; i++){
		for(j=0; j<ZJUMap.pathNum; j++){
			printf("%4d", ZJUMap.path[i][j]);
		}
		printf("\n");
	}//打印的时候不打印第一行，第一行全是字符
}

void login()
{
    //printf("\033[1;4;30;43m"); // 设置字体为亮绿色背景红色，字体大小为2号字体
    printf("\033[1;30;43m\n\n                Zhejiang University Navigation System\n");
    printf("\033[0m"); // 恢复默认样式
    printf("\n\033[1;30;47m\n                *********************************** \n");
    printf("                *        1.Tourist login           *\n");
    printf("                *        2.Administrator login     *\n");
    printf("                *        3.Exit                    *\n");
    printf("                ************************************");
    printf("\033[0m\n"); // 恢复默认样式
    scanf("%d", &flag);
    switch (flag)
    {
        case 1:
        {
            vistor();
            break;
        }
        case 2:
        {
            if (isAdmin == true)
                admin();
            adminLogin();
            break;
        }
        case 3:
        {
            quit();
            break;
        }
        default:
        {
            printf("Input error, please enter again!\n");
            login();
            break;
        }
    }
}

void init()
{
    ZJUMap.placeNum = 28;
    ZJUMap.pathNum = 56;
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            ZJUMap.path[i][j] = INF;
        }
    }
    //writeFile();
}

void showInfo()
{
    printf("\033[1;30;47m\n**** Tourist attractions directory****\n");
    for (num = 0; num < ZJUMap.placeNum; num++)
        printf("%s[%d] %s\n", ZJUMap.place[num].name,num + 1, ZJUMap.place[num].name);
    printf("**************************************\033[0m\n");
}

void inquiryInfo(int n)
{
    printf("Number:%d  Name:%s\n", n, ZJUMap.place[n - 1].name);
}

void Floyd(int start, int end)
{
    printf("Origin:%s, Destination:%s\n", ZJUMap.place[start - 1].name, ZJUMap.place[end - 1].name);
    if (ZJUMap.placeNum <= 0)
    {
        printf("No tourist attractions found in the map!\n");
        return;
    }
    if (ZJUMap.placeNum == 1)
    {
        printf("There is only one attraction in the map, cannot search!\n");
        return;
    }
    if (ZJUMap.pathNum <= 0)
    {
        printf("No path found in the map!\n");
        return;
    }
    int e[100][100];//邻接矩阵
    int pre[100][100];//存路径
    int i, j, k;
    //初始化
    for (i = 0; i < ZJUMap.placeNum; i++)
        for (j = 0; j < ZJUMap.placeNum; j++)
        {
            e[i][j] = ZJUMap.path[i][j];
            pre[i][j] = j;
        }
    //更新路径
    for (k = 0; k < ZJUMap.placeNum; k++)
        for (i = 0; i < ZJUMap.placeNum; i++)
            for (j = 0; j < ZJUMap.placeNum; j++)
            {

                if (e[i][j] > e[i][k] + e[k][j])
                {
                    e[i][j] = e[i][k] + e[k][j];
                    pre[i][j] = pre[i][k];
                }
            }
    // 打印路径
    // 无穷大，无路径
    if (e[end - 1][start - 1] == INF)
    {
        printf("Cannot reach %s from %s\n", ZJUMap.place[end - 1].name, ZJUMap.place[start - 1].name);
    } else
    {
        printf("The shortest path distance from %s to %s is %d meters.\n", ZJUMap.place[start - 1].name, ZJUMap.place[end - 1].name,
               e[end - 1][start - 1]);
        printf("The path is:\n%s", ZJUMap.place[start - 1].name);
        if (pre[start - 1][end - 1] == end - 1)
        { // 如果可以直连
            printf("->%s", ZJUMap.place[end - 1].name);
        } else
        { //需要绕路
            temp = start - 1; //初始化temp为终点
            while (temp != end - 1)
            {
                printf("->%s", ZJUMap.place[pre[temp][end - 1]].name);
                temp = pre[temp][end - 1];
            }
        }
        printf("\n");
    }
}

void vistor() //进入导览
{
    printf("\033[1;30;47m\n==========Tourist================================\n"
           "        0.Return             \n"
           "        1.Search for routes within the campus.\n"
           "        2.Attraction information.\n"
           "        3.Exit\n"
           "=========Please enter the number================\033[0m\n");
    scanf("%d", &flag);
    switch (flag)
    {
        case 0:
        {
            login();
        }
        case 1:
        {
            int start, end;
            showInfo();
            printf("Please enter the starting location number.\n");
            scanf("%d", &start);
            printf(" Please enter the destination location number.\n");
            scanf("%d", &end); 
            if (start < 1 || start > ZJUMap.placeNum || end < 1 || end > ZJUMap.placeNum || start == end)
            {
                printf("Starting point/ending point does not exist or the starting point cannot be the same as the destination!");
                vistor();
            }
            Floyd(start, end);
            vistor();
            break;
        }
        case 2:
        {
            showInfo();
            printf("Please enter the attraction number.\n");
            scanf("%d", &num);
            inquiryInfo(num);
            vistor();
            break;
        }
        case 3:
        {
            quit();
            break;
        }
        default:
        {
            printf(" Input error, please enter again!\n");
            vistor();
        }
    }
}

void quit()  //退出
{
    printf("\033[1;30;47m\nSuccessfully exited, welcome to use Zhejiang University Navigation System again!");
    if (isAdmin == true)
        rewriteMap();
        printf("\n!!Map Saved!!\n");
    printf("\033[0m"); // 恢复默认样式
    exit(0);
}

void adminLogin()  //管理员登陆
{
    printf("\033[1;30;47m\n=========Administrator login======\n");
    printf("Please enter the administrator password:\n");
    printf("The password is 123456.\033[0m\n");
    char password[18];
    getPassword:
    scanf("%s", password);
    if (strcmp(password, "123456") == 0)
    {
        printf("L\n");
        isAdmin = true;
        admin();
    } else
    {
        printf("Incorrect password. \nPlease enter the password again:\n");
        goto getPassword;
    }
}

void admin()  //管理员编辑
{
    printf("\033[1;30;47m\n=========Administrator system.======\033[0m\n");
    showInfo();
    adminFlag:

    printf("\033[1;30;47m\n=======Please enter the number.=========\n"
           "1.Add place\n"
           "2.Delete place\n"
           "3.Add path\n"
           "4.Delete path\n"
           "5.Modify place information\n"
           "6.Enter the tourist system\n"
           "7.Exit\n"
           "=========================\033[0m\n");
    scanf("%d", &flag);
    switch (flag)
    {
        case 1:
        {
            addPlace();
            break;
        }
        case 2:
        {
            delPlace();
            break;
        }
        case 3:
        {
            addPath();
            break;
        }
        case 4:
        {
            delPath();
            break;
        }
        case 5:
        {
            modPlace();
            break;
        }

        case 6:
        {
            vistor();
            break;
        }
        case 7:
        {
            quit();
            break;
        }
        default:
        {
            printf("Incorrect input, please try again\n");
            goto adminFlag;
        }
    }
    
}

void addPlace()  //管理员编辑——增加地点
{
    if (ZJUMap.placeNum >= 100)
    {
        printf(" The space is full and cannot be added!\n");
        return;
    }
    showInfo();
    char newName[50];
    char newInfo[200];
    char zero[20];
    getchar();
    printf("Please enter the name of the attraction you want to add:\n");
    //scanf("%s", newName);
    fgets(newName,sizeof(newName),stdin);
    newName[strcspn(newName, "\n")] = '\0';
    //printf("newName=%s",newName);
    //newName[strcspn(newName, "\n")] = '\0';
    //printf("请输入%s的景点信息简介\n", newName);
    strcpy(ZJUMap.place[ZJUMap.placeNum].name, newName);
    ZJUMap.placeNum++;
    printf("Add succussfully!\n"
           "1.Continue adding attractions.\n"
           "2.Return to the previous screen.\n");
    scanf("%d", &flag);
    if (flag == 1)
        addPlace();
    admin();
}

void addPath()  //管理员编辑——增加路径
{
    if (ZJUMap.placeNum <= 0)
    {
        printf("No place in the map!\n");
        admin();
    }
    if (ZJUMap.placeNum == 1)
    {
        printf("There is only one attraction in the current system, cannot add path!\n");
        admin();
    }
    showInfo();
    printf("Please enter the 2 numbers:\n");
    add:
    scanf("%d%d", &a, &b);
    if (a < 1 || a > ZJUMap.placeNum || b < 1 || b > ZJUMap.placeNum || a == b)
    {
        if (a == b)
            printf("Do not enter two identical numbers, please enter again!\n");
        else
            printf("Invalid number, both numbers should be between 1 and %d!\n", ZJUMap.placeNum);
        goto add;
    }
    if (ZJUMap.path[a - 1][b - 1] < INF)
    {
        printf("There is already a road between %s and %s, cannot add another one!\n", ZJUMap.place[a - 1].name, ZJUMap.place[b - 1].name);
        admin();
    } else
    {
        int distance;
        printf("Please enter the length of the road between %s and %s:\n", ZJUMap.place[a - 1].name, ZJUMap.place[b - 1].name);
        scanf("%d", &distance);
        undis:
        if (distance <= 0 || distance >= INF)
        {
            printf("Inlegal distance, please input again!\n");
            scanf("%d", &distance);
            goto undis;
        }
        ZJUMap.path[a - 1][b - 1] = ZJUMap.path[b - 1][a - 1] = distance;
        ZJUMap.pathNum++;
        printf("The road between %s and %s has been added successfully!\n", ZJUMap.place[a - 1].name, ZJUMap.place[b - 1].name);
    }
    printf("Do you want to continue adding? [Yes 1 / No 0]\n");
    scanf("%d", &flag);
    if (flag == 1)
        addPath();
    admin();
}

void delPlace()  //管理员编辑——删除地点
{
    if (ZJUMap.placeNum <= 0)
    {
        printf("No path in the map!\n");
        return;
    }
    showInfo();
    printf("Please enter the number\n");
    scanf("%d", &a);
    while (a > ZJUMap.placeNum || a < 1)
    {
        printf("The number should be between 1 and %d. Please enter again!\n", ZJUMap.placeNum);
        scanf("%d", &a);
    }
    printf("The name of the attraction you want to delete is %s. Please enter 1 to confirm. \n", ZJUMap.place[a - 1].name);
    scanf("%d", &flag);
    if (flag == 1)
    {
        int i, j;
        int count = 0;
        //数一下有多少条相关的道路
        for (i = 0; i < ZJUMap.placeNum; i++)
            if (ZJUMap.path[a - 1][i] != INF)
                count++;
        //其他景点往前移动
        for (i = a - 1; i < ZJUMap.placeNum; i++)
            ZJUMap.place[i] = ZJUMap.place[i + 1];
        //景点移动后，修改对应道路
        //横向
        for (i = 0; i < ZJUMap.placeNum; i++)
            for (j = a - 1; j < ZJUMap.placeNum; j++)
                ZJUMap.path[i][j] = ZJUMap.path[i][j + 1];
        //纵向
        for (i = 0; i < ZJUMap.placeNum; i++)
            for (j = a - 1; j < ZJUMap.placeNum; j++)
                ZJUMap.path[j][i] = ZJUMap.path[j + 1][i];
        ZJUMap.placeNum--;
        ZJUMap.pathNum -= count;
    } else
        return;
    printf("Delete successfully!\n");
    printf("Continue with deletion? [Yes 1/No 0]\n");
    scanf("%d", &flag);
    if (flag == 1)
        delPlace();
    showInfo();
    admin();
}

void delPath()  //管理员编辑——删除路径
{
    if (ZJUMap.pathNum <= 0)
    {
        printf("No path in the map\n");
        admin();
    }
    showInfo();
    printf("There are currently %d paths in total.\n", ZJUMap.pathNum);
    printf("Please enter the numbers of the two attractions to delete the road:\n");
    scanf("%d %d", &a, &b);
    while (a < 1 || a > ZJUMap.placeNum || b < 1 || b > ZJUMap.placeNum || a == b)
    {
        if (a == b)
            printf("Please do not enter two identical numbers, please enter again!\n");
        if (a < 1 || a > ZJUMap.placeNum || b < 1 || b > ZJUMap.placeNum)
            printf("Incorrect input of number. The number should be between 1 and %d. Please enter again!\n", ZJUMap.placeNum);
        scanf("%d %d", &a, &b);
    }
    if (ZJUMap.path[a - 1][b - 1] >= INF)
    {
        printf("No path between %s and %s\n", ZJUMap.place[a - 1].name, ZJUMap.place[b - 1].name);
        admin();
    } else
    {
        printf("Confirm to delete the path between %s and %s? [Confirm 1/Cancel 0] \n", ZJUMap.place[a - 1].name, ZJUMap.place[b - 1].name);
        scanf("%d", &flag);
        if (flag == 1)
        {
            ZJUMap.path[a - 1][b - 1] = ZJUMap.path[b - 1][a - 1] = INF;
            ZJUMap.pathNum--;
            printf("Delete successfully\n");
        } else
            admin();
    }
    printf("Continue with deletion? [Yes 1/No 0]\n");
    int f;
    scanf("%d", &f);
    if (f == 1)
        delPath();
    admin();
}

void modPlace()  //管理员编辑——修改地点信息
{
    if (ZJUMap.place <= 0)
    {
        printf("There are no attractions in the map, please add attractions first!\n");
        return;
    }
    showInfo();
    printf("Please enter the number\n");
    scanf("%d", &num);
    while (num < 1 || num > ZJUMap.placeNum)
    {
        printf("Incorrect input of number. The number should be between 1 and %d. Please enter again!\n", ZJUMap.placeNum);
        scanf("%d", &num);
    }
    char newName[100];
    char newInfo[1000];
    printf("The current name of this attraction is %s. Please enter the new name for this attraction:\n", ZJUMap.place[num - 1].name);
    scanf("%s", newName);
    //printf("The current  of this attraction is %s. Please enter the new name for this attraction:\n", hbuMap.place[num - 1].info);
    //scanf("%s", newInfo);
    strcpy(ZJUMap.place[num - 1].name, newName);
    //strcpy(ZJUMap.place[num - 1].info, newInfo);
    printf("Modify successfully!\n");
    admin();
}

int main()
{
    //printf("First use【Y1/N0】\n");
    init();
    readMap();
    //print_two_dimension();
    login();
    return 0;
}
