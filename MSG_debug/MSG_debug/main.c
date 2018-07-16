#include <stdio.h>
#include <Windows.h>

#include "main.h"

static void main_make_thread( void );

int main( void )
{
	int iRet = D_MAIN_NG;
	int iMsgId = E_MSGID_MAIN_INVALID;
	S_MSG_DATA* psRecvData = (S_MSG_DATA*)NULL;
	
	/* イベントキュー初期化 */
	MSGCOM_init();
	
	/* スレッド生成 */
	main_make_thread();
	
	/* 領域確保 */
	psRecvData = (S_MSG_DATA*)malloc( sizeof( S_MSG_DATA ) );
	if((S_MSG_DATA*)NULL == psRecvData)
	{
		goto END;
	}
	/* 初期化 */
	memset( psRecvData, 0x00, sizeof( S_MSG_DATA ) );
	
	/* MAIN_TASK */
	while (1)	/* loop sta*/
	{
		/* MSG受信 */
		iRet = MSGCOM_recv( E_TASKID_MAIN, psRecvData );
		
		if (E_MSGID_MAIN_INVALID != psRecvData->iMsgid)
		{
			printf( "recv:0x%08x,MAIN\n", psRecvData->iMsgid );
			
			/* 受信処理 */
			MAIN_recv( psRecvData );
			
			/* 受信データクリア */
			if ((void*)NULL != psRecvData->vpPara)
			{
				free( psRecvData->vpPara );
				psRecvData->vpPara = (void*)NULL;
			}
			memset( psRecvData, 0x00, sizeof( S_MSG_DATA ) );
		}
		
		/* キー入力の監視 */
		MAIN_recv_cmd();
		
	}			/* loop end */
	
	return D_MAIN_OK;

END:
	/*** 解放処理 ***/
	if ((S_MSG_DATA*)NULL != psRecvData)
	{
		free( psRecvData );
		psRecvData = (S_MSG_DATA*)NULL;
	}
	
	return D_MAIN_NG;
}

void main_make_thread( void )
{
	int iMsgThId = 0;
	HANDLE	hNThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)TASK_N_Thread, NULL, 0, &iMsgThId );
	HANDLE	hNMThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)TASK_NM_Thread, NULL, 0, &iMsgThId );
	HANDLE	hEXMThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)TASK_EXM_Thread, NULL, 0, &iMsgThId );
	HANDLE	hMNTThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)TASK_MNT_Thread, NULL, 0, &iMsgThId );
	
	return;
}

