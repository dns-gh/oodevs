//*****************************************************************************
//
// $Created : AML 03-04-01 $
//  $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/LMP_App.cpp $
//   $Author: Aml $
//  $Modtime: 18/04/03 10:36 $
// $Revision: 7 $
// $Workfile: LMP_App.cpp $
//
//*****************************************************************************

#include "LMP_pch.h"
#include "LMP_App.h"

#include "moc_LMP_App.cpp"


#ifndef USE_INLINE
#   include "LMP_App.inl"
#endif

#include "LMP_Workspace.h"
#include "LMP_MainWindow.h"

//-----------------------------------------------------------------------------
// Name   : LMP_App::LMP_App
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
LMP_App::LMP_App( int argc, char** argv )
    :   QApplication            ( argc, argv    )
    ,   bRunning_               ( true          )
    ,   bDisplay_               ( false         )    
    ,   pMainWindow_            ( NULL          )
    ,   strLauncherHostname_    ( "localhost"   )

{
    ParseCmdArgs( argc, argv );

    pWorkSpace_ = new LMP_Workspace( *this );
////    pWorkSpace_->Initialize();

    if( bDisplay_ )
    {
        if ( !QGLFormat::hasOpenGL() )
        {
            qWarning( "This system has no OpenGL support. Display disabled." );
            bDisplay_ = false;
        }
        pMainWindow_ = new LMP_MainWindow( *pWorkSpace_ );
    }

    setMainWidget( pMainWindow_ );

    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( SlotQuit() ) );  
}



//-----------------------------------------------------------------------------
// Name   : LMP_App::~LMP_App
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
LMP_App::~LMP_App()
{
    delete pWorkSpace_;
    delete pMainWindow_;
}


//-----------------------------------------------------------------------------
// Name   : LMP_App::Run
// Object : 
// Return : true if the application must be restarted
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
bool32 LMP_App::Run()
{
    const int nSleepTime = 50;

    if( bDisplay_ )
    {
        pMainWindow_->show();
        while( bRunning_ )
        {
            Sleep( nSleepTime );
            processEvents();
            pWorkSpace_->Execute();
        }
        pMainWindow_->hide();
    }
    else
    {
        while( bRunning_ )
        {            
            Sleep( nSleepTime );
            pWorkSpace_->Execute();
        }
    }

    return bRunning_;
}

//=============================================================================
// SLOTS
//==========================================================================<===

//-----------------------------------------------------------------------------
// Name: LMP_App::SlotQuit
// Created:  NLD 2002-01-07 
//-----------------------------------------------------------------------------
void LMP_App::SlotQuit()
{
    bRunning_ = false;
//    quit();
}

//-----------------------------------------------------------------------------
// Name   : LMP_App::ParseCmdArgs
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
bool LMP_App::ParseCmdArgs( int nArgc, char** ppArgv )
{
    if( nArgc < 1 )
    {
        MT_LOG_ERROR_MSG( MT_FormatString( "Usage : %s [-display] [-nodisplay] [-hostname #] ", ppArgv[0] ).c_str() );
        return false;
    }

    // Parse options
    for( int nArgNbr = 1; nArgNbr < nArgc && *ppArgv[nArgNbr] == '-'; ++nArgNbr )
    {
        // Server
        if( stricmp( ppArgv[nArgNbr], "-display" ) == 0 )
		{
            bDisplay_ = true;
		}
        else if( stricmp( ppArgv[nArgNbr], "-nodisplay" ) == 0 )
		{
            bDisplay_ = false;
		}
        else if( stricmp( ppArgv[nArgNbr], "-hostname" ) == 0 )
        {
            ++nArgNbr;

            if( nArgNbr >= nArgc )
            {
                MT_LOG_ERROR_MSG( "Please specify the hostname" );
                return false;
            }
            strLauncherHostname_ = ppArgv[nArgNbr];
        }
    }

    bDisplay_ = true; // on force a toujours true, cette appli ne sert a rien sans ca

    return true;
}

