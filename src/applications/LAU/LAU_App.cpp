//*****************************************************************************
//
// $Created : AML 03-03-27 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_App.cpp $
// $Author: Nld $
// $Modtime: 23/07/04 17:42 $
// $Revision: 17 $
// $Workfile: LAU_App.cpp $
//
//*****************************************************************************

#include "LAU_pch.h"
#include "LAU_App.h"

#ifndef USE_INLINE
#   include "LAU_App.inl"
#endif

#include "LAU_Workspace.h"

LAU_App* LAU_App::static_pInstance = 0;

//-----------------------------------------------------------------------------
// Name: LAU_App constructor
// Created: AML 03-03-27 
//-----------------------------------------------------------------------------
LAU_App::LAU_App( int /*argc*/, char** /*argv*/ )
    : bRunning_( false )
{
#ifdef _DEBUG
    const int nDefaultSleepTime = 400;
#else
    const int nDefaultSleepTime = 2000;
#endif

    nSleepTime_ = nDefaultSleepTime;

    bKillStuckedSim_ = false;           // by default don't kill

    const char szArchiveName[] = "ScipioLauncher.xml";

    MT_XXmlInputArchive archive;
    if( !archive.Open( szArchiveName ) )
        throw MT_ScipioException( "LAU_App::Constructor", __FILE__, __LINE__, MT_FormatString( "Cannot open file '%s'", szArchiveName ) );

    if( !archive.Section( "ScipioLauncher" ) )
        throw MT_ScipioException( "LAU_Workspace::Constructor", __FILE__, __LINE__, "Section 'ScipioLauncher' doesn't exist", archive.RetrieveLastError()->GetInfo() ); 

    if( !archive.Section( "App" ) )
        throw MT_ScipioException( "LAU_App::Constructor", __FILE__, __LINE__, "Section 'App' doesn't exist", archive.RetrieveLastError()->GetInfo() ); 

    if ( !archive.ReadField( "SleepTime", nSleepTime_ ) )
        throw MT_ScipioException( "LAU_App::Constructor", __FILE__, __LINE__, "Field 'SleepTime' doesn't exist", archive.RetrieveLastError()->GetInfo() );

//    if ( !archive.ReadField( "KillStuckedSim", bKillStuckedSim_ ) )
//        ;
        // $$$$ Alma 2004-01-30: pour l'instant on throw rien, par defaut on kill plus, 
//        throw MT_ScipioException( "LAU_App::Constructor", __FILE__, __LINE__, "Field 'KillStuckedSim' doesn't exist", archive.RetrieveLastError()->GetInfo() );

    archive.EndSection(); // App


    MT_LOG_INFO_MSG( MT_FormatString( "Settings: LAU_App.SleepTime: %d", nSleepTime_ ).c_str() );


    pWorkSpace_ = new LAU_Workspace( *this, archive );
    pWorkSpace_->Initialize();


    archive.EndSection(); // ScipioLauncher

    // gestion du singleton
    if( static_pInstance == 0 )
        static_pInstance = this;
}


//-----------------------------------------------------------------------------
// Name: LAU_App destructor
// Created: AML 03-03-27
//-----------------------------------------------------------------------------
LAU_App::~LAU_App()
{
    if( pWorkSpace_ != 0 )
    {
        pWorkSpace_->Stop();
        pWorkSpace_->Terminate();
        delete pWorkSpace_;
    }

    // gestion du singleton
    if( static_pInstance == this )
        static_pInstance = 0;
}


//-----------------------------------------------------------------------------
// Name: LAU_App::Initialize
// Created: AML 03-04-24
//-----------------------------------------------------------------------------
bool32 LAU_App::Initialize()
{
    pWorkSpace_->Start();
    bRunning_ = true;

    return true;
}



//-----------------------------------------------------------------------------
// Name: LAU_App::Run
// Return false if the application must be closed
// Created: AML 03-03-27
//-----------------------------------------------------------------------------
bool32 LAU_App::Run()
{
    while( bRunning_ )
        RunOneFrame();
    return bRunning_;
}


//-----------------------------------------------------------------------------
// Name: LAU_App::RunOneFrame
// Return false if the application must be closed
// Created: AML 03-04-24
//-----------------------------------------------------------------------------
bool32 LAU_App::RunOneFrame()
{
    Sleep( nSleepTime_ );
    pWorkSpace_->Execute();

    return bRunning_;
}


//-----------------------------------------------------------------------------
// Name: LAU_App::Stop
// Created: NLD 2002-09-05
//-----------------------------------------------------------------------------
void LAU_App::Stop()
{
    bRunning_ = false;    
}


//-----------------------------------------------------------------------------
// Name: LAU_App::Instance
// Created: 03-04-24
//-----------------------------------------------------------------------------
LAU_App* LAU_App::Instance()
{
    if( static_pInstance == 0 )
        static_pInstance = new LAU_App();
    return static_pInstance;
}