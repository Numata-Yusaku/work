#pragma once

#include "../msg_com/msg_com.h"
#include "task_mnt_recv.h"
#include "task_mnt_send.h"

/*** _C ***/
#define D_TASK_MNT_OK		(0)
#define D_TASK_MNT_NG		(-1)

/*** _E ***/

/*** _S ***/
typedef void( *F_MNT_RECVFUNCPTR )(S_MSG_DATA* recv);

typedef struct
{
	int iMsgId;
	void* func;
}S_MNT_RECV_TABLE;

/*** _P ***/
void TASK_MNT_Thread( void );
void MNT_recv( S_MSG_DATA* );

/*** _T ***/

