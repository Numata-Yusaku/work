#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "enum.h"

#if D_MAIN_HEADER_EXISTSTS	/* D_MAIN_HEADER_EXISTSTS */
#include "enumlist_table.h"
#endif	/* D_MAIN_HEADER_EXISTSTS */

int main(void)
{
	
	switch(D_MAIN_DEBUGSWICH)
	{
		case E_DEBUG_MODE_MAKE_HEADER:
				MakeHeader();
			break;

#if D_MAIN_HEADER_EXISTSTS	/* D_MAIN_HEADER_EXISTSTS */
		case E_DEBUG_MODE_CHK_ENUM:
				MakeEnumList();
			break;
#endif	/* D_MAIN_HEADER_EXISTSTS */

		default:
			break;
	}
	

	return 0;
}
void MakeHeader(void)
{
	S_ENUM_LIST*	stList = (S_ENUM_LIST*)NULL;
	S_FILE_INFO*	stFileInfo = (S_FILE_INFO*)NULL;
	
	/*** 領域確保 ***/
	stFileInfo = (S_FILE_INFO*)malloc( sizeof( S_FILE_INFO ) );
	if ((S_FILE_INFO*)NULL == stFileInfo)
	{
		goto END;
	}
	
	/* 初期化 */
	memset( stFileInfo, 0x00, sizeof( S_FILE_INFO ) );
	
	/* ファイル情報取得 */
	memcpy( &stFileInfo->u1FileName, D_MAIN_FILENAME_ENUMLIST, sizeof( stFileInfo->u1FileName ) );
	get_fileinfo( stFileInfo );
	
	
	/*** 領域確保 ***/
	stList = (S_ENUM_LIST*)malloc( sizeof(S_ENUM_LIST) * stFileInfo->iLineNum );
	if( (S_ENUM_LIST*)NULL == stList)
	{
		goto END;
	}
	
	/* 初期化 */
	memset( stList, 0x00, sizeof( S_ENUM_LIST ) * stFileInfo->iLineNum );
	
	get_filedata( stFileInfo, stList );
	
	make_enumlist_header( stFileInfo, stList, D_MAIN_HEADER_MODE_MAKE );

END:
	
	/*** 領域開放 ***/
	if ((S_FILE_INFO*)NULL == stFileInfo)
	{
		free( stFileInfo );
		stFileInfo = (S_FILE_INFO*)NULL;
	}
	
	if( (S_ENUM_LIST*)NULL != stList )
	{
		free( stList );
		stList = (S_ENUM_LIST*)NULL;
	}
	
	return;
}

void get_fileinfo(
	S_FILE_INFO* stFileInfo
)
{
	int		iLineNum = 0;							/* 行数 */
	int		iCharNum = 0;							/* 文字総数 */
	int		iCharCnt = 0;							/* 文字カウンタ */
	int		iLineCharMax = 0;						/* 1行文字最大 */

	FILE* pInEnumList = (FILE*)NULL;
	char cGetChar = ' ';

	/*** ファイルオープン ***/
	pInEnumList = fopen( stFileInfo->u1FileName, "r" );
	if ((FILE*)NULL == pInEnumList)
	{
		goto END;
	}

	/* 行数・文字数取得 */
	while (D_MAIN_CHAR_EOF != cGetChar)
	{
		/* 文字取得 */
		cGetChar = getc( pInEnumList );
		iCharCnt++;

		if (D_MAIN_CHAR_EOF == cGetChar)
		{
			break;
		}
		else if (D_MAIN_CHAR_ENTER == cGetChar)
		{

			if (iCharCnt > iLineCharMax)
			{
				iLineCharMax = iCharCnt;
			}

			/* 行数更新 */
			iLineNum++;
			iCharCnt = 0;
		}
		/* 文字数更新 */
		iCharNum++;
	}

	if ((0 >= iLineNum) ||
		(0 >= iLineCharMax))
	{
		goto END;
	}
	
	/* データ設定 */
	stFileInfo->iLineNum = iLineNum;
	stFileInfo->iLineCharMax = iLineCharMax;

END:
	
	/*** ファイルクローズ ***/
	if ((FILE*)NULL != pInEnumList)
	{
		fclose( pInEnumList );
		pInEnumList = (FILE*)NULL;
	}

	return;
}

void get_filedata(
	S_FILE_INFO* stFileInfo,
	S_ENUM_LIST* stList
)
{
	int		iLineNum = 0;							/* 行数 */
	int		iCharNum = 0;							/* 文字総数 */
	int		iCharCnt = 0;							/* 文字カウンタ */
	int		iLineCharMax = 0;						/* 1行文字最大 */
	
	char*	cLineBuf = (char*)NULL;					/* 文字バッファ */
	FILE* pInEnumList = (FILE*)NULL;
	
	char cGetChar = ' ';
	
	cLineBuf = (char*)malloc( stFileInfo->iLineCharMax );
	if ((char*)NULL == cLineBuf)
	{
		goto END;
	}
	
	/* 初期化 */
	memset( cLineBuf, 0x00, stFileInfo->iLineCharMax );
	
	/*** ファイルオープン ***/
	pInEnumList = fopen( stFileInfo->u1FileName, "r" );
	if ((FILE*)NULL == pInEnumList)
	{
		goto END;
	}

	/* 行数・文字数取得 */
	while ( D_MAIN_CHAR_EOF != cGetChar )
	{
		/* 文字取得 */
		cGetChar = getc( pInEnumList );
		
		if ( D_MAIN_CHAR_EOF == cGetChar )
		{
			/* ファイル終端 */
			break;
		}
		else if(D_MAIN_CHAR_END == cGetChar)
		{
			/* 文字列終端 */
			
			/* 文字数クリア */
			iCharCnt = 0;
		}
		else if( D_MAIN_CHAR_ENTER == cGetChar )
		{
			/* 改行 */
			
			if( NULL != cLineBuf[0])
			{
				memcpy( stList[iLineNum].u1Name, cLineBuf, stFileInfo->iLineCharMax );
				/* 行数更新 */
				iLineNum++;
			}
			
			/* 文字列クリア */
			memset( cLineBuf, 0x00, stFileInfo->iLineCharMax );
			iCharCnt = 0;
		}
		else
		{
			/* 文字列格納 */
			cLineBuf[iCharCnt] = cGetChar;
			iCharCnt++;
		}
	}

	/* 無効行削除 */
	stFileInfo->iLineNum = iLineNum;

END:
	/*** 領域開放 ***/
	if ((char*)NULL != cLineBuf)
	{
		free( cLineBuf );
		cLineBuf = (char*)NULL;
	}

	/*** ファイルクローズ ***/
	if ((FILE*)NULL != pInEnumList)
	{
		fclose( pInEnumList );
		pInEnumList = (FILE*)NULL;
	}

	return;
}

void make_enumlist_header(
	S_FILE_INFO* stFileInfo,
	S_ENUM_LIST* stList,
	int mode
)
{
	int		iLineCnt = 0;							/* 行数カウンタ */
	
	FILE* pOutEnumListTable = (FILE*)NULL;
	char	cLineBuf[D_MAIN_ENUMNAME];					/* 文字バッファ */

	memset( cLineBuf, 0x00, D_MAIN_ENUMNAME );

	/*** ファイルオープン ***/
	pOutEnumListTable = fopen( D_MAIN_FILENAME_ENUMLIST_TABLE, "w" );
	if ((FILE*)NULL == pOutEnumListTable)
	{
		goto END;
	}
	
	fprintf( pOutEnumListTable, "#ifndef __ENUMLIST_TABLE_H__\n" );
	fprintf( pOutEnumListTable, "#define __ENUMLIST_TABLE_H__\n\n\n" );

	/* テーブル部 */
	fprintf( pOutEnumListTable, "S_ENUM_LIST T_ENUMLIST[%d] = \n", stFileInfo->iLineNum );
	fprintf( pOutEnumListTable, "{\n" );

	for( iLineCnt = 0; iLineCnt < stFileInfo->iLineNum; iLineCnt++ )
	{
		memset( cLineBuf, 0x00, D_MAIN_ENUMNAME );
		memcpy( &cLineBuf,&(stList[iLineCnt].u1Name), stFileInfo->iLineCharMax );
		
		fprintf( pOutEnumListTable, "\t{ %s,\"%s\"},\n", cLineBuf, cLineBuf );
		
	}

	fprintf( pOutEnumListTable, "};\n" );
	fprintf( pOutEnumListTable, "#endif \/* __ENUMLIST_TABLE_H__ *\/\n" );
	

END:
	/*** ファイルクローズ ***/
	if ((FILE*)NULL != pOutEnumListTable)
	{
		fclose( pOutEnumListTable );
		pOutEnumListTable = (FILE*)NULL;
	}

	return;
}

void MakeEnumList( void )
{
#if D_MAIN_HEADER_EXISTSTS	/* D_MAIN_HEADER_EXISTSTS */
	int iTableNum = 0;
	int iTableCnt = 0;
	FILE* pOutEnumListCsv = (FILE*)NULL;
	S_ENUM_LIST* stList = (S_ENUM_LIST*)NULL;
	
	stList = T_ENUMLIST;
	iTableNum = sizeof( T_ENUMLIST ) / sizeof( S_ENUM_LIST );

	/*** ファイルオープン ***/
	pOutEnumListCsv = fopen( D_MAIN_FILENAME_ENUMLIST_CSV, "w" );
	if ((FILE*)NULL == pOutEnumListCsv)
	{
		goto END;
	}

	fprintf( pOutEnumListCsv, "値,名称\n" );
	for(iTableCnt = 0;iTableCnt < iTableNum; iTableCnt++)
	{
		fprintf( pOutEnumListCsv, "0x%08x,%s\n", stList[iTableCnt].i4Data, stList[iTableCnt].u1Name );
	}

END:
	/*** ファイルクローズ ***/
	if ((FILE*)NULL != pOutEnumListCsv)
	{
		fclose( pOutEnumListCsv );
		pOutEnumListCsv = (FILE*)NULL;
	}

#endif	/* D_MAIN_HEADER_EXISTSTS */
	return;
}