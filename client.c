#include "pro.h"


int ClientDo()
{
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if(fd <0){
    perror("socket");
    exit(1);

  }
  
  printf("%d\n",fd);
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(PIP);//客户端访问的是公网IP
  server.sin_port = htons(PORT);
  
  int con = connect(fd, (struct sockaddr*)&server, sizeof(server));
  if(con < 0){
    perror("connect");
    exit(2);

  }
  printf("connect success\n");
  return fd;

}


void Working(int sockfd, char* name)
{
  if(fcntl(0, F_SETFL,FNDELAY) < 0){
    perror("fcntl");
    exit(3);

  }

  user u;
  char buf[NAME+MSG] ={0};
  while(1){
    strncpy(buf, name, NAME);

    ssize_t r = read(0,u._msg, MSG);
    if(r > 0){
      u._msg[r] = 0;
      fflush(stdout);
      strcpy(buf+NAME,u._msg);
      send(sockfd, buf, MSG+NAME, MSG_DONTWAIT);

    }
    ssize_t rcv = recv(sockfd, buf, MSG+NAME, MSG_DONTWAIT);
    if(rcv > 0){
      strncpy(u._name,buf, NAME);
      strncpy(u._msg, buf+NAME,MSG);

      printf("[%s]# %s ",u._name,u._msg);

    }

  }
  close(sockfd);

}
int main(int argc, char* argv[])
{
  if(argc != 2){
    perror("Uage ./client [name]\n");
    return 1;

  }

  int sockfd = ClientDo();

  Working(sockfd, argv[1]);

  return 0;

}

