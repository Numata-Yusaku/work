#include <stdio.h>

#include "task_mnt.h"

static F_MNT_RECVFUNCPTR mnt_get_recvfunc( int iMsgId );


S_MNT_RECV_TABLE T_MNT_RECV_TABLE[] =
{
	/* MSGID						,	RecvFunc			*/
	{ E_MSGID_TASK_MNT_REQ_TEST		,	mnt_rcv_test_req		},
};

/****************************************************************************/
void MNT_recv( S_MSG_DATA* recv)
{
	int iMsgId = recv->iMsgid;
	F_MNT_RECVFUNCPTR pvRecvFunc = (F_MNT_RECVFUNCPTR)NULL;
	
	/* 実行関数取得 */
	pvRecvFunc = mnt_get_recvfunc( iMsgId );
	if( (void*)NULL != pvRecvFunc)
	{
		/* 受信関数実行 */
		pvRecvFunc( recv );
	}
	
	return;
}

/****************************************************************************/
F_MNT_RECVFUNCPTR mnt_get_recvfunc( int iMsgId )
{
	F_MNT_RECVFUNCPTR func = (F_MNT_RECVFUNCPTR)NULL;
	int iNum = 0;
	int iCount = 0;
	S_MNT_RECV_TABLE* psRecvTable = (S_MNT_RECV_TABLE*)NULL;
	
	/* 受信テーブル設定 */
	psRecvTable =T_MNT_RECV_TABLE;
	if((S_MNT_RECV_TABLE*)NULL == psRecvTable )
	{
		return (F_MNT_RECVFUNCPTR)NULL;
	}
	
	/* 受信テーブル要素数取得 */
	iNum = sizeof( T_MNT_RECV_TABLE ) / sizeof( S_MNT_RECV_TABLE );
	if( 0 >= iNum )
	{
		return (F_MNT_RECVFUNCPTR)NULL;
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

void mnt_rcv_test_req( S_MSG_DATA* recv )
{
	mnt_send_test_res( recv );
	
	return;
}

