#include "unp.h"

int main(int argc, char **argv){
    int sockfd,n;
    char recvline[MAXLINE+1]
    struct socketaddr_in servaddr;

    if(argc !=2 ){
        err_quie("usage: a.out <IPaddress>");
    }
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)<0)){
        err_sys("socket error");
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);
    if(inet_(AF_INET,argv[1],&servaddr.sin_addr)<0){
        err_quie("inet_pton error for %s",argv[1]);
    }
    if(connect(sockfd,(SA*)&servaddr,sizeof(servaddr))<0){
        err_sys("conn error");
    }
    while((n = read(sockfd,recvline,MACLINE))>0){
        recvline[n] = 0;
        if(fputs(recvline,stdout) == EOF){
            err_sys("fputs error");
        }
    }
    if(n<0){
        err_sys("read error");
    }
    exit(0);
}