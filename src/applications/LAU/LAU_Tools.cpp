//*****************************************************************************
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 16 $
// $Workfile: LAU_SIM.cpp $
//
//*****************************************************************************

#include "LAU_pch.h"
#include "LAU_Tools.h"

const char* MT_GetApplicationPath( const char* pFilenameToConcatene )
{
	static char szExePath[MAX_PATH];

	GetModuleFileName( NULL, szExePath, MAX_PATH );

	int i = strlen( szExePath );
	while( szExePath[i] != '\\' && szExePath[i] != '/' && i > 0 )
	{
		--i;
	}

#ifdef _DEBUG
	if( !strnicmp( &szExePath[i-5], "debug", 5 ) )
	{
		i-=6;
	}
	if( !strnicmp( &szExePath[i-3], "out", 3 ) )
	{
		i-=4;
	}
#endif // _DEBUG

	if( !strnicmp( &szExePath[i-7], "release", 7 ) )
	{
		i-=8;
	}

	szExePath[i+1] = '\0';

	if( pFilenameToConcatene )
	{
		strcat( szExePath, pFilenameToConcatene );
	}

	return szExePath;
}


//#define LAU_WANT_TO_DEBUG_START_OF_THE_APP
void OutputLogDebug( const char* szMsg, const char* szSourceFilename, const int nNumLine )
{
#ifdef LAU_WANT_TO_DEBUG_START_OF_THE_APP

////#ifdef _DEBUG

    char buffer[256];
    time_t nTime = time( NULL );
    strftime( buffer, 256, "%a, %d %b %Y %H:%M:%S", localtime( &nTime ) );

    FILE* pf = fopen( MT_GetApplicationPath( "ScipioLauncherInternal.log" ), "at" );

    char bufferLine[128];
    
    if( nNumLine != -1 )
    {
        sprintf( bufferLine, " Line: %d", nNumLine );
    }
    else
    {
        bufferLine[0] = '\0';
    }
    
    fprintf( pf, "[%s] %s%s: %s\n", buffer, szSourceFilename!=NULL?szSourceFilename:"", bufferLine, szMsg );

    fclose( pf );

////#endif // _DEBUG

#else // LAU_WANT_ALL_LOG_IN_THE_SAME_FILE

    szSourceFilename;
    nNumLine;
    MT_LOG_INFO_MSG( szMsg  );

#endif // LAU_WANT_xxx
}

const char* SystemGetErrorMessage( const int nNumSystemError )
{
    static char buffer[512];
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, nNumSystemError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, 512, NULL );
    return buffer;
}