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
	
	/*** �̈�m�� ***/
	stFileInfo = (S_FILE_INFO*)malloc( sizeof( S_FILE_INFO ) );
	if ((S_FILE_INFO*)NULL == stFileInfo)
	{
		goto END;
	}
	
	/* ������ */
	memset( stFileInfo, 0x00, sizeof( S_FILE_INFO ) );
	
	/* �t�@�C�����擾 */
	memcpy( &stFileInfo->u1FileName, D_MAIN_FILENAME_ENUMLIST, sizeof( stFileInfo->u1FileName ) );
	get_fileinfo( stFileInfo );
	
	
	/*** �̈�m�� ***/
	stList = (S_ENUM_LIST*)malloc( sizeof(S_ENUM_LIST) * stFileInfo->iLineNum );
	if( (S_ENUM_LIST*)NULL == stList)
	{
		goto END;
	}
	
	/* ������ */
	memset( stList, 0x00, sizeof( S_ENUM_LIST ) * stFileInfo->iLineNum );
	
	get_filedata( stFileInfo, stList );
	
	make_enumlist_header( stFileInfo, stList, D_MAIN_HEADER_MODE_MAKE );

END:
	
	/*** �̈�J�� ***/
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
	int		iLineNum = 0;							/* �s�� */
	int		iCharNum = 0;							/* �������� */
	int		iCharCnt = 0;							/* �����J�E���^ */
	int		iLineCharMax = 0;						/* 1�s�����ő� */

	FILE* pInEnumList = (FILE*)NULL;
	char cGetChar = ' ';

	/*** �t�@�C���I�[�v�� ***/
	pInEnumList = fopen( stFileInfo->u1FileName, "r" );
	if ((FILE*)NULL == pInEnumList)
	{
		goto END;
	}

	/* �s���E�������擾 */
	while (D_MAIN_CHAR_EOF != cGetChar)
	{
		/* �����擾 */
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

			/* �s���X�V */
			iLineNum++;
			iCharCnt = 0;
		}
		/* �������X�V */
		iCharNum++;
	}

	if ((0 >= iLineNum) ||
		(0 >= iLineCharMax))
	{
		goto END;
	}
	
	/* �f�[�^�ݒ� */
	stFileInfo->iLineNum = iLineNum;
	stFileInfo->iLineCharMax = iLineCharMax;

END:
	
	/*** �t�@�C���N���[�Y ***/
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
	int		iLineNum = 0;							/* �s�� */
	int		iCharNum = 0;							/* �������� */
	int		iCharCnt = 0;							/* �����J�E���^ */
	int		iLineCharMax = 0;						/* 1�s�����ő� */
	
	char*	cLineBuf = (char*)NULL;					/* �����o�b�t�@ */
	FILE* pInEnumList = (FILE*)NULL;
	
	char cGetChar = ' ';
	
	cLineBuf = (char*)malloc( stFileInfo->iLineCharMax );
	if ((char*)NULL == cLineBuf)
	{
		goto END;
	}
	
	/* ������ */
	memset( cLineBuf, 0x00, stFileInfo->iLineCharMax );
	
	/*** �t�@�C���I�[�v�� ***/
	pInEnumList = fopen( stFileInfo->u1FileName, "r" );
	if ((FILE*)NULL == pInEnumList)
	{
		goto END;
	}

	/* �s���E�������擾 */
	while ( D_MAIN_CHAR_EOF != cGetChar )
	{
		/* �����擾 */
		cGetChar = getc( pInEnumList );
		
		if ( D_MAIN_CHAR_EOF == cGetChar )
		{
			/* �t�@�C���I�[ */
			break;
		}
		else if(D_MAIN_CHAR_END == cGetChar)
		{
			/* ������I�[ */
			
			/* �������N���A */
			iCharCnt = 0;
		}
		else if( D_MAIN_CHAR_ENTER == cGetChar )
		{
			/* ���s */
			
			if( NULL != cLineBuf[0])
			{
				memcpy( stList[iLineNum].u1Name, cLineBuf, stFileInfo->iLineCharMax );
				/* �s���X�V */
				iLineNum++;
			}
			
			/* ������N���A */
			memset( cLineBuf, 0x00, stFileInfo->iLineCharMax );
			iCharCnt = 0;
		}
		else
		{
			/* ������i�[ */
			cLineBuf[iCharCnt] = cGetChar;
			iCharCnt++;
		}
	}

	/* �����s�폜 */
	stFileInfo->iLineNum = iLineNum;

END:
	/*** �̈�J�� ***/
	if ((char*)NULL != cLineBuf)
	{
		free( cLineBuf );
		cLineBuf = (char*)NULL;
	}

	/*** �t�@�C���N���[�Y ***/
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
	int		iLineCnt = 0;							/* �s���J�E���^ */
	
	FILE* pOutEnumListTable = (FILE*)NULL;
	char	cLineBuf[D_MAIN_ENUMNAME];					/* �����o�b�t�@ */

	memset( cLineBuf, 0x00, D_MAIN_ENUMNAME );

	/*** �t�@�C���I�[�v�� ***/
	pOutEnumListTable = fopen( D_MAIN_FILENAME_ENUMLIST_TABLE, "w" );
	if ((FILE*)NULL == pOutEnumListTable)
	{
		goto END;
	}
	
	fprintf( pOutEnumListTable, "#ifndef __ENUMLIST_TABLE_H__\n" );
	fprintf( pOutEnumListTable, "#define __ENUMLIST_TABLE_H__\n\n\n" );

	/* �e�[�u���� */
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
	/*** �t�@�C���N���[�Y ***/
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

	/*** �t�@�C���I�[�v�� ***/
	pOutEnumListCsv = fopen( D_MAIN_FILENAME_ENUMLIST_CSV, "w" );
	if ((FILE*)NULL == pOutEnumListCsv)
	{
		goto END;
	}

	fprintf( pOutEnumListCsv, "�l,����\n" );
	for(iTableCnt = 0;iTableCnt < iTableNum; iTableCnt++)
	{
		fprintf( pOutEnumListCsv, "0x%08x,%s\n", stList[iTableCnt].i4Data, stList[iTableCnt].u1Name );
	}

END:
	/*** �t�@�C���N���[�Y ***/
	if ((FILE*)NULL != pOutEnumListCsv)
	{
		fclose( pOutEnumListCsv );
		pOutEnumListCsv = (FILE*)NULL;
	}

#endif	/* D_MAIN_HEADER_EXISTSTS */
	return;
}