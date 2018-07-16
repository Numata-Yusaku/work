#include <stdio.h>

#include "task_nm.h"

static F_NM_RECVFUNCPTR nm_get_recvfunc( int iMsgId );


S_NM_RECV_TABLE T_NM_RECV_TABLE[] =
{
	/* MSGID						,	RecvFunc			*/
	{ E_MSGID_TASK_NM_REQ_TEST		,	nm_rcv_test_req		},
	{ E_MSGID_TASK_EXM_RES_TEST		,	nm_rcv_test_res		},
};

/****************************************************************************/
void NM_recv( S_MSG_DATA* recv)
{
	int iMsgId = recv->iMsgid;
	F_NM_RECVFUNCPTR pvRecvFunc = (F_NM_RECVFUNCPTR)NULL;
	
	/* ���s�֐��擾 */
	pvRecvFunc = nm_get_recvfunc( iMsgId );
	if( (void*)NULL != pvRecvFunc)
	{
		/* ��M�֐����s */
		pvRecvFunc( recv );
	}
	
	return;
}

/****************************************************************************/
F_NM_RECVFUNCPTR nm_get_recvfunc( int iMsgId )
{
	F_NM_RECVFUNCPTR func = (F_NM_RECVFUNCPTR)NULL;
	int iNum = 0;
	int iCount = 0;
	S_NM_RECV_TABLE* psRecvTable = (S_NM_RECV_TABLE*)NULL;
	
	/* ��M�e�[�u���ݒ� */
	psRecvTable =T_NM_RECV_TABLE;
	if((S_NM_RECV_TABLE*)NULL == psRecvTable )
	{
		return (F_NM_RECVFUNCPTR)NULL;
	}
	
	/* ��M�e�[�u���v�f���擾 */
	iNum = sizeof( T_NM_RECV_TABLE ) / sizeof( S_NM_RECV_TABLE );
	if( 0 >= iNum )
	{
		return (F_NM_RECVFUNCPTR)NULL;
	}
	
	/* ��M�֐��T�� */
	for(iCount = 0; iCount < iNum; iCount++)
	{
		if( iMsgId == psRecvTable[iCount].iMsgId )
		{
			func = psRecvTable[iCount].func;
			break;
		}
	}
	
	return func;
}

void nm_rcv_test_req( S_MSG_DATA* recv )
{
	nm_send_test_req( recv );
	
	return;
}

void nm_rcv_test_res( S_MSG_DATA* recv )
{
	nm_send_test_res( recv );
	
	return;
}

