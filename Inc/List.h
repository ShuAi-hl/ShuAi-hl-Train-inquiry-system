#ifndef __LIST_H__
#define __LIST_H__

/*
struct Data{
    char add[30];
    char end[30];
    int year;
    int month;
    int day;
};
struct Data data;
*/


typedef struct Node{
    int code;
    char train_number[10];      //火车编号
    char depart_name[30];       //离开的车站
    char depart_city[30];       //离开的城市
    char arrive_name[30];       //到达的车站
    char arrive_city[30];       //到达的城市
    char depart_time[30];       //离开的时间    
    char arrive_time[30];       //到达的时间
    char date[15];              //发车日期

    struct Node * next;                //指向下一个结点
}Node;

Node * head;

Node * Head_Init();                              //初始化头结点
bool Node_IsEmpty(Node * head);                  //判断链表是否为空
Node * New_Init();                               //创建新结点
void Node_Insert(Node * head, Node * new);       //插入结点
void Node_Printf(Node * head);

//void Node_Destroy(Node * head);                  //销毁链表

#endif //__LIST_H__