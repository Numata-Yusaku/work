#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task_n.h"


void n_send_test_req( S_MSG_DATA* send )
{
	int iRet = D_TASK_N_NG;
	S_MSG_DATA* psSendData = (S_MSG_DATA*)NULL;
	void* psSendPara = (void*)NULL;
	
	/* 領域確保 */
	psSendData = (S_MSG_DATA*)malloc( sizeof( S_MSG_DATA ) );
	if ((S_MSG_DATA*)NULL == psSendData)
	{
		goto END;
	}
	
	psSendPara = (void*)malloc( sizeof( int ) );
	if ((void*)NULL == psSendPara)
	{
		goto END;
	}
	
	/* 初期化 */
	memset( psSendData, 0x00, sizeof( S_MSG_DATA ) );
	memset( psSendPara, 0x00, sizeof( int ) );
	
	/* 送信データ設定 */
	psSendData->iMsgid = E_MSGID_TASK_NM_REQ_TEST;
	psSendData->iSize = sizeof( S_MSG_DATA );
	psSendData->vpPara = psSendPara;
	
	/* MSG送信 */
	iRet = MSGCOM_send( E_TASKID_NM, psSendData );

END:
	/*** 解放処理 ***/
	if ((void*)NULL != psSendPara)
	{
		free( psSendPara );
		psSendPara = (void*)NULL;
	}
	
	if ((S_MSG_DATA*)NULL != psSendData)
	{
		free( psSendData );
		psSendData = (S_MSG_DATA*)NULL;
	}
	
	return;
}

void n_send_test_res( S_MSG_DATA* send )
{
	int iRet = D_TASK_N_NG;
	S_MSG_DATA* psSendData = (S_MSG_DATA*)NULL;
	void* psSendPara = (void*)NULL;
	
	/* 領域確保 */
	psSendData = (S_MSG_DATA*)malloc( sizeof( S_MSG_DATA ) );
	if ((S_MSG_DATA*)NULL == psSendData)
	{
		goto END;
	}
	
	psSendPara = (void*)malloc( sizeof( int ) );
	if ((void*)NULL == psSendPara)
	{
		goto END;
	}
	
	/* 初期化 */
	memset( psSendData, 0x00, sizeof( S_MSG_DATA ) );
	memset( psSendPara, 0x00, sizeof( int ) );
	
	/* 送信データ設定 */
	psSendData->iMsgid = E_MSGID_TASK_N_RES_TEST;
	psSendData->iSize = sizeof( S_MSG_DATA );
	psSendData->vpPara = psSendPara;
	
	/* MSG送信 */
	iRet = MSGCOM_send( E_TASKID_MAIN, psSendData );

END:
	/*** 解放処理 ***/
	if ((void*)NULL != psSendPara)
	{
		free( psSendPara );
		psSendPara = (void*)NULL;
	}
	
	if ((S_MSG_DATA*)NULL != psSendData)
	{
		free( psSendData );
		psSendData = (S_MSG_DATA*)NULL;
	}
	
	return;
}

