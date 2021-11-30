#include "config.h"
void list_init(ListNode **pList){
    *pList = (ListNode *)malloc(sizeof(Node));
	if (!pList)
	{
		printf("malloc error!\n");
		return;
	}
	(*pList)->next = NULL;
}
int isOnLine(ListNode *list , User user)
{
	ListNode *p = list , *pre = p;
	//traverseList(list);
	while(p!=NULL && strcmp(p->user.userName , user.userName) != 0)
	{
		pre = p;
		p = p->next;
	}//while
	
	/*不存在该在线用户*/
	if(p == NULL)
		return 0;
	return 1;
}

int insertListHead(ListNode *pList, User user)
{
	Node *head;
	Node *temp;

	// 判断链表是否存在
	if (!pList)
	{
		printf("list not exist!\n");
		return 0;
	}

	// 让head指向链表的头结点
	head = pList;

	// 创建存放插入元素的结点
	temp = (Node *)malloc(sizeof(Node));
	if (!temp)
	{
		printf("malloc error!\n");
		return 0;
	}
    memset(&(temp->user),0,sizeof(user));
    memcpy(&(temp->user),&(user),sizeof(user));
	// 头结点后插入结点
	temp->next = head->next;
	head->next = temp;

	return 1;
}

// 尾部后插入元素操作
int insertListTail(ListNode *pList, User user)
{
	Node *cur;
	Node *temp;
    int i = 0;
	// 判断链表是否存在
	if (!pList)
	{
		printf("list not exist!\n");
		return 0;
	}

	// 找到链表尾节点
	cur = pList;
	while (cur->next!=NULL)
	{
		cur = cur->next;
	}
    printf("\n");

	// 创建存放插入元素的结点
	temp = (Node *)malloc(sizeof(Node));
	if (!temp)
	{
		printf("malloc error!\n");
		return -1;
	}
	// temp->data = e;
    memset(&(temp->user),0,sizeof(user));
    memcpy(&(temp->user),&(user),sizeof(user));
	// 尾结点后插入结点
	temp->next = NULL; 
	cur->next = temp;

	return 1;
}
ListNode *list_reversd(ListNode *pList)
{
    ListNode *list_old = pList;
    ListNode *list_tmp,*list_new;
    if(pList == NULL){
        printf("list_revers error the list is empty\n");
        return pList;
    }else if(pList->next == NULL)
    {
        printf("list_revers error the list is one point \n");
        return pList;
    }else {
        list_new = list_old->next;
        pList->next = NULL;
        while(list_new!=NULL){
            list_tmp = list_new;
            list_new = list_new->next;
            list_tmp->next = list_old;
            list_old = list_tmp;
        }
        pList = list_old;
        return pList;
    }
}
void deleteNode(ListNode *list , User user)
{
	if(list == NULL)
		return;

	ListNode *p = list , *pre = p;
	while(p!=NULL && strcmp(p->user.userName , user.userName) != 0)
	{
		pre = p;
		p = p->next;
	}//while
	
	/*不存在该在线用户*/
	if(p == NULL)
		return ;
	/*该用户位于链表头部*/
	else if(p == list)
	{
		list = list->next;
	}//elif
	/*该用户位于链表尾部*/
	else if(p->next == NULL)
	{
		pre->next = NULL;
	}//elif
	/*该用户节点位于链表中间*/
	else
	{
		pre->next = p->next;
	}//else
	/*释放该用户节点占用的空间*/
	free(p);
	p = NULL;
}
// 清空链表操作
int clearList(ListNode *pList)
{
	Node *cur; // 当前结点
	Node *temp; // 事先保存下一结点，防止释放当前结点后导致“掉链”

	// 判断链表是否存在
	if (!pList)
	{
		printf("list not exist!\n");
		return 0;
	}

	cur = pList; // 指向头结点
	while (cur)
	{
		temp = cur->next; // 事先保存下一结点，防止释放当前结点后导致“掉链”
		free(cur); // 释放当前结点
		cur = temp; // 将下一结点赋给当前结点p
	}
	pList->next = NULL; // 头结点指针域指向空

	return 1;
}

// 遍历链表操作
void traverseList(ListNode *pList)
{
	// 判断链表是否存在
	if (!pList)
	{
		printf("list not exist!\n");
		return;
	}

	Node *cur = pList->next;
	while (cur != NULL)
	{
		printf("%s ", cur->user.userName);
		cur = cur->next;
	}
	printf("\n");
}
void menuInterface()
{
		printf("------------------------------------------\n");
		printf("	     1. 查看链表中用户                 \n");
		printf("	     2. 头部插入用户                         \n");
		printf("	     3. 尾部插入用户                         \n");
		printf("	     4. 删除用户                 \n");
		printf("	     5. 退出                         \n");
		printf("请选择操作～                    \n");
		printf("------------------------------------------\n\n\n");
}