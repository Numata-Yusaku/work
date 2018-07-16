#pragma once


/*** _C ***/
#define D_FILEIO_OK		(0)
#define D_FILEIO_NG		(-1)

#define D_FILEIO_MSGLOG_FILENAME_MAX		(256)
#define D_FILEIO_MSGLOG_INPUTDIR			"INPUT\\"
#define D_FILEIO_MSGLOG_INPUTDIR_SIZE		sizeof(D_FILEIO_MSGLOG_INPUTDIR)

/*** _E ***/
enum EN_FILEIO_FILEKIND
{
	E_FILEIO_FILEKIND_TEST = 0,

	/* Ç±Ç±ÇÊÇËè„Ç…íËã`Ç∑ÇÈÇ±Ç∆ */
	E_FILEIO_FILEKIND_NUM
};

/*** _S ***/

/*** _P ***/
int FILEIO_open(void);
int FILEIO_read(void);
int FILEIO_close(void);

