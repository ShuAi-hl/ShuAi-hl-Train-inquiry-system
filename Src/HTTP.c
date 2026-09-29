#define _GNU_SOURCE

#define  _POSIX_C_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/epoll.h>

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <time.h>

#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>  //本地转网络  网络转本地 头文件
#include "../Inc/cJSON.h"

#include "../Inc/List.h"
#include "../UI/ui_events.h"

typedef struct sockaddr_in IPV4;
typedef struct sockaddr    ADDR;
socklen_t socklen = sizeof(IPV4);

int tcp_Client(const char * com)
{
    struct hostent *p = gethostbyname(com);
    const char * IP = inet_ntoa(*(struct in_addr *)(p->h_addr_list[0]));
    printf("域名地址:%s\n",IP);

    int tcpfd = socket(AF_INET, SOCK_STREAM, 0);
    if(tcpfd == -1)
    {
        perror("tcpfd socket failed");
        return -1;
    }

    IPV4 saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(80);
    saddr.sin_addr.s_addr = inet_addr(IP);

    int ret = connect(tcpfd, (ADDR *)&saddr, socklen);
    if(ret == -1)
    {
        perror("connect failed");
        return -1;
    }
    printf("客户端连接成功\n");

    return tcpfd;
}

void Reply(int tcpfd)
{
    // 动态内存接收
    size_t total_size = 0;
    size_t buffer_capacity = 4096; 
    char *json_response = (char *)malloc(buffer_capacity);
    if(json_response == NULL) {
        perror("malloc failed");
        return;
    }
    json_response[0] = '\0';

    while(1)
    {
        char buf[1024] = {0};
        size_t size = recv(tcpfd, buf, sizeof(buf), 0);
        if(size == 0)
        {
            printf("recv completed\n");
            break;
        }
        if(size == (size_t)-1) {
            perror("recv error");
            break;
        }

        // 检查容量是否足够，不够则动态扩容
        while(total_size + size >= buffer_capacity) {
            buffer_capacity *= 2;
            char *new_ptr = (char *)realloc(json_response, buffer_capacity);
            if(new_ptr == NULL) {
                perror("realloc failed");
                free(json_response);
                return;
            }
            json_response = new_ptr;
        }

        memcpy(json_response + total_size, buf, size);
        total_size += size;
        json_response[total_size] = '\0';
    }

    char *json_start = strstr(json_response, "{");
    if(json_start)
    {
        cJSON *root = cJSON_Parse(json_start);
        if(root)
        {
            cJSON *code_item = cJSON_GetObjectItem(root, "code");
            if(code_item && cJSON_IsNumber(code_item))
            {
                head->code = code_item->valueint;
            }

            cJSON *data_array = cJSON_GetObjectItem(root, "datas");
            if(data_array && cJSON_IsArray(data_array))
            {
                cJSON *item = NULL;
                cJSON_ArrayForEach(item, data_array)
                {
                    Node *new_node = New_Init();
                    cJSON *tmp = NULL;

                    tmp = cJSON_GetObjectItem(item, "train_number");
                    if(tmp && tmp->valuestring) strcpy(new_node->train_number, tmp->valuestring);

                    tmp = cJSON_GetObjectItem(item, "depart_name");
                    if(tmp && tmp->valuestring) strcpy(new_node->depart_name, tmp->valuestring);

                    tmp = cJSON_GetObjectItem(item, "depart_city");
                    if(tmp && tmp->valuestring) strcpy(new_node->depart_city, tmp->valuestring);

                    tmp = cJSON_GetObjectItem(item, "arrive_name");
                    if(tmp && tmp->valuestring) strcpy(new_node->arrive_name, tmp->valuestring);

                    tmp = cJSON_GetObjectItem(item, "arrive_city");
                    if(tmp && tmp->valuestring) strcpy(new_node->arrive_city, tmp->valuestring);

                    tmp = cJSON_GetObjectItem(item, "depart_time");
                    if(tmp && tmp->valuestring) strcpy(new_node->depart_time, tmp->valuestring);

                    tmp = cJSON_GetObjectItem(item, "arrive_time");
                    if(tmp && tmp->valuestring) strcpy(new_node->arrive_time, tmp->valuestring);

                    tmp = cJSON_GetObjectItem(item, "date");
                    if(tmp && tmp->valuestring) strcpy(new_node->date, tmp->valuestring);

                    Node_Insert(head, new_node);
                }
            }
            cJSON_Delete(root);
        }
    }
    
    free(json_response); // 释放动态内存
    Node_Printf(head);
    printf("完成\n");
}

void *Get_Datas(void *arg)
//int main()
{
    pthread_detach(pthread_self());

    printf(">>> 发起查询: add=[%s], end=[%s], date=%d-%d-%d\n",
       add, end, date->year, date->month, date->day);

    int tcpfd = tcp_Client("cn.apihz.cn");
    if(tcpfd == -1)
        return NULL;

    // 定义关键数据变量，可直接修改此处或改为从终端读取以实现控制
    char *id = "********";
    char *key = "********************************";

    char Request[256];

    // 拆分模板并合并变量数据
    sprintf(Request, 
             "GET /api/12306/api.php?"
             "id=%s&key=%s&"
             "add=%s&"
             "end=%s&"
             "y=%d&"
             "m=%d&"
             "d=%d HTTP/1.1\r\n"
             "Host: cn.apihz.cn\r\n"
             "Connection: close\r\n"
             "\r\n",
             id, key, add, end, date->year, date->month, date->day);

    send(tcpfd, Request, strlen(Request), 0);
    Reply(tcpfd);

    close(tcpfd);
}