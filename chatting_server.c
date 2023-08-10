#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define CLNT_MAX 10
#define BUFFSIZE 200
#define PORT 7889
// SOCK_DGRAM : UDP
// PF_INET = AF_INET : protocol, address
// INADDR_ANY : computer ip address

int g_clnt_sockets[CLNT_MAX];
int g_clnt_count = 0;

pthread_mutex_t g_mutex;

void sendAllClnt(char *msg, int mysock)
{

  pthread_mutex_lock(&g_mutex);

  for (int i = 0; i < g_clnt_count; i++)
  {
    if (g_clnt_sockets[i] != mysock)
    {
      printf("send msg : %s", msg);
      write(g_clnt_sockets[i], msg, strlen(msg) + 1);
    }
  }

  pthread_mutex_unlock(&g_mutex);
}

void *clnt_connection(void *arg)
{

  int clnt_sock = (int)arg;
  int str_len = 0;

  char msg[BUFFSIZE];
  int i;

  while (1)
  {

    str_len = read(clnt_sock, msg, sizeof(msg));
    if (str_len == -1)
    {
      printf("clnt[%d] close\n", clnt_sock);
      break;
    }
    sendAllClnt(msg, clnt_sock);

    printf("%s\n", msg);
  }

  pthread_mutex_lock(&g_mutex);

  for (i = 0; i < g_clnt_count; i++)
  {
    if (clnt_sock == g_clnt_sockets[i])
    {
      for (; i < g_clnt_count - 1; i++)
        g_clnt_sockets[i] = g_clnt_sockets[i + 1];
      break;
    }
  }

  pthread_mutex_lock(&g_mutex); // unlock으로 바꿔보기
  close(clnt_sock);
  pthread_exit(0);
  return NULL;
}

int main(int argc, char **argv)
{

  int serv_sock;
  int clnt_sock;

  pthread_t t_thread;

  struct sockaddr_in clnt_addr;
  int clnt_addr_size;

  pthread_mutex_init(&g_mutex, NULL);

  struct sockaddr_in serv_addr;
  serv_sock = socket(PF_INET, SOCK_STREAM, 0);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);
  //	serv_addr.sin_port = htons(atoi(argv[1]);

  if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
  {
    printf("bind error\n");
  }

  if (listen(serv_sock, 5) == -1)
  {
    printf("listen error");
  }

  char buff[200];
  int recv_len = 0;

  while (1)
  {

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

    pthread_mutex_lock(&g_mutex);
    g_clnt_sockets[g_clnt_count++] = clnt_sock;
    pthread_mutex_unlock(&g_mutex);

    pthread_create(&t_thread, NULL, clnt_connection, (void *)clnt_sock);
  }

  return 0;
}
