# Chatting program

### 리눅스 동작

헤더파일

<unistd.h>

```c
int close(int fd);
ssize_t write(int fd, const void * buf, size_t nbytes);
ssize_t read(int fd, void *buf,size_t nbytes);
```

<arpa/inet.h> : 주소 변환 기능을 사용할 경우 사용하는 헤더

<sys/types.h> : 데이터 r/w 을 위한 파일 I/O, 자료형이 저장된 헤더
해당 코드는 없어도 돌아감

```c
int open(const char *path, int flag);
```

<sys/socekt.h> : 소켓 헤더

```c
int socket(int domain,int type, int protocol);
int bind(int sockfd, struct sockaddr, *myaddr, socklen_t addrlen);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int listen(int sockfd, int backlog);
int connect(int sockfd, struct sockaddr *serv_addr,socklen_t addrlen);
```

<netinet/in.h> : 소켓 구조체 헤더

### TCP/IP 통신

### thread 사용

### 출처 [OJ Tube](https://www.youtube.com/watch?v=GLMNFOCiGSU&list=PLz--ENLG_8TMdMJIwyqDIpcEOysvNoonf&index=22)
