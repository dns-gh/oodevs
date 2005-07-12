//*****************************************************************************
//
// $Created : AML 03-04-02 $
//  $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/LMP_Workspace.cpp $
//   $Author: Aml $
//  $Modtime: 25/07/03 11:53 $
// $Revision: 11 $
// $Workfile: LMP_Workspace.cpp $
//
//*****************************************************************************

#include "LMP_pch.h"
#include "LMP_Workspace.h"


#ifndef USE_INLINE
#   include "LMP_Workspace.inl"
#endif

#define LMP_WORKSPACE_LAU_NUM_PORT  4321

#include "LMP_App.h"
#include "LMP_MainWindow.h"



using namespace DIN;
//-----------------------------------------------------------------------------
// Name   : LMP_Workspace constructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
LMP_Workspace::LMP_Workspace(  LMP_App& app )
    :   app_                (       app     )
    ,   pEngine_            (       0       )
    ,   nNextExerciceID_    (       0       )
{
    // dans notre cas le DIN_Engine est unique, dans le cas de mos, ils passeront le leur
    pEngine_    = new DIN_Engine;

    pLau_       = new MOS_LAU( app.GetLauncherHostname(), LMP_WORKSPACE_LAU_NUM_PORT, pEngine_ );
}



//-----------------------------------------------------------------------------
// Name   : LMP_Workspace destructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
LMP_Workspace::~LMP_Workspace()
{
    delete pLau_;
    delete pEngine_;
}


//-----------------------------------------------------------------------------
// Name   : LMP_Workspace::Reconnect
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-03
//-----------------------------------------------------------------------------
void LMP_Workspace::Reconnect( const std::string& strHostName )
{
////    if( pLau_ != NULL )
////    {
////        pLau_->CloseConnection();

////        delete pLau_;
////    }
////    delete pEngine_;
////    pEngine_ = new DIN_Engine;
////
////    pLau_ = new MOS_LAU();  // il faut pas le detruire: on peut pas creer et detruire des messages service comme ca c'est interdit, parceque ya un ordre a respecter, donc a l'avenir, il ne faudra pas detruire MOS_LAU


    pLau_->CloseConnection();

    pLau_->SetDinEngine( pEngine_ );
    pLau_->SetHostName( strHostName );

    pLau_->ConnectToLauncher();
}


//-----------------------------------------------------------------------------
// Name   : LMP_Workspace::Execute
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
void LMP_Workspace::Execute( void )
{
    assert( pLau_ );
    pLau_->Update();
}



//-----------------------------------------------------------------------------
// Name: LMP_Workspace::SendMsg_LaunchSim
// Object : if nExerciceID == 0, give automatically an exerciceID
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
bool32 LMP_Workspace::SendMsg_LaunchSim( const uint32 nExerciceID, const uint32 nScenarioID, const std::string& strCommandLine )
{
    if( nNextExerciceID_ > 7 )
    {
        nNextExerciceID_ = 0;
    }

    LMP_MainWindow::GetMainWindow()->UpdateTableInfoSim( nNextExerciceID_, 1, 0 );

    return pLau_->SendMsg_MosLau_LaunchSim( nExerciceID!=0?nExerciceID:nNextExerciceID_++, nScenarioID, strCommandLine );
}


//-----------------------------------------------------------------------------
// Name: LMP_Workspace::SendMsg_LaunchSimFromCheckpoint
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
bool32 LMP_Workspace::SendMsg_LaunchSimFromCheckpoint( const uint32 nExerciceID, const uint32 nCheckpointID, const std::string& strCommandLine )
{
    if( nNextExerciceID_ > 7 )
    {
        nNextExerciceID_ = 0;
    }

    LMP_MainWindow::GetMainWindow()->UpdateTableInfoSim( nNextExerciceID_, 1, 0 );

    return pLau_->SendMsg_MosLau_LaunchSimFromCheckpoint( nExerciceID!=0?nExerciceID:nNextExerciceID_++, nCheckpointID, strCommandLine );
}

