#include <stdio.h>
#include <Windows.h>

#include "task_exm.h"

void TASK_EXM_Thread( void )
{
	int		iRet = D_TASK_EXM_NG;
	int		iMsgId = E_MSGID_TASK_EXM_INVALID;
	S_MSG_DATA* psRecvData = (S_MSG_DATA*)NULL;
	
	/* 領域確保 */
	psRecvData = (S_MSG_DATA*)malloc( sizeof( S_MSG_DATA ) );
	if ((S_MSG_DATA*)NULL == psRecvData)
	{
		goto END;
	}
	
	/* 初期化 */
	memset( psRecvData, 0x00, sizeof( S_MSG_DATA ) );
	
	/* EXM_TASK */
	while (1)
	{
		iRet = MSGCOM_recv( E_TASKID_EXM, psRecvData );
		
		if (E_MSGID_TASK_EXM_INVALID != psRecvData->iMsgid)
		{
			printf( "recv:0x%08x,TASK_EXM\n", psRecvData->iMsgid );
			
			/* 受信処理 */
			EXM_recv( psRecvData );
			
			/* 受信データクリア */
			if ((void*)NULL != psRecvData->vpPara)
			{
				free( psRecvData->vpPara );
				psRecvData->vpPara = (void*)NULL;
			}
			memset( psRecvData, 0x00, sizeof( S_MSG_DATA ) );
		}
		
		/* タスクサイクル(ms) */
		Sleep( D_FILEIO_TASKCICLE_100 );
	}

END:
	/*** 解放処理 ***/
	if ((S_MSG_DATA*)NULL != psRecvData)
	{
		free( psRecvData );
		psRecvData = (S_MSG_DATA*)NULL;
	}
	return;
}
