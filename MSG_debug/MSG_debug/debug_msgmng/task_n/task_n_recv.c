#include <stdio.h>

#include "task_n.h"

static F_N_RECVFUNCPTR n_get_recvfunc( int iMsgId );


S_N_RECV_TABLE T_N_RECV_TABLE[] =
{
	/* MSGID						,	RecvFunc			*/
	{ E_MSGID_TASK_N_REQ_TEST		,	n_rcv_test_req		},
	{ E_MSGID_TASK_NM_RES_TEST		,	n_rcv_test_res		},
};

/****************************************************************************/
void N_recv( S_MSG_DATA* recv)
{
	int iMsgId = recv->iMsgid;
	F_N_RECVFUNCPTR pvRecvFunc = (F_N_RECVFUNCPTR)NULL;
	
	/* ���s�֐��擾 */
	pvRecvFunc = n_get_recvfunc( iMsgId );
	if( (void*)NULL != pvRecvFunc)
	{
		/* ��M�֐����s */
		pvRecvFunc( recv );
	}
	
	return;
}

/****************************************************************************/
F_N_RECVFUNCPTR n_get_recvfunc( int iMsgId )
{
	F_N_RECVFUNCPTR func = (F_N_RECVFUNCPTR)NULL;
	int iNum = 0;
	int iCount = 0;
	S_N_RECV_TABLE* psRecvTable = (S_N_RECV_TABLE*)NULL;
	
	/* ��M�e�[�u���ݒ� */
	psRecvTable =T_N_RECV_TABLE;
	if((S_N_RECV_TABLE*)NULL == psRecvTable )
	{
		return (F_N_RECVFUNCPTR)NULL;
	}
	
	/* ��M�e�[�u���v�f���擾 */
	iNum = sizeof( T_N_RECV_TABLE ) / sizeof( S_N_RECV_TABLE );
	if( 0 >= iNum )
	{
		return (F_N_RECVFUNCPTR)NULL;
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

void n_rcv_test_req( S_MSG_DATA* recv )
{
	n_send_test_req( recv );
	
	return;
}

void n_rcv_test_res( S_MSG_DATA* recv )
{
	n_send_test_res( recv );
	
	return;
}

