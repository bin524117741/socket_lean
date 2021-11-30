#include "config.h"
extern ListNode *userList;
int loginUser(Message *msg , int sockfd)
{
    int ret;
	/*声明用户信息*/
	User user;
	char buf[MAX_LINE];
    Message message;
    recv(sockfd , buf , sizeof(user) , 0);
	memset(&user , 0 , sizeof(user));
	memcpy(&user , buf , sizeof(user));
	user.userAddr = (*msg).sendAddr;
	user.sockfd = sockfd;
    if(isOnLine(userList , user) == 1)
    {
        printf("user is online");
        return ALREADY_ONLINE;
    }
		
    /*
    user
    should
    cmp psw for sql
    */
    // userList = insertNode(userList , &user);
    insertListHead(userList , user);
    ret = SUCCESS;
    printf("login success\n");
    printf("cur user name  is :");
    traverseList(userList);
    printf("\n");
    return ret;
}