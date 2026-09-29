#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../Inc/List.h"

//初始化头节点
Node * Head_Init()
{
    Node * head = malloc(sizeof(Node));
    head->code = 0;
    head->next = NULL;
    return head;
}

//判断是否为空
bool Node_IsEmpty(Node * head)
{
    return head->next == NULL;    
}

//初始化新的节点
Node * New_Init()
{
    Node * new = malloc(sizeof(Node));
    new->code = -1;
    strcpy(new->train_number, "0");
    strcpy(new->depart_name, "0");
    strcpy(new->depart_city, "0");
    strcpy(new->arrive_name, "0");
    strcpy(new->arrive_city, "0");
    strcpy(new->depart_time, "0");
    strcpy(new->arrive_time, "0");
    strcpy(new->date, "0");
    new->next = NULL;
    return new;
}

//尾插法
void Node_Insert(Node * head, Node * new)
{
    if(head == NULL)
        return;
    Node * temp = head;
    while(temp->next != NULL)
        temp = temp->next;
    temp->next = new;
    new->next = NULL;
}

//编译链表
void Node_Printf(Node * head)
{
    if(head == NULL)
        return;
    Node * temp = head->next;
    printf("%d\n", head->code);
    printf("车次\t出发站\t出发城市\t到达站\t到达城市\t出发时间\t到达时间\t日期\n");
    while(temp != NULL)
    {
        printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", temp->train_number, temp->depart_name, temp->depart_city, temp->arrive_name, temp->arrive_city, temp->depart_time, temp->arrive_time, temp->date);
        temp = temp->next;
    }
}

/*
//销毁链表
void Node_Destroy(Node * head)
{
    if(head == NULL)
        return;
    Node * temp = head;
    while(temp != NULL)
    {
        Node * temp1 = temp->next;
        free(temp);
        temp = temp1;
    }
    head = NULL;
}
*/


