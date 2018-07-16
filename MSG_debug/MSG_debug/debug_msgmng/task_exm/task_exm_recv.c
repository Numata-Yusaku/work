#include <stdio.h>

#include "task_exm.h"

static F_EXM_RECVFUNCPTR exm_get_recvfunc( int iMsgId );


S_EXM_RECV_TABLE T_EXM_RECV_TABLE[] =
{
	/* MSGID						,	RecvFunc			*/
	{ E_MSGID_TASK_EXM_REQ_TEST		,	exm_rcv_test_req		},
	{ E_MSGID_TASK_MNT_RES_TEST		,	exm_rcv_test_res		},
};

/****************************************************************************/
void EXM_recv( S_MSG_DATA* recv)
{
	int iMsgId = recv->iMsgid;
	F_EXM_RECVFUNCPTR pvRecvFunc = (F_EXM_RECVFUNCPTR)NULL;
	
	/* 実行関数取得 */
	pvRecvFunc = exm_get_recvfunc( iMsgId );
	if( (void*)NULL != pvRecvFunc)
	{
		/* 受信関数実行 */
		pvRecvFunc( recv );
	}
	
	return;
}

/****************************************************************************/
F_EXM_RECVFUNCPTR exm_get_recvfunc( int iMsgId )
{
	F_EXM_RECVFUNCPTR func = (F_EXM_RECVFUNCPTR)NULL;
	int iNum = 0;
	int iCount = 0;
	S_EXM_RECV_TABLE* psRecvTable = (S_EXM_RECV_TABLE*)NULL;
	
	/* 受信テーブル設定 */
	psRecvTable =T_EXM_RECV_TABLE;
	if((S_EXM_RECV_TABLE*)NULL == psRecvTable )
	{
		return (F_EXM_RECVFUNCPTR)NULL;
	}
	
	/* 受信テーブル要素数取得 */
	iNum = sizeof( T_EXM_RECV_TABLE ) / sizeof( S_EXM_RECV_TABLE );
	if( 0 >= iNum )
	{
		return (F_EXM_RECVFUNCPTR)NULL;
	}
	
	/* 受信関数探索 */
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

void exm_rcv_test_req( S_MSG_DATA* recv )
{
	exm_send_test_req( recv );
	
	return;
}

void exm_rcv_test_res( S_MSG_DATA* recv )
{
	exm_send_test_res( recv );
	
	return;
}

