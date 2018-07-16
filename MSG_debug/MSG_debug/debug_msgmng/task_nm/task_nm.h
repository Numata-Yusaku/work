#pragma once

#include "../msg_com/msg_com.h"
#include "task_nm_recv.h"
#include "task_nm_send.h"

/*** _C ***/
#define D_TASK_NM_OK		(0)
#define D_TASK_NM_NG		(-1)

/*** _E ***/

/*** _S ***/
typedef void( *F_NM_RECVFUNCPTR )(S_MSG_DATA* recv);

typedef struct
{
	int iMsgId;
	void* func;
}S_NM_RECV_TABLE;

/*** _P ***/
void TASK_NM_Thread( void );
void NM_recv( S_MSG_DATA* );

/*** _T ***/

