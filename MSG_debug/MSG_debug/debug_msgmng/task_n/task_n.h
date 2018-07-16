#pragma once

#include "../msg_com/msg_com.h"
#include "task_n_recv.h"
#include "task_n_send.h"

/*** _C ***/
#define D_TASK_N_OK		(0)
#define D_TASK_N_NG		(-1)

/*** _E ***/

/*** _S ***/
typedef void( *F_N_RECVFUNCPTR )(S_MSG_DATA* recv);

typedef struct
{
	int iMsgId;
	void* func;
}S_N_RECV_TABLE;

/*** _P ***/
void TASK_N_Thread( void );
void N_recv( S_MSG_DATA* );

/*** _T ***/

