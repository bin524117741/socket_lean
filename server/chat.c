#include "config.h"
extern ListNode *userList;
void enterChat(int *fd)
{
    int n,ret,sockfd;
	User user;
	/*消息发送缓冲区*/
	char buf[MAX_LINE];
	memset(buf , 0 , MAX_LINE);

	/*消息内容*/
	Message message;	
	memset(&message , 0 , sizeof(message));
	
	sockfd = *fd;

    while(1){
        n = recv(sockfd, buf, sizeof(buf),0);
        if(n < 1){
            printf("recv error\n");
            close(sockfd);
			return ;	
        }else{
            memcpy(&message , buf , sizeof(buf));
            printf("[%s]sendName : %s\n", inet_ntoa(message.sendAddr.sin_addr),message.sendName);
            printf("[%s]recvName : %s\n", inet_ntoa(message.recvAddr.sin_addr),message.recvName);
            printf("msgTime : %s\n", message.msgTime);
            printf("text : %s\n", message.text);
            printf("msgType : %d\n", message.msgType);
            printf("msgRet : %d\n", message.msgRet);
            switch(message.msgType){
                case VIEW_USER_LIST:
                    printf("来自%s的查看在线用户列表请求！\n", message.sendName);
                    /*转到查看在线用户列表处理函数*/
                    traverseList(userList);
                    //ret = viewUserList(&message , sockfd);
                break;
                case EXIT:
                {
					/*用户退出聊天室*/
					printf("用户%s退出聊天室！\n", message.sendName);
					memset(&user , 0 , sizeof(user));
					strcpy(user.userName , message.sendName);
					deleteNode(userList , user);
					close(sockfd);
					return;
				}
                default:			
				break;
            }
        }
    }
}