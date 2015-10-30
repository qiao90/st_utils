#ifndef __ST_SLIST_H
#define __ST_SLIST_H

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define st_d_print(...) \
	do{ fprintf( stderr,"DEBUG:%s|%s<%d>\n",__FILE__, __FUNCTION__,__LINE__); \
		fprintf( stderr , __VA_ARGS__ ); \
	}while(0)
#define st_print(...) fprintf( stderr , __VA_ARGS__ )
#else
#define st_d_print(...) \
	do {} while(0)
#define st_print(...) fprintf( stderr , __VA_ARGS__ )
#endif

//����list_head�򻯵õ��ĵ���NULL��β����
typedef struct _slist_head {
	struct _slist_head *next;
} SLIST_HEAD, *P_SLIST_HEAD;

static inline void slist_init(P_SLIST_HEAD head)
{
	head->next = NULL;
}

// replaced of container_of
#define list_entry(ptr, type, member) \
((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))  

#define slist_for_each(pos, head) \
	for (pos = (head)->next; pos != NULL; pos = pos->next)  

#define slist_for_each_entry(pos, head, member)\
	for (pos = list_entry((head)->next, typeof(*pos), member);  \
        &pos->member != NULL;								\
         pos = list_entry(pos->member.next, typeof(*pos), member))


static inline void slist_add(P_SLIST_HEAD new_item, P_SLIST_HEAD head)
{
	P_SLIST_HEAD ptr =  head;
	while(ptr->next)
		ptr = ptr->next;
	ptr->next = new_item;
	new_item->next = NULL;

	//new_item->next = head->next;
	//head->next = new_item;
}

static inline int slist_empty(const P_SLIST_HEAD head)
{
	return head->next == NULL;
}

static inline unsigned int slist_count (const P_SLIST_HEAD head)
{
	int count = 0;
	P_SLIST_HEAD pos;

	if(!head)
		return -1;

	slist_for_each(pos, head)
		count ++;

	return count;
}

// �Ƚ��ȳ��Ķ���
static inline P_SLIST_HEAD slist_fetch(P_SLIST_HEAD head)
{
    P_SLIST_HEAD ret = NULL;

    if (!head || slist_empty(head))
        return NULL;
    
    ret = head->next;
    head->next = head->next->next;  //may also be null

    return ret;
}

static inline int slist_u_test(void)
{
	typedef struct _test_struct 
	{
		SLIST_HEAD list;
		int data;
	} TEST_STRUCT, *P_TEST_STRUCT;

	int tmp = 0;
	
	SLIST_HEAD test_head = {0};
	P_SLIST_HEAD p_test_head = &test_head;
	P_SLIST_HEAD pos = NULL; 

	tmp = slist_empty(p_test_head);
	if(!tmp)
	{
		fprintf(stderr,"slist_empty test FAILED!\n");
		return 0;
	}
	else
		fprintf(stderr,"slist_empty test PASS!\n");

	tmp = slist_count(p_test_head);
	if(tmp != 0 )
	{
		fprintf(stderr,"slist_count test FAILED!\n");
		return 0;
	}

	int i = 1, data;
	for(i = 1; i <= 10; i++)  
	{   
		data = (11 - i) * 10;  //data 

		P_TEST_STRUCT p_node = (P_TEST_STRUCT)malloc(sizeof(TEST_STRUCT));
		p_node->data = data;
		slist_add(&p_node->list, p_test_head);

		tmp = slist_count(p_test_head);
		if(tmp != i)
		{
			fprintf(stderr,"slist_count test FAIL!\n");
			return 0;
		}
	}   
	
	fprintf(stderr,"slist_count test PASS!\n");
    
	i = 10;
	slist_for_each(pos, p_test_head)  
	{   
		P_TEST_STRUCT p_node = list_entry(pos, TEST_STRUCT, list);   
		tmp = i-- * 10;
		if(tmp != p_node->data)
		{
            fprintf(stderr,"list_entry test FAIL!\n");
            fprintf(stderr,"%d <-> %d \n", tmp, p_node->data);
            return 0;
		}
	}   

    fprintf(stderr,"list_entry test PASS!\n");

    P_TEST_STRUCT p_node = NULL;

    slist_for_each_entry(p_node, p_test_head, list)
        st_print("%d\n", p_node->data);

	//pos = slist_fetch(p_test_head);
    //pos = slist_fetch(p_test_head);
    pos = slist_fetch(p_test_head);
    p_node = list_entry(pos, TEST_STRUCT, list);
    if (p_node->data != 100)
    {
        fprintf(stderr,"list_fetch test FAIL!\n");
        fprintf(stderr,"%d \n", p_node->data);
        return 0;
    }
    free(p_node);

    //���ԣ��������ڴ�й©
    pos = slist_fetch(p_test_head); //90
    pos = slist_fetch(p_test_head); //80
    pos = slist_fetch(p_test_head); //70
    pos = slist_fetch(p_test_head); //60
    p_node = list_entry(pos, TEST_STRUCT, list);
    if (p_node->data != 60)
    {
        fprintf(stderr,"list_fetch test FAIL!\n");
        fprintf(stderr,"%d \n", p_node->data);
        return 0;
    }
    free(p_node);

    slist_for_each_entry(p_node, p_test_head, list)
        st_print("%d\n", p_node->data);

    pos = slist_fetch(p_test_head); //50
    pos = slist_fetch(p_test_head); //40
    pos = slist_fetch(p_test_head); //30
    pos = slist_fetch(p_test_head); //20
    pos = slist_fetch(p_test_head); //10
    p_node = list_entry(pos, TEST_STRUCT, list);
    if (p_node->data != 10)
    {
        fprintf(stderr,"list_fetch test FAIL!\n");
        fprintf(stderr,"%d \n", p_node->data);
        return 0;
    }
    free(p_node);

    slist_for_each_entry(p_node, p_test_head, list)
        st_print("%d\n", p_node->data);

    if (slist_fetch(p_test_head) != NULL)
    {
        fprintf(stderr,"list_fetch test FAIL!\n");
        return 0;
    }
	fprintf(stderr,"list_fetch test PASS!\n");

	fprintf(stderr,"slist unit test PASS!");
}
#endif //ST_SLIST_H