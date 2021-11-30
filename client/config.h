#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h> /*使用memcpy所需的头文件*/

#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>


#ifndef FD_SETSIZE
#define FD_SETSIZE 256
#endif

#define MAX_LINE  8192
#define PORT  8888
#define LISTENEQ  6000

enum MessageType{	
	REGISTER = 1,	/*注册请求*/		
	LOGIN,		/*登陆请求*/
	HELP,		/*帮助请求*/
	EXIT,				/*退出请求*/
	VIEW_USER_LIST,		/*查看在线列表*/
	GROUP_CHAT,		/*群聊请求*/
	PERSONAL_CHAT,		/*私聊请求*/
	VIEW_RECORDS,		/*查看聊天记录请求*/
	RESULT,				/*结果消息类型*/
	UNKONWN				/*未知请求类型*/
};

enum StateRet{
    EXCEED, //已达服务器链接上限
    SUCCESS, //成功
	FAILED,  //失败
    DUPLICATEID, //重复的用户名
	INVALID,	//不合法的用户名
    ID_NOT_EXIST, //账号不存在
    WRONGPWD, //密码错误
	ALREADY_ONLINE,		//已经在线
	ID_NOT_ONLINE,	//账号不在线
	ALL_NOT_ONLINE, 	//无人在线
	MESSAGE_SELF	//消息对象不能选择自己
};

typedef struct MessageS{
    int msgType;	//
    int msgRet;
    struct sockaddr_in sendAddr; /*发送者IP*/
	struct sockaddr_in recvAddr;
    char sendName[20]; /*发送者名称*/
	char recvName[20]; /*接收者名称*/
	char msgTime[20];  /*消息发送时间*/
    char text[8192];		/*针对聊天类型的消息，填充该字段*/
}Message;
typedef struct _User{
	char userName[20]; 		//用户名
	char password[20];
	struct sockaddr_in userAddr;	//用户IP地址，选择IPV4
	int sockfd;			//当前用户套接字描述符
	int speak;			//是否禁言标志
	char registerTime[20];	//记录用户注册时间	
}User;

typedef struct _ListNode{
	User user;
	struct _ListNode *next;
}ListNode;

extern ListNode *userList;
extern int loginUser(int sockfd);
extern void enterChat(User *user , int sockfd);