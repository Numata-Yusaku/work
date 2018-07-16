#include <stdio.h>

#include "task_main.h"

static F_MAIN_RECVFUNCPTR main_get_recvfunc( int iMsgId );
static F_MAIN_CMDRECVFUNCPTR main_get_cmdrecvfunc( int iKeyId );


S_MAIN_RECV_TABLE T_MAIN_RECV_TABLE[] =
{
	/* MSGID						,	RecvFunc			*/
	{ E_MSGID_MAIN_REQ_TEST			,	main_rcv_test_req		},
	{ E_MSGID_TASK_N_RES_TEST		,	main_rcv_test_res		},
};

S_MAIN_CMDRECV_TABLE T_MAIN_CMDRECV_TABLE[] =
{
	/* KEYID	,	RecvFunc			*/
	{ 'a'		,	main_rcv_cmd_a		},
	//{ 'b'		,	NULL				},
	//{ 'c'		,	NULL				},
	//{ 'd'		,	NULL				},
	{ 'e'		,	main_rcv_cmd_e		},
	//{ 'f'		,	NULL				},
	//{ 'g'		,	NULL				},
	//{ 'h'		,	NULL				},
	//{ 'i'		,	NULL				},
	//{ 'j'		,	NULL				},
	//{ 'k'		,	NULL				},
	//{ 'l'		,	NULL				},
	//{ 'm'		,	NULL				},
	//{ 'n'		,	NULL				},
	//{ 'o'		,	NULL				},
	//{ 'p'		,	NULL				},
	//{ 'q'		,	NULL				},
	//{ 'r'		,	NULL				},
	{ 's'		,	main_rcv_cmd_s		},
	//{ 't'		,	NULL				},
	//{ 'u'		,	NULL				},
	//{ 'v'		,	NULL				},
	//{ 'w'		,	NULL				},
	//{ 'x'		,	NULL				},
	//{ 'y'		,	NULL				},
	//{ 'z'		,	NULL				},
};

/****************************************************************************/
void MAIN_recv( S_MSG_DATA* recv)
{
	int iMsgId = recv->iMsgid;
	F_MAIN_RECVFUNCPTR pvRecvFunc = (F_MAIN_RECVFUNCPTR)NULL;
	
	/* ���s�֐��擾 */
	pvRecvFunc = main_get_recvfunc( iMsgId );
	if( (void*)NULL != pvRecvFunc)
	{
		/* ��M�֐����s */
		pvRecvFunc( recv );
	}
	
	return;
}

void MAIN_recv_cmd( void )
{
	int key = 0;
	F_MAIN_CMDRECVFUNCPTR pvRecvFunc = (F_MAIN_CMDRECVFUNCPTR)NULL;
	

	/* �L�[���͂�����ꍇ */
	if ( kbhit() )
	{
		/* �L�[���擾 */
		key = getch();
		
		/* ���s�֐��擾 */
		pvRecvFunc = main_get_cmdrecvfunc( key );
		if( (void*)NULL != pvRecvFunc)
		{
			/* ��M�֐����s */
			pvRecvFunc();
		}
	}	
	return;
}


/****************************************************************************/
F_MAIN_RECVFUNCPTR main_get_recvfunc( int iMsgId )
{
	F_MAIN_RECVFUNCPTR func = (F_MAIN_RECVFUNCPTR)NULL;
	int iNum = 0;
	int iCount = 0;
	S_MAIN_RECV_TABLE* psRecvTable = (S_MAIN_RECV_TABLE*)NULL;
	
	/* ��M�e�[�u���ݒ� */
	psRecvTable =T_MAIN_RECV_TABLE;
	if((S_MAIN_RECV_TABLE*)NULL == psRecvTable )
	{
		return (F_MAIN_RECVFUNCPTR)NULL;
	}
	
	/* ��M�e�[�u���v�f���擾 */
	iNum = sizeof( T_MAIN_RECV_TABLE ) / sizeof( S_MAIN_RECV_TABLE );
	if( 0 >= iNum )
	{
		return (F_MAIN_RECVFUNCPTR)NULL;
	}
	
	/* ��M�֐��T�� */
	for(iCount = 0; iCount < iNum; iCount++)
	{
		if( iMsgId == psRecvTable[iCount].iMsgId )
		{
			func = psRecvTable[iCount].func;
			break;
		}
	}
	
	return func;
}

F_MAIN_CMDRECVFUNCPTR main_get_cmdrecvfunc( int iKeyId )
{
	F_MAIN_CMDRECVFUNCPTR func = (F_MAIN_CMDRECVFUNCPTR)NULL;
	int iNum = 0;
	int iCount = 0;
	S_MAIN_CMDRECV_TABLE* psRecvTable = (S_MAIN_CMDRECV_TABLE*)NULL;
	
	/* ��M�e�[�u���ݒ� */
	psRecvTable =T_MAIN_CMDRECV_TABLE;
	if((S_MAIN_CMDRECV_TABLE*)NULL == psRecvTable )
	{
		return (F_MAIN_CMDRECVFUNCPTR)NULL;
	}
	
	/* ��M�e�[�u���v�f���擾 */
	iNum = sizeof( T_MAIN_CMDRECV_TABLE ) / sizeof( S_MAIN_CMDRECV_TABLE );
	if( 0 >= iNum )
	{
		return (F_MAIN_CMDRECVFUNCPTR)NULL;
	}
	
	/* ��M�֐��T�� */
	for(iCount = 0; iCount < iNum; iCount++)
	{
		if( iKeyId == psRecvTable[iCount].iKeyId )
		{
			func = psRecvTable[iCount].func;
			break;
		}
	}
	
	return func;
}

void main_rcv_test_req( S_MSG_DATA* recv )
{
	main_send_test_req( recv );
	
	return;
}

void main_rcv_test_res( S_MSG_DATA* recv )
{
	main_send_test_res( recv );
	
	return;
}

void main_rcv_cmd_a( void )
{
	
	return;
}
void main_rcv_cmd_e( void )
{
	/* �v���O�����I�� */
	exit();
	return;
}

void main_rcv_cmd_s( void )
{
	/* �e�X�g���M */
	main_send_test_req( NULL );
	return;
}
