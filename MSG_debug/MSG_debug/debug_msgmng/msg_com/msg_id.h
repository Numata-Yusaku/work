#pragma once

/*** _C ***/

/*** _E ***/
enum EN_MSGTASKID
{
	E_MSGTASKID_MAIN = 0x0000,
	E_MSGTASKID_N,
	E_MSGTASKID_NM,
	E_MSGTASKID_EXM,
	E_MSGTASKID_MNT,
};

enum EN_MSGID_MAIN
{
	E_MSGID_MAIN_REQ_TEST =			( 0x00000001 | ( E_MSGTASKID_MAIN << 16) ),
	E_MSGID_MAIN_RES_TEST,
	
	/* ��������ɒ�`���邱�� */
	E_MSGID_MAIN_INVALID = 0x00000000,
};

enum EN_MSGID_TASK_N
{
	E_MSGID_TASK_N_REQ_TEST =		( 0x00000001 | ( E_MSGTASKID_N << 16) ),
	E_MSGID_TASK_N_RES_TEST,
	
	/* ��������ɒ�`���邱�� */
	E_MSGID_TASK_N_INVALID = 0x00000000,
};

enum EN_MSGID_TASK_NM
{
	E_MSGID_TASK_NM_REQ_TEST =		( 0x00000001 | ( E_MSGTASKID_NM << 16) ),
	E_MSGID_TASK_NM_RES_TEST,
	
	/* ��������ɒ�`���邱�� */
	E_MSGID_TASK_NM_INVALID = 0x00000000,
};

enum EN_MSGID_TASK_EXM
{
	E_MSGID_TASK_EXM_REQ_TEST =		( 0x00000001 | ( E_MSGTASKID_EXM << 16) ),
	E_MSGID_TASK_EXM_RES_TEST,
	
	/* ��������ɒ�`���邱�� */
	E_MSGID_TASK_EXM_INVALID = 0x00000000,
};

enum EN_MSGID_TASK_MNT
{
	E_MSGID_TASK_MNT_REQ_TEST =		( 0x00000001 | ( E_MSGTASKID_MNT << 16) ),
	E_MSGID_TASK_MNT_RES_TEST,
	
	/* ��������ɒ�`���邱�� */
	E_MSGID_TASK_MNT_INVALID = 0x00000000,
};


/*** _S ***/

/*** _P ***/

/*** _T ***/

