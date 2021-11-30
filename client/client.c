#include "config.h"
int mainInterface()
{
   
		printf("-------------------------------------\n");
		printf("    欢迎进入小B聊天室～	      \n");
		printf("	   1.注册                 \n");
		printf("	   2.登陆			      \n");
		printf("	   3.帮助			      \n");
		printf("	   4.退出			      \n");
		printf("-------------------------------------\n\n\n");
}
int helpInterface()
{
   
		printf("-------------------------------------\n");
		printf("         欢迎进入小帮助～	      \n");
		printf("	           ^_^                 \n");
		printf("        请在主界面选择操作～     \n");
		printf("	           ^_^			      \n");
		printf("-------------------------------------\n\n\n");
}
// void chatInterface(char userName[])
// {
// 		printf("------------------------------------------\n");
// 		printf("你好，%s                       \n", userName);
// 		printf("	     1. 查看在线用户                 \n");
// 		printf("	     2. 私聊                         \n");
// 		printf("	     3. 群聊                         \n");
// 		printf("	     4. 查看聊天记录                 \n");
// 		printf("	     5. 退出                         \n");
// 		printf("请选择操作～                    \n");
// 		printf("------------------------------------------\n\n\n");
// }
// void deal_msg(int msg,char *text){

// }
int main(int argc,char **argv){
    int sockfd , choice , ret; //choice代表用户在主界面所做选择,ret代表操作结果
	struct sockaddr_in servaddr;
	struct hostent *host;
    /*声明消息变量*/
	Message message;
	/*声明消息缓冲区*/
	char buf[MAX_LINE];

	/*UserInfo*/
	User user;
    time_t timep;	/*存储当前时间*/
	strcpy(user.userName , "***");
	user.speak = 1;
    while(1){
        if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
		{
			perror("socket error");
			exit(1);
		}//if

		/*(2) 设置链接服务器地址结构*/
		bzero(&servaddr , sizeof(servaddr));		/*清空地址结构*/
		servaddr.sin_family = AF_INET;				/*使用IPV4通信域*/
		servaddr.sin_port = htons(PORT);			/*端口号转换为网络字节序*/
		//servaddr.sin_addr = *((struct in_addr *)host->h_addr);		/*可接受任意地址*/
		if(inet_pton(AF_INET , "192.168.1.4" , &servaddr.sin_addr) < 0)
		{
			printf("inet_pton error for 192.168.1.4\n");
			exit(1);
		}//if

		 /*(3) 发送链接服务器请求*/
		if( connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
		{
			perror("connect error");
			exit(1);
		}//if
        mainInterface();	
		setbuf(stdin,NULL); //是linux中的C函数，主要用于打开和关闭缓冲机制
		scanf("%d",&choice);
		setbuf(stdin,NULL);	
        while(choice != 1 && choice != 2 && choice != 3 && choice !=4)
		{
			printf("未找到命令，直接退出！\n");
			close(sockfd);
			exit(0);	/*用户退出*/
		}//while
        switch (choice)
        {
            case REGISTER:
                memset(&message , 0 , sizeof(message));
			    memset(buf , 0 , MAX_LINE);		
			    message.msgType = REGISTER;
                strcpy(message.text ,"123456");
			    message.sendAddr = servaddr;
                time(&timep);
			    strcpy(message.msgTime , ctime(&timep));
                memcpy(buf , &message , sizeof(message));	
			    send(sockfd , buf , sizeof(buf) , 0);
                printf("user register\n");
                break;
            case LOGIN:
                memset(&message , 0 , sizeof(message));
			    memset(buf , 0 , MAX_LINE);		
			    message.msgType = LOGIN;
                strcpy(message.text ,"987654");
			    message.sendAddr = servaddr;
                memcpy(buf , &message , sizeof(buf));	
			    send(sockfd , buf , sizeof(buf) , 0);
                printf("user login\n");
                if(loginUser(sockfd)!= SUCCESS)
                {
                    close(sockfd);
                    printf("退出聊天室!\n");
                    exit(0);	/*用户退出*/
                }
                break;
            case HELP:
                memset(&message , 0 , sizeof(message));
                memset(buf , 0 , MAX_LINE);
                message.msgType = HELP;
                strcpy(message.text ,"aaaaaa");
                message.sendAddr = servaddr;
                /*向服务器发送登陆请求*/
                memcpy(buf , &message , sizeof(buf));
                send(sockfd , buf , sizeof(buf) , 0);
                printf("uesr help!\n");
                helpInterface();
                break;
            case EXIT:
                memset(&message , 0 , sizeof(message));
                memset(buf , 0 , MAX_LINE);
                message.msgType = EXIT;
                strcpy(message.text ,"aaaaaa");
                message.sendAddr = servaddr;
                /*向服务器发送登陆请求*/
                memcpy(buf , &message , sizeof(buf));
                send(sockfd , buf , sizeof(buf) , 0);
				sleep(2);
                close(sockfd);
                printf("退出聊天室!\n");
                exit(0);	/*用户退出*/
                break;
            default:
                printf("unknown operation.\n");
                //goto sign;
                break;	
		//switch	
        }
    }
    close(sockfd);
    return 0;
}