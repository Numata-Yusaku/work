#pragma once

#ifndef __MAIN_H__
#define __MAIN_H__

/************************************************************************/
/*	【準備】															*/
/*	1.	enum.hにチェック対象を定義する。								*/
/*	2.	[enumlist.txt]にチェック対象の定義名を記載する					*/
/*	【使用方法】														*/
/*	3.	[D_MAIN_DEBUGSWICH]を「0」にしてデバッグ実行					*/
/*	4.	[D_MAIN_DEBUGSWICH]を「1」にしてデバッグ実行					*/
/*																		*/
/*	【確認方法】														*/
/*	5.	[enumlist.csv]の出力が定義と一致しているか確認					*/
/*																		*/
#define D_MAIN_DEBUGSWICH	(1)	/*	←ここを「0」or「1」切り替える		*/
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