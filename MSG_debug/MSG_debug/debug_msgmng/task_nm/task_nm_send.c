#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task_nm.h"


void nm_send_test_req( S_MSG_DATA* send )
{
	int iRet = D_TASK_NM_NG;
	S_MSG_DATA* psSendData = (S_MSG_DATA*)NULL;
	void* psSendPara = (void*)NULL;
	
	/* �̈�m�� */
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
	
	/* ������ */
	memset( psSendData, 0x00, sizeof( S_MSG_DATA ) );
	memset( psSendPara, 0x00, sizeof( int ) );
	
	/* ���M�f�[�^�ݒ� */
	psSendData->iMsgid = E_MSGID_TASK_EXM_REQ_TEST;
	psSendData->iSize = sizeof( S_MSG_DATA );
	psSendData->vpPara = psSendPara;
	
	/* MSG���M */
	iRet = MSGCOM_send( E_TASKID_EXM, psSendData );

END:
	/*** ������� ***/
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

void nm_send_test_res( S_MSG_DATA* send )
{
	int iRet = D_TASK_NM_NG;
	S_MSG_DATA* psSendData = (S_MSG_DATA*)NULL;
	void* psSendPara = (void*)NULL;
	
	/* �̈�m�� */
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
	
	/* ������ */
	memset( psSendData, 0x00, sizeof( S_MSG_DATA ) );
	memset( psSendPara, 0x00, sizeof( int ) );
	
	/* ���M�f�[�^�ݒ� */
	psSendData->iMsgid = E_MSGID_TASK_NM_RES_TEST;
	psSendData->iSize = sizeof( S_MSG_DATA );
	psSendData->vpPara = psSendPara;
	
	/* MSG���M */
	iRet = MSGCOM_send( E_TASKID_N, psSendData );

END:
	/*** ������� ***/
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
