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
	/* キューの初期化 */
	msg_init_que();
	
	/* メッセージログ */
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
	
	/* グローバル変数初期化 */
	memset( &iMsgNum, 0x00,sizeof( iMsgNum ) );
	
	/* オート変数初期化 */
	memset( pMsgLog, 0x00, sizeof( FILE* ) * E_MSGLOG_FILEKIND_NUM );
	memset( &aFilePath, 0x00, sizeof( aFilePath ) );
	
	/* テーブル設定 */
	psFileList = T_MSG_MSGLOGFILE;
	if( (S_MSG_LOGFILE*)NULL == psFileList )
	{
		return;
	}
	
	/* ファイルオープン */
	for( int i = 0; i < E_MSGLOG_FILEKIND_NUM; i++ )
	{
		/* バッファクリア */
		memset( &aFilePath, 0x00, sizeof( aFilePath ) );
		
		if( D_FILEIO_MODE_ON != psFileList[i].iMode )
		{
			continue;
		}
		/* ファイルパス連結 */
		strcat( aFilePath, D_FILEIO_MSGLOG_OUTPUTDIR);
		strcat( aFilePath, psFileList[i].cFileName);
		
		/* ファイルオープン */
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
	
	/* 引数チェック */
	if(E_TASKID_NUM < iTask ||
		(S_MSG_DATA*)NULL == psPara )
	{
		return D_FILEIO_NG;
	}
	
	/* 領域確保 */
	psSend = (S_MSG_DATA*)malloc( sizeof( S_MSG_DATA ) );
	if((S_MSG_DATA*)NULL == psSend)
	{
		goto END;
	}
	memset( psSend, 0x00, sizeof( S_MSG_DATA ) );
	
	/* キュー取得 */
	psQue = &pgQue[iTask];
	if( (S_MSG_QUE*)NULL == psQue )
	{
		goto END;
	}
	
	/* キュー溢れ */
	if( D_FILEIO_QUEMAX <= psQue->stStatus.iNum )
	{
		printf( "que over: %08x\n",iTask);
		goto END;
	}
	
	/* 送信データ退避 */
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
	
	/*** データ設定 ***/
	/* ID */
	psQue->stData[psQue->stStatus.iTail].iMsgid = psSend->iMsgid;
	/* パラメータサイズ */
	psQue->stData[psQue->stStatus.iTail].iSize = psSend->iSize;
	/* パラメータアドレス */
	psQue->stData[psQue->stStatus.iTail].vpPara = psSend->vpPara;
	
	/* MSGログ */
	msg_set_msglog_msgcom( iTask, psQue );
	
	/*** 状態更新 ***/
	/* キュー数 */
	psQue->stStatus.iNum++;
	/* 終端 */
	psQue->stStatus.iTail++;
	
	
	/* 終端の場合は先頭に戻す */
	if (D_FILEIO_QUEMAX == psQue->stStatus.iTail)
	{
		psQue->stStatus.iTail = 0;
	}
	
	/* キューログ */
	msg_set_msglog_que( iTask, psQue );
	
	/************/
	psQue->stStatus.iFlag = D_FILEIO_MODE_OFF;
	
	
	/*** 解放処理 ***/
	if ((S_MSG_DATA*)NULL != psSend)
	{
		free( psSend );
		psSend = (S_MSG_DATA*)NULL;
	}
	
	return D_FILEIO_OK;

END:
	/*** 解放処理 ***/
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
	
	/* 引数チェック */
	if ( E_TASKID_NUM < iTask ||
		(S_MSG_DATA*)NULL == psPara)
	{
		return D_FILEIO_NG;
	}
	
	/* 領域確保 */
	psRecv = (S_MSG_DATA*)malloc( sizeof( S_MSG_DATA ) );
	if ((S_MSG_DATA*)NULL == psRecv)
	{
		goto END;
	}
	memset( psRecv, 0x00, sizeof( S_MSG_DATA ) );
	
	/* キュー取得 */
	psQue = &pgQue[iTask];
	if ((S_MSG_QUE*)NULL == psQue)
	{
		goto END;
	}
	
	/* イベントなし */
	if (0 == psQue->stStatus.iNum)
	{
		goto END;
	}
	
	psQue->stStatus.iFlag = D_FILEIO_MODE_ON;
	/************/
	/*** データ取得 ***/
	/* ID */
	psRecv->iMsgid = psQue->stData[psQue->stStatus.iHead].iMsgid;
	/* パラメータサイズ */
	psRecv->iSize = psQue->stData[psQue->stStatus.iHead].iSize;
	/* パラメータアドレス */
	psRecv->vpPara = psQue->stData[psQue->stStatus.iHead].vpPara;
	
	psData = (void*)malloc( psRecv->iSize );
	if ((void*)NULL == psData)
	{
		goto END;
	}
	memset( psData, 0x00, psRecv->iSize );
	memcpy( psData, psRecv->vpPara, psRecv->iSize );
	
	/* 出力に設定 */
	/* ID */
	psPara->iMsgid = psRecv->iMsgid;
	/* パラメータサイズ */
	psPara->iSize = psRecv->iSize;
	/* パラメータアドレス */
	psPara->vpPara = psData;
	
	/*** データクリア ***/
	/* ID */
	psQue->stData[psQue->stStatus.iHead].iMsgid = D_FILEIO_MSGID_INVALID;
	/* パラメータサイズ */
	psQue->stData[psQue->stStatus.iHead].iSize = 0;
	/* パラメータアドレス */
	if( (void*)NULL != psQue->stData[psQue->stStatus.iHead].vpPara)
	{
		free( psQue->stData[psQue->stStatus.iHead].vpPara );
		psQue->stData[psQue->stStatus.iHead].vpPara = (void*)NULL;
		psRecv->vpPara = (void*)NULL;
	}
	
	/*** 状態更新 ***/
	/* キュー数 */
	psQue->stStatus.iNum--;
	/* 先頭 */
	psQue->stStatus.iHead++;
	
	/* 終端の場合は先頭に戻す */
	if (D_FILEIO_QUEMAX == psQue->stStatus.iHead)
	{
		psQue->stStatus.iHead = 0;
	}
	
	/************/
	psQue->stStatus.iFlag = D_FILEIO_MODE_OFF;
	
	return D_FILEIO_OK;

END:
	/*** 解放処理 ***/
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

	/* 引数チェック */
	if(E_TASKID_NUM < iTask ||
		(S_MSG_QUE*)NULL == psQue )
	{
		return D_FILEIO_NG;
	}
	
	/* テーブル設定 */
	psFileList = T_MSG_MSGLOGFILE;
	if( (S_MSG_LOGFILE*)NULL == psFileList )
	{
		return D_FILEIO_NG;
	}
	
	/* ログ出力 */
	for( int i = 0; i < E_MSGLOG_FILEKIND_NUM; i++ )
	{
		if( D_FILEIO_MODE_ON != psFileList[i].iMode )
		{
			continue;
		}
		
		/* ログ出力関数取得 */
		pFunc = psFileList[i].pvFunc;
		
		/* ログ出力関数コール */
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
	
	/* 初期化 */
	memset( &stTime, 0x00, sizeof(time_t) );
	memset( &aFilePath, 0x00, sizeof( aFilePath ) );
	
	/* テーブル設定 */
	psFileList = T_MSG_MSGLOGFILE;
	if( (S_MSG_LOGFILE*)NULL == psFileList )
	{
		goto END;
	}
	
	if( D_FILEIO_MODE_ON != psFileList[iFileKind].iMode )
	{
		goto END;
	}
	
	/* ファイルパス連結 */
	strcat( aFilePath, D_FILEIO_MSGLOG_OUTPUTDIR);
	strcat( aFilePath, psFileList[iFileKind].cFileName);
		
	/* ファイルオープン */
	pFile = fopen( (const char*)(&aFilePath), "a" );
	if( (FILE*)NULL == pFile )
	{
		goto END;
	}
	
	/* ヘッダ出力 */
	if( 0 == iMsgNum[iFileKind])
	{
		fprintf(pFile,"No,Date,Time,MSGID\n");
	}
	
	/*** ログ出力 ***/
	/* No */
	iMsgNum[iFileKind]++;
	fprintf(pFile,"%08d,",iMsgNum[iFileKind]);
	
	/* 時刻 */
	time( &stTime );
	psLocalTime = localtime( &stTime );
	
	fprintf(pFile,"%04d", (psLocalTime->tm_year + 1900) );	/* 年：[1900からの経過年数] */
	fprintf(pFile,"%02d", (psLocalTime->tm_mon + 1) );		/* 月：[0-11] + 1 */
	fprintf(pFile,"%02d,", psLocalTime->tm_mday );			/* 日：[1-31] */
	fprintf(pFile,"%02d", psLocalTime->tm_hour );			/* 時：[0-23] */
	fprintf(pFile,"%02d", psLocalTime->tm_min );			/* 分：[0-59] */
	fprintf(pFile,"%02d,", psLocalTime->tm_sec );			/* 秒：[0-61] 最大2秒までのうるう秒を考慮 */
	
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
	
	/* 初期化 */
	memset( &stTime, 0x00, sizeof(time_t) );
	memset( &aFilePath, 0x00, sizeof( aFilePath ) );
	
	/* テーブル設定 */
	psFileList = T_MSG_MSGLOGFILE;
	if( (S_MSG_LOGFILE*)NULL == psFileList )
	{
		goto END;
	}
	
	if( D_FILEIO_MODE_ON != psFileList[iFileKind].iMode )
	{
		goto END;
	}
	
	/* ファイルパス連結 */
	strcat( aFilePath, D_FILEIO_MSGLOG_OUTPUTDIR);
	strcat( aFilePath, psFileList[iFileKind].cFileName);
		
	/* ファイルオープン */
	pFile = fopen( (const char*)(&aFilePath), "a" );
	if( (FILE*)NULL == pFile )
	{
		goto END;
	}
	
	/* ヘッダ出力 */
	if( 0 == iMsgNum[iFileKind])
	{
		fprintf(pFile,"No,Date,Time,QueNum\n");
	}
	
	/*** ログ出力 ***/
	/* No */
	iMsgNum[iFileKind]++;
	fprintf(pFile,"%08d,",iMsgNum[iFileKind]);
	
	/* 時刻 */
	time( &stTime );
	psLocalTime = localtime( &stTime );
	
	fprintf(pFile,"%04d", (psLocalTime->tm_year + 1900) );	/* 年：[1900からの経過年数] */
	fprintf(pFile,"%02d", (psLocalTime->tm_mon + 1) );		/* 月：[0-11] + 1 */
	fprintf(pFile,"%02d,", psLocalTime->tm_mday );			/* 日：[1-31] */
	fprintf(pFile,"%02d", psLocalTime->tm_hour );			/* 時：[0-23] */
	fprintf(pFile,"%02d", psLocalTime->tm_min );			/* 分：[0-59] */
	fprintf(pFile,"%02d,", psLocalTime->tm_sec );			/* 秒：[0-61] 最大2秒までのうるう秒を考慮 */
	
	/* Que数 */
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

