#ifndef __PRO_H__
#define __PRO_H__

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>

#define NAME 20 //用户名长度
#define MSG 1024 //发送信息大小
#define IP "172.27.0.14"
#define PIP "118.24.182.142"
#define PORT 6688 

typedef struct SocketListNode
{

  int _sockfd;
  struct SocketListNode* _prev;
  struct SocketListNode* _next;

}SLN;//带头双向链表，存储所� 客户端套接文件描述符

typedef struct User
{

  char _name[NAME];//用户名
  char _msg[MSG];//用户发送的信息

}user;

#endif //__PROTOCOL_H___

