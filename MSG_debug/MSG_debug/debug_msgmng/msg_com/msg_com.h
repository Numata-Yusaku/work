#pragma once

#include "msg_id.h"

/*** _C ***/
#define D_FILEIO_OK		(0)
#define D_FILEIO_NG		(-1)

#define D_FILEIO_MODE_ON	(1)
#define D_FILEIO_MODE_OFF	(0)

#define D_FILEIO_QUEMAX	(256)			/* キューの最大数 */

#define D_FILEIO_MSGID_INVALID (0x00000000)

#define D_FILEIO_TASKCICLE_100	(100)

#define D_FILEIO_MSGLOG_FILENAME_MAX		(256)
#define D_FILEIO_MSGLOG_OUTPUTDIR			"OUTPUT\\"
#define D_FILEIO_MSGLOG_OUTPUTDIR_SIZE		sizeof(D_FILEIO_MSGLOG_OUTPUTDIR)

/*** _E ***/
enum EN_TASKID
{
	E_TASKID_MAIN = 0x00000000,
	E_TASKID_N,
	E_TASKID_NM,
	E_TASKID_EXM,
	E_TASKID_MNT,

	/* ここより上に定義すること */
	E_TASKID_NUM,
	E_TASKID_INVALID = 0xFFFFFFFF,
};

enum EN_MSGLOG_FILEKIND
{
	E_MSGLOG_FILEKIND_FILEIOCOM = 0,
	E_MSGLOG_FILEKIND_QUE,

	/* ここより上に定義すること */
	E_MSGLOG_FILEKIND_NUM
};

/*** _S ***/
typedef struct
{
	int		iMsgid;
	int		iSize;
	void*	vpPara;
}S_MSG_DATA;

typedef struct
{
	int		iFlag;
	int		iHead;
	int		iTail;
	int		iNum;
}S_MSG_STATUS;

typedef struct
{
	S_MSG_STATUS stStatus;
	S_MSG_DATA stData[D_FILEIO_QUEMAX];
}S_MSG_QUE;

typedef void( *F_MSG_MSGLOGPTR )(int iTask, S_MSG_QUE* psQue );

typedef struct
{
	int iKind;
	int iMode;
	void* pvFunc;
	char cFileName[D_FILEIO_MSGLOG_FILENAME_MAX];
}S_MSG_LOGFILE;

/*** _P ***/
void MSGCOM_init( void );
int MSGCOM_send( int iTask, S_MSG_DATA* psPara );
int MSGCOM_recv( int iTask, S_MSG_DATA* psPara );
