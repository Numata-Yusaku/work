#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

#include "msg_com.h"

S_MSG_QUE pgQue[E_TASKID_NUM];
int iMsgNum[E_MSGLOG_FILEKIND_NUM];

static void msg_init_que( void );
void msg_init_msglog( void );
static int msg_send( int iTask, S_MSG_DATA* psPara );
static int msg_recv( int iTask, S_MSG_DATA* psPara );
static int msg_set_msglog( int iTask, S_MSG_QUE* psQue );
static void msg_set_msglog_msgcom( int iTask, S_MSG_QUE* psQue );
static void msg_set_msglog_que( int iTask, S_MSG_QUE* psQue );

S_MSG_LOGFILE T_MSG_MSGLOGFILE[E_MSGLOG_FILEKIND_NUM] =
{
	{ E_MSGLOG_FILEKIND_FILEIOCOM		,D_FILEIO_MODE_ON	,	msg_set_msglog_msgcom	,	"msgcom.csv"},
	{ E_MSGLOG_FILEKIND_QUE			,D_FILEIO_MODE_OFF	,	msg_set_msglog_que		,	"que.csv"},
};
/****************************************************************************/
void MSGCOM_init( void )
{
	/* �L���[�̏����� */
	msg_init_que();
	
	/* ���b�Z�[�W���O */
	msg_init_msglog();
	
	return;
}

int MSGCOM_send( int iTask, S_MSG_DATA* psPara )
{
	int iRet = D_FILEIO_OK;
	
	iRet = msg_send( iTask, psPara );
	
	return iRet;
}

int MSGCOM_recv( int iTask, S_MSG_DATA* psPara )
{
	int iRet = D_FILEIO_OK;
	
	iRet = msg_recv( iTask, psPara );
	
	return iRet;
}
/****************************************************************************/
void msg_init_que( void )
{
	memset( pgQue, 0x00, ( sizeof( S_MSG_QUE )* E_TASKID_NUM) );
	
	return;
}

void msg_init_msglog( void )
{
	FILE* pMsgLog[E_MSGLOG_FILEKIND_NUM];
	S_MSG_LOGFILE* psFileList = (S_MSG_LOGFILE*)NULL;
	char aFilePath[D_FILEIO_MSGLOG_OUTPUTDIR_SIZE + D_FILEIO_MSGLOG_FILENAME_MAX];
	
	/* �O���[�o���ϐ������� */
	memset( &iMsgNum, 0x00,sizeof( iMsgNum ) );
	
	/* �I�[�g�ϐ������� */
	memset( pMsgLog, 0x00, sizeof( FILE* ) * E_MSGLOG_FILEKIND_NUM );
	memset( &aFilePath, 0x00, sizeof( aFilePath ) );
	
	/* �e�[�u���ݒ� */
	psFileList = T_MSG_MSGLOGFILE;
	if( (S_MSG_LOGFILE*)NULL == psFileList )
	{
		return;
	}
	
	/* �t�@�C���I�[�v�� */
	for( int i = 0; i < E_MSGLOG_FILEKIND_NUM; i++ )
	{
		/* �o�b�t�@�N���A */
		memset( &aFilePath, 0x00, sizeof( aFilePath ) );
		
		if( D_FILEIO_MODE_ON != psFileList[i].iMode )
		{
			continue;
		}
		/* �t�@�C���p�X�A�� */
		strcat( aFilePath, D_FILEIO_MSGLOG_OUTPUTDIR);
		strcat( aFilePath, psFileList[i].cFileName);
		
		/* �t�@�C���I�[�v�� */
		pMsgLog[i] = fopen( (const char*)(&aFilePath), "w" );
		if( (FILE*)NULL != pMsgLog[i] )
		{
			fclose(pMsgLog[i]);
			pMsgLog[i] = (FILE*)NULL;
		}
		else
		{
			return;
		}
	}
	
	return;
}

int msg_send( int iTask, S_MSG_DATA* psPara )
{
	S_MSG_QUE* psQue = (S_MSG_QUE*)NULL;
	S_MSG_DATA* psSend = (S_MSG_DATA*)NULL;
	void* psData = (void*)NULL;
	
	/* �����`�F�b�N */
	if(E_TASKID_NUM < iTask ||
		(S_MSG_DATA*)NULL == psPara )
	{
		return D_FILEIO_NG;
	}
	
	/* �̈�m�� */
	psSend = (S_MSG_DATA*)malloc( sizeof( S_MSG_DATA ) );
	if((S_MSG_DATA*)NULL == psSend)
	{
		goto END;
	}
	memset( psSend, 0x00, sizeof( S_MSG_DATA ) );
	
	/* �L���[�擾 */
	psQue = &pgQue[iTask];
	if( (S_MSG_QUE*)NULL == psQue )
	{
		goto END;
	}
	
	/* �L���[��� */
	if( D_FILEIO_QUEMAX <= psQue->stStatus.iNum )
	{
		printf( "que over: %08x\n",iTask);
		goto END;
	}
	
	/* ���M�f�[�^�ޔ� */
	psSend->iMsgid = psPara->iMsgid;
	psSend->iSize = psPara->iSize;
	if( 0 >= psSend->iSize )
	{
		goto END;
	}
	
	psData = (void*)malloc( psSend->iSize );
	if ((void*)NULL == psData)
	{
		goto END;
	}
	memset( psData, 0x00, psSend->iSize );
	memcpy( psData, psPara->vpPara, psSend->iSize );
	psSend->vpPara = psData;
	
	psQue->stStatus.iFlag = D_FILEIO_MODE_ON;
	/************/
	
	/*** �f�[�^�ݒ� ***/
	/* ID */
	psQue->stData[psQue->stStatus.iTail].iMsgid = psSend->iMsgid;
	/* �p�����[�^�T�C�Y */
	psQue->stData[psQue->stStatus.iTail].iSize = psSend->iSize;
	/* �p�����[�^�A�h���X */
	psQue->stData[psQue->stStatus.iTail].vpPara = psSend->vpPara;
	
	/* MSG���O */
	msg_set_msglog_msgcom( iTask, psQue );
	
	/*** ��ԍX�V ***/
	/* �L���[�� */
	psQue->stStatus.iNum++;
	/* �I�[ */
	psQue->stStatus.iTail++;
	
	
	/* �I�[�̏ꍇ�͐擪�ɖ߂� */
	if (D_FILEIO_QUEMAX == psQue->stStatus.iTail)
	{
		psQue->stStatus.iTail = 0;
	}
	
	/* �L���[���O */
	msg_set_msglog_que( iTask, psQue );
	
	/************/
	psQue->stStatus.iFlag = D_FILEIO_MODE_OFF;
	
	
	/*** ������� ***/
	if ((S_MSG_DATA*)NULL != psSend)
	{
		free( psSend );
		psSend = (S_MSG_DATA*)NULL;
	}
	
	return D_FILEIO_OK;

END:
	/*** ������� ***/
	if ( (S_MSG_DATA*)NULL != psSend)
	{
		free( psSend );
		psSend = (S_MSG_DATA*)NULL;
	}
	
	if ((void*)NULL == psData)
	{
		free( psData );
		psData = (S_MSG_DATA*)NULL;
	}
	
	return D_FILEIO_NG;
}

int msg_recv( int iTask, S_MSG_DATA* psPara )
{
	int iRet = D_FILEIO_OK;
	S_MSG_QUE* psQue = (S_MSG_QUE*)NULL;
	S_MSG_DATA* psRecv = (S_MSG_DATA*)NULL;
	void* psData = (void*)NULL;
	
	/* �����`�F�b�N */
	if ( E_TASKID_NUM < iTask ||
		(S_MSG_DATA*)NULL == psPara)
	{
		return D_FILEIO_NG;
	}
	
	/* �̈�m�� */
	psRecv = (S_MSG_DATA*)malloc( sizeof( S_MSG_DATA ) );
	if ((S_MSG_DATA*)NULL == psRecv)
	{
		goto END;
	}
	memset( psRecv, 0x00, sizeof( S_MSG_DATA ) );
	
	/* �L���[�擾 */
	psQue = &pgQue[iTask];
	if ((S_MSG_QUE*)NULL == psQue)
	{
		goto END;
	}
	
	/* �C�x���g�Ȃ� */
	if (0 == psQue->stStatus.iNum)
	{
		goto END;
	}
	
	psQue->stStatus.iFlag = D_FILEIO_MODE_ON;
	/************/
	/*** �f�[�^�擾 ***/
	/* ID */
	psRecv->iMsgid = psQue->stData[psQue->stStatus.iHead].iMsgid;
	/* �p�����[�^�T�C�Y */
	psRecv->iSize = psQue->stData[psQue->stStatus.iHead].iSize;
	/* �p�����[�^�A�h���X */
	psRecv->vpPara = psQue->stData[psQue->stStatus.iHead].vpPara;
	
	psData = (void*)malloc( psRecv->iSize );
	if ((void*)NULL == psData)
	{
		goto END;
	}
	memset( psData, 0x00, psRecv->iSize );
	memcpy( psData, psRecv->vpPara, psRecv->iSize );
	
	/* �o�͂ɐݒ� */
	/* ID */
	psPara->iMsgid = psRecv->iMsgid;
	/* �p�����[�^�T�C�Y */
	psPara->iSize = psRecv->iSize;
	/* �p�����[�^�A�h���X */
	psPara->vpPara = psData;
	
	/*** �f�[�^�N���A ***/
	/* ID */
	psQue->stData[psQue->stStatus.iHead].iMsgid = D_FILEIO_MSGID_INVALID;
	/* �p�����[�^�T�C�Y */
	psQue->stData[psQue->stStatus.iHead].iSize = 0;
	/* �p�����[�^�A�h���X */
	if( (void*)NULL != psQue->stData[psQue->stStatus.iHead].vpPara)
	{
		free( psQue->stData[psQue->stStatus.iHead].vpPara );
		psQue->stData[psQue->stStatus.iHead].vpPara = (void*)NULL;
		psRecv->vpPara = (void*)NULL;
	}
	
	/*** ��ԍX�V ***/
	/* �L���[�� */
	psQue->stStatus.iNum--;
	/* �擪 */
	psQue->stStatus.iHead++;
	
	/* �I�[�̏ꍇ�͐擪�ɖ߂� */
	if (D_FILEIO_QUEMAX == psQue->stStatus.iHead)
	{
		psQue->stStatus.iHead = 0;
	}
	
	/************/
	psQue->stStatus.iFlag = D_FILEIO_MODE_OFF;
	
	return D_FILEIO_OK;

END:
	/*** ������� ***/
	if ((S_MSG_DATA*)NULL != psRecv)
	{
		free( psRecv );
		psRecv = (S_MSG_DATA*)NULL;
	}
	
	if ((void*)NULL == psData)
	{
		free( psData );
		psData = (S_MSG_DATA*)NULL;
	}
	
	return D_FILEIO_NG;
}

int msg_set_msglog( int iTask, S_MSG_QUE* psQue )
{
	S_MSG_LOGFILE* psFileList = (S_MSG_LOGFILE*)NULL;
	F_MSG_MSGLOGPTR pFunc = (F_MSG_MSGLOGPTR)NULL;

	/* �����`�F�b�N */
	if(E_TASKID_NUM < iTask ||
		(S_MSG_QUE*)NULL == psQue )
	{
		return D_FILEIO_NG;
	}
	
	/* �e�[�u���ݒ� */
	psFileList = T_MSG_MSGLOGFILE;
	if( (S_MSG_LOGFILE*)NULL == psFileList )
	{
		return D_FILEIO_NG;
	}
	
	/* ���O�o�� */
	for( int i = 0; i < E_MSGLOG_FILEKIND_NUM; i++ )
	{
		if( D_FILEIO_MODE_ON != psFileList[i].iMode )
		{
			continue;
		}
		
		/* ���O�o�͊֐��擾 */
		pFunc = psFileList[i].pvFunc;
		
		/* ���O�o�͊֐��R�[�� */
		if( (F_MSG_MSGLOGPTR)NULL != pFunc )
		{
			pFunc( iTask, psQue );
		}
	}
	
	return D_FILEIO_OK;
}

void msg_set_msglog_msgcom( int iTask, S_MSG_QUE* psQue )
{
	int iFileKind = E_MSGLOG_FILEKIND_FILEIOCOM;
	FILE* pFile = (FILE*)NULL;
	S_MSG_LOGFILE* psFileList = (S_MSG_LOGFILE*)NULL;
	time_t stTime;
	struct tm* psLocalTime = NULL;
	char aFilePath[D_FILEIO_MSGLOG_OUTPUTDIR_SIZE + D_FILEIO_MSGLOG_FILENAME_MAX];
	
	/* ������ */
	memset( &stTime, 0x00, sizeof(time_t) );
	memset( &aFilePath, 0x00, sizeof( aFilePath ) );
	
	/* �e�[�u���ݒ� */
	psFileList = T_MSG_MSGLOGFILE;
	if( (S_MSG_LOGFILE*)NULL == psFileList )
	{
		goto END;
	}
	
	if( D_FILEIO_MODE_ON != psFileList[iFileKind].iMode )
	{
		goto END;
	}
	
	/* �t�@�C���p�X�A�� */
	strcat( aFilePath, D_FILEIO_MSGLOG_OUTPUTDIR);
	strcat( aFilePath, psFileList[iFileKind].cFileName);
		
	/* �t�@�C���I�[�v�� */
	pFile = fopen( (const char*)(&aFilePath), "a" );
	if( (FILE*)NULL == pFile )
	{
		goto END;
	}
	
	/* �w�b�_�o�� */
	if( 0 == iMsgNum[iFileKind])
	{
		fprintf(pFile,"No,Date,Time,MSGID\n");
	}
	
	/*** ���O�o�� ***/
	/* No */
	iMsgNum[iFileKind]++;
	fprintf(pFile,"%08d,",iMsgNum[iFileKind]);
	
	/* ���� */
	time( &stTime );
	psLocalTime = localtime( &stTime );
	
	fprintf(pFile,"%04d", (psLocalTime->tm_year + 1900) );	/* �N�F[1900����̌o�ߔN��] */
	fprintf(pFile,"%02d", (psLocalTime->tm_mon + 1) );		/* ���F[0-11] + 1 */
	fprintf(pFile,"%02d,", psLocalTime->tm_mday );			/* ���F[1-31] */
	fprintf(pFile,"%02d", psLocalTime->tm_hour );			/* ���F[0-23] */
	fprintf(pFile,"%02d", psLocalTime->tm_min );			/* ���F[0-59] */
	fprintf(pFile,"%02d,", psLocalTime->tm_sec );			/* �b�F[0-61] �ő�2�b�܂ł̂��邤�b���l�� */
	
	/* MSGID */
	fprintf(pFile,"%08x\n",psQue->stData[psQue->stStatus.iTail].iMsgid );
	
END:
	
	if( (FILE*)NULL != pFile )
	{
		fclose(pFile);
		pFile = (FILE*)NULL;
	}
	
	return;
}

void msg_set_msglog_que( int iTask, S_MSG_QUE* psQue )
{
	int iFileKind = E_MSGLOG_FILEKIND_QUE;
	FILE* pFile = (FILE*)NULL;
	S_MSG_LOGFILE* psFileList = (S_MSG_LOGFILE*)NULL;
	time_t stTime;
	struct tm* psLocalTime = NULL;
	char aFilePath[D_FILEIO_MSGLOG_OUTPUTDIR_SIZE + D_FILEIO_MSGLOG_FILENAME_MAX];
	
	/* ������ */
	memset( &stTime, 0x00, sizeof(time_t) );
	memset( &aFilePath, 0x00, sizeof( aFilePath ) );
	
	/* �e�[�u���ݒ� */
	psFileList = T_MSG_MSGLOGFILE;
	if( (S_MSG_LOGFILE*)NULL == psFileList )
	{
		goto END;
	}
	
	if( D_FILEIO_MODE_ON != psFileList[iFileKind].iMode )
	{
		goto END;
	}
	
	/* �t�@�C���p�X�A�� */
	strcat( aFilePath, D_FILEIO_MSGLOG_OUTPUTDIR);
	strcat( aFilePath, psFileList[iFileKind].cFileName);
		
	/* �t�@�C���I�[�v�� */
	pFile = fopen( (const char*)(&aFilePath), "a" );
	if( (FILE*)NULL == pFile )
	{
		goto END;
	}
	
	/* �w�b�_�o�� */
	if( 0 == iMsgNum[iFileKind])
	{
		fprintf(pFile,"No,Date,Time,QueNum\n");
	}
	
	/*** ���O�o�� ***/
	/* No */
	iMsgNum[iFileKind]++;
	fprintf(pFile,"%08d,",iMsgNum[iFileKind]);
	
	/* ���� */
	time( &stTime );
	psLocalTime = localtime( &stTime );
	
	fprintf(pFile,"%04d", (psLocalTime->tm_year + 1900) );	/* �N�F[1900����̌o�ߔN��] */
	fprintf(pFile,"%02d", (psLocalTime->tm_mon + 1) );		/* ���F[0-11] + 1 */
	fprintf(pFile,"%02d,", psLocalTime->tm_mday );			/* ���F[1-31] */
	fprintf(pFile,"%02d", psLocalTime->tm_hour );			/* ���F[0-23] */
	fprintf(pFile,"%02d", psLocalTime->tm_min );			/* ���F[0-59] */
	fprintf(pFile,"%02d,", psLocalTime->tm_sec );			/* �b�F[0-61] �ő�2�b�܂ł̂��邤�b���l�� */
	
	/* Que�� */
	//fprintf(pFile,"%08x\n",psQue->stData[psQue->stStatus.iTail].iMsgid );
	fprintf(pFile,"%04d\n",psQue->stStatus.iNum );
	
END:
	
	if( (FILE*)NULL != pFile )
	{
		fclose(pFile);
		pFile = (FILE*)NULL;
	}
	
	return;
}

