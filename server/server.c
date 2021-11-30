#include "config.h"

ListNode *userList = NULL;
int while_flag = 1;

int main(int argc, char **argv){
    int listenfd,maxfd,maxi,client_sockfd[FD_SETSIZE],nready;
    int opt = 1;
	
    struct sockaddr_in server_addr , client_addr;
    if((listenfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
	{
		perror("socket error.\n");
		exit(1);
	}
    bzero(&server_addr , sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
    setsockopt(listenfd , SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt));
    if(bind(listenfd , (struct sockaddr *)&server_addr , sizeof(server_addr)) < 0)
	{
		perror("bind error.\n");
		exit(1);
	}
	if(listen(listenfd , LISTENEQ) < 0)
	{
		perror("listen error.\n");
		exit(1);
	}
    /*(5) 首先初始化客户端描述符集*/
	maxfd = listenfd;
	maxi = -1;
    int i;
	for(i=0; i<FD_SETSIZE; ++i)
	{
		client_sockfd[i] = -1;
	}//for
    fd_set rset , allset;
	/*清空allset描述符集*/
	FD_ZERO(&allset);

	/*将监听描述符加到allset中*/
	FD_SET(listenfd , &allset);
    socklen_t clilen;
    int connfd,sockfd;
    pthread_t pid;
	list_init(&userList);
    while(while_flag){
        rset = allset;
		/*得到当前可读的文件描述符数*/
		nready = select(maxfd+1 , &rset , NULL , NULL , 0);
        if(FD_ISSET(listenfd , &rset))
		{
			/*接收客户端的请求*/
			clilen = sizeof(client_addr);
			if((connfd = accept(listenfd , (struct sockaddr *)&client_addr , &clilen)) < 0)
			{
				perror("accept error.\n");
				exit(1);
			}//if
			
			printf("server: got connection from %s\n", inet_ntoa(client_addr.sin_addr));

			/*查找空闲位置，设置客户链接描述符*/
			for(i=0; i<FD_SETSIZE; ++i)
			{
				if(client_sockfd[i] < 0)
				{
					client_sockfd[i] = connfd; /*将处理该客户端的链接套接字设置在该位置*/
					break;				
				}//if
			}//for

			if(i == FD_SETSIZE)
			{		
				perror("too many connection.\n");
				exit(1);
			}//if

			/* 将来自客户的连接connfd加入描述符集 */
			FD_SET(connfd , &allset);

			/*新的连接描述符 -- for select*/
			if(connfd > maxfd)
				maxfd = connfd;
			
			/*max index in client_sockfd[]*/
			if(i > maxi)
				maxi = i;

			/*no more readable descriptors*/
			if(--nready <= 0)
				continue;
		}//if
        for(i=0 ; i<=maxi ; ++i)
		{
			if((sockfd = client_sockfd[i]) < 0)
				continue;
				
			if(FD_ISSET(sockfd , &rset))
			{
				/*如果当前没有可以读的套接字，退出循环*/
				if(--nready < 0)
					break;	
                printf("touch pthread\n");
				pthread_create(&pid , NULL , (void *)handleRequest , (void *)&sockfd);
				
			}//if
			/*清除处理完的链接描述符*/
			FD_CLR(sockfd , &allset);
			client_sockfd[i] = -1;			
		}//for
    }
    close(listenfd);
    return 0;
}
void printf_msg(Message *message){
    printf("[%s]sendName : %s\n", inet_ntoa(message->sendAddr.sin_addr),message->sendName);
	printf("[%s]recvName : %s\n", inet_ntoa(message->recvAddr.sin_addr),message->recvName);
	printf("msgTime : %s\n", message->msgTime);
	printf("text : %s\n", message->text);
	printf("msgType : %d\n", message->msgType);
	printf("msgRet : %d\n", message->msgRet);
}

void* handleRequest(int *fd)
{
    int sockfd , ret , n;
	/*声明消息变量*/
	Message message;
	/*声明消息缓冲区*/
	char buf[MAX_LINE];

	sockfd = *fd;

	memset(buf , 0 , MAX_LINE);
	memset(&message , 0 , sizeof(message));
    n = recv(sockfd , buf , sizeof(buf)+1 , 0);
    if(n <= 0)
	{
		//关闭当前描述符，并清空描述符数组 
		fflush(stdout);
		close(sockfd);
		*fd = -1;
		printf("来自%s的接收失败！\n", inet_ntoa(message.sendAddr.sin_addr));		
		return NULL;			
	}else{
        memcpy(&message , buf , sizeof(message));
		//printf_msg(message);
		printf("[%s]sendName : %s\n", inet_ntoa(message.sendAddr.sin_addr),message.sendName);
		printf("[%s]recvName : %s\n", inet_ntoa(message.recvAddr.sin_addr),message.recvName);
		printf("msgTime : %s\n", message.msgTime);
		printf("text : %s\n", message.text);
		printf("msgType : %d\n", message.msgType);
		printf("msgRet : %d\n", message.msgRet);
        switch(message.msgType)
        {
            case REGISTER:
            	printf("来自%s的注册请求！\n", inet_ntoa(message.sendAddr.sin_addr));
				break;
			case LOGIN:
				printf("来自%s的登录请求！\n", inet_ntoa(message.sendAddr.sin_addr));
				ret = loginUser(&message , sockfd);	
							
				memset(&message , 0 , sizeof(message));
				message.msgType = RESULT;
				message.msgRet = ret;
				//strcpy(message.text , stateMsg(ret));							
				memset(buf , 0 , MAX_LINE);
				memcpy(buf , &message , sizeof(buf));						
				/*发送操作结果消息*/
				send(sockfd , buf , sizeof(buf) , 0);
				if(ret == SUCCESS)
				{
					printf("登录成功\n");
					enterChat(&sockfd);
				}else{
					printf("登录失败\n");
				}	
				break;
			case HELP:
				printf("来自%s的帮助请求！\n", inet_ntoa(message.sendAddr.sin_addr));
				traverseList(userList);
				break;
			case EXIT: 
				printf("来自%s的退出请求！\n", inet_ntoa(message.sendAddr.sin_addr));
				break;
			default:
				printf("unknown operation.\n");
				break;
        }
    }
    close(sockfd);
	*fd = -1;
	return NULL;			
}