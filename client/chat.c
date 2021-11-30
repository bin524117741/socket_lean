#include "config.h"
void recvMsg(int *sockfd){

}
void chatInterface(char userName[])
{
		printf("------------------------------------------\n");
		printf("你好，%s                       \n", userName);
		printf("	     1. 查看在线用户                 \n");
		printf("	     2. 私聊                         \n");
		printf("	     3. 群聊                         \n");
		printf("	     4. 查看聊天记录                 \n");
		printf("	     5. 退出                         \n");
		printf("请选择操作～                    \n");
		printf("------------------------------------------\n\n\n");
}


void enterChat(User *user , int sockfd)
{
    char c , buf[MAX_LINE] , str[MAX_LINE];
    pthread_t pid;	/*处理接收消息线程*/
	Message message;
    time_t timep;	/*存储当前时间*/
    int choice = 0;
	/*创建接收消息线程*/
	int ret = pthread_create(&pid , NULL , (void *)recvMsg , (void *)&sockfd);
    if(ret != 0){
        printf("error ! please repeat login\n");
        memset(&message , 0 , sizeof(message));
        strcpy(message.sendName , (*user).userName);
        message.msgType = EXIT;
        memcpy(buf , &message , sizeof(message));
		send(sockfd , buf , sizeof(buf) , 0);
		close(sockfd);
		exit(1); 
    }
    setbuf(stdin, NULL);
    while(1){
        memset(&message , 0 , sizeof(message));
		strcpy(message.sendName , (*user).userName);
		memset(&str , 0 , MAX_LINE);
		memset(buf , 0 , MAX_LINE);
        chatInterface((*user).userName);
        setbuf(stdin,NULL); //是linux中的C函数，主要用于打开和关闭缓冲机制
        scanf("%d",&choice);
        while(choice != 1 && choice != 2 && choice != 3 && choice !=4 && choice != 5)
        {
            printf("未知操作，请重新输入！\n");
            memset(&message , 0 , sizeof(message));
            strcpy(message.sendName , (*user).userName);
            message.msgType = EXIT;
            memcpy(buf , &message , sizeof(message));
            send(sockfd , buf , sizeof(buf) , 0);
            close(sockfd);
            exit(1);
        }//while
        switch(choice)
        {
            case 1:
            {
                message.msgType = VIEW_USER_LIST;
                memcpy(buf , &message , sizeof(message));
                send(sockfd , buf , sizeof(buf) , 0);						
                break;	
            }
            case 2:
            {
                printf("chat with one\n");
                break;
            }
            case 3:
            {
                printf("chat with group\n");
                break;
            }
            case 4:
            {
                printf("creat a group\n");
                break;
            }
            case 5:
            {
                printf("exit[%s][%s]\n",message.sendName, (*user).userName);
                memset(&message , 0 , sizeof(message));
                strcpy(message.sendName , (*user).userName);
                message.msgType = EXIT;
                memcpy(buf , &message , sizeof(message));
                send(sockfd , buf , sizeof(buf) , 0);
                close(sockfd);
                exit(0);
                break;
            }

        }
    }
    
}
