#pragma once

#include "../msg_com/msg_com.h"
#include "task_exm_recv.h"
#include "task_exm_send.h"

/*** _C ***/
#define D_TASK_EXM_OK		(0)
#define D_TASK_EXM_NG		(-1)

/*** _E ***/

/*** _S ***/
typedef void( *F_EXM_RECVFUNCPTR )(S_MSG_DATA* recv);

typedef struct
{
	int iMsgId;
	void* func;
}S_EXM_RECV_TABLE;

/*** _P ***/
void TASK_EXM_Thread( void );
void EXM_recv( S_MSG_DATA* );

/*** _T ***/

