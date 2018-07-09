#include "pro.h"

#include <assert.h>
#include <pthread.h>

SLN* head = NULL;//定义全局变量，链表的头

SLN* BuyNode(int fd)
{
  SLN* node = (SLN*)malloc(sizeof(SLN));
  assert(node);
  node->_sockfd = fd;
  node->_prev = NULL;
  node->_next = NULL;

  return node;
}

int ServerDo()
{
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if(fd < 0){
    perror("socket");
    exit(1);
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(IP);
  server.sin_port = htons(PORT);

  int bin = bind(fd, (struct sockaddr*)&server, sizeof(server));
  if(bin < 0){
    perror("bind");
    exit(2);

  }

  int lis = listen(fd, 5);
  if(lis < 0){
    perror("listen");
    exit(3);

  }
  printf("listen....\n");
  return fd;
}

void* func(void* arg)
{
  printf("1\n");
  SLN* node = (SLN*)arg;
  SLN* cur = NULL;
  char buf[NAME+MSG]= {0};

  while(1){
    ssize_t rec = recv(node->_sockfd, buf, MSG+NAME,0);//阻塞式接受
    if(rec <= 0)
    {
      printf("client quit\n");
      break;
    }
    cur = head->_next;
    while(cur)
    {
      send(cur->_sockfd, buf, MSG+NAME,MSG_DONTWAIT);
      cur = cur->_next;
    }
  }

  //删除节点，prev node next
  SLN* prev = node->_prev;
  prev->_next = node->_next;
  if(node->_next != NULL)
  {
    node->_next->_prev = prev;
  }
  node->_next = NULL;
  node->_prev = NULL;
  free(node);

  close(node->_sockfd);
  return NULL;
}

void Working(int listenfd)//服务器工作
{
  int acceptfd = 0;//接收到客户端的连接，创建的新文件描述符
  while(1){
    acceptfd = accept(listenfd,NULL,NULL);//接收新连接

    if(acceptfd < 0){
      perror("accept");
      continue;
    }

    printf("%d\n",acceptfd);

    //将客户端的套接字信息存入链表中，采用头插
    SLN* newnode = BuyNode(acceptfd);
    printf("%p\n",newnode);

    newnode->_next = head->_next;
    newnode->_prev = head;

    if(head->_next != NULL)
    {
      head->_next->_prev = newnode;
    }
  
    head->_next = newnode;
    printf("head\n");

    pthread_t id = 0; 
    pthread_create(&id,NULL,func,(void*)newnode);

    pthread_detach(id);//将新线程分离出去

  }

  close(listenfd);
}


int main()
{
  head = BuyNode(0);
  int sockfd = ServerDo();
  Working(sockfd);
  return 0;


}
