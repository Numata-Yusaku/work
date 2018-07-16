#pragma once

#ifndef __MAIN_H__
#define __MAIN_H__

/************************************************************************/
/*	�y�����z															*/
/*	1.	enum.h�Ƀ`�F�b�N�Ώۂ��`����B								*/
/*	2.	[enumlist.txt]�Ƀ`�F�b�N�Ώۂ̒�`�����L�ڂ���					*/
/*	�y�g�p���@�z														*/
/*	3.	[D_MAIN_DEBUGSWICH]���u0�v�ɂ��ăf�o�b�O���s					*/
/*	4.	[D_MAIN_DEBUGSWICH]���u1�v�ɂ��ăf�o�b�O���s					*/
/*																		*/
/*	�y�m�F���@�z														*/
/*	5.	[enumlist.csv]�̏o�͂���`�ƈ�v���Ă��邩�m�F					*/
/*																		*/
#define D_MAIN_DEBUGSWICH	(1)	/*	���������u0�vor�u1�v�؂�ւ���		*/
/************************************************************************/
/* _c */
#define	D_MAIN_HEADER_MODE_DUMMY	(0)
#define	D_MAIN_HEADER_MODE_MAKE		(1)

#define	D_MAIN_ENUMNAME				(256)

#if D_MAIN_DEBUGSWICH	/* HEADER_EXIST */
#define D_MAIN_HEADER_EXISTSTS			(1)
#else	/* HEADER_EXIST */
#define D_MAIN_HEADER_EXISTSTS			(0)
#endif	/* HEADER_EXIST */

#define	D_MAIN_FILENAME_ENUMLIST		"InputData\\enumlist.txt"
#define	D_MAIN_FILENAME_ENUMLIST_TABLE	"enumlist_table.h"
#define	D_MAIN_FILENAME_ENUMLIST_CSV	"OutputData\\enumlist.csv"

#define	D_MAIN_CHAR_EOF				(EOF)
#define	D_MAIN_CHAR_ENTER			('\n')
#define	D_MAIN_CHAR_END			('\0')


enum EN_DEBUG_MODE
{
	E_DEBUG_MODE_MAKE_HEADER = 0,
	E_DEBUG_MODE_CHK_ENUM,
};

/* _s */
typedef struct
{
	int i4Data;
	char u1Name[D_MAIN_ENUMNAME];
}S_ENUM_LIST;

typedef struct
{
	char u1FileName[D_MAIN_ENUMNAME];
	int iLineNum;
	int iLineCharMax;
}S_FILE_INFO;

/* _p */
/* E_DEBUG_MODE_MAKE_HEADER */
void MakeHeader(void);
void get_fileinfo(
	S_FILE_INFO* stFileInfo
);
void get_filedata(
	S_FILE_INFO* stFileInfo,
	S_ENUM_LIST* stListExmng
);
void make_enumlist_header(
	S_FILE_INFO* stFileInfo,
	S_ENUM_LIST* stListExmng,
	int mode
);
/* E_DEBUG_MODE_CHK_ENUM */
void MakeEnumList( void );

/* _t */
//S_ENUM_LIST T_ENUMLIST_EXMNG[2] =
//{
//	{ 1,"AAA" },
//	{ E_CHAR_KIND_NUMBER,"E_CHAR_KIND_NUMBER" },
//};


#endif	/* __MAIN_H__ */