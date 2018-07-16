#pragma once

#include "../msg_com/msg_com.h"
#include "task_main_recv.h"
#include "task_main_send.h"

/*** _C ***/
#define D_TASK_MAIN_OK		(0)
#define D_TASK_MAIN_NG		(-1)

/*** _E ***/

/*** _S ***/
typedef void( *F_MAIN_RECVFUNCPTR )(S_MSG_DATA* recv);
typedef void( *F_MAIN_CMDRECVFUNCPTR )(void);

typedef struct
{
	int iMsgId;
	void* func;
}S_MAIN_RECV_TABLE;

typedef struct
{
	int iKeyId;
	void* func;
}S_MAIN_CMDRECV_TABLE;

/*** _P ***/
void MAIN_recv( S_MSG_DATA* );
void MAIN_recv_cmd( void );


/*** _T ***/

