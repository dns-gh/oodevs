//*****************************************************************************
//
// $Created : AML 03-04-02 $
//  $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/LMP_MainWindow.cpp $
//   $Author: Nld $
//  $Modtime: 23/07/04 17:43 $
// $Revision: 21 $
// $Workfile: LMP_MainWindow.cpp $
//
//*****************************************************************************

#include "LMP_pch.h"
#include "LMP_MainWindow.h" 

#include "moc_LMP_MainWindow.cpp"

#ifndef USE_INLINE
#   include "LMP_MainWindow.inl"
#endif

#include "LMP_Workspace.h"

#include "QToolBar.h"
#include "QToolButton.h"
#include "QPushButton.h"
#include "QListBox.h"
#include "QTable.h"
#include "QLabel.h"
#include "QLayout.h"
#include "QLineEdit.h"
#include "QButton.h"
#include "QValidator.h"
#include "QMessageBox.h"



#include "Icon.Xpm"
#include "IconError.Xpm"

LMP_MainWindow* LMP_MainWindow::static_pTheOnlyMainWindow = NULL;

//-----------------------------------------------------------------------------
// Name   : LMP_MainWindow constructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
LMP_MainWindow::LMP_MainWindow( LMP_Workspace& workspace )
    :   workSpace_     (   workspace   )
{
    static_pTheOnlyMainWindow = this;

    move( 640, 320 );
    resize( 400, 400 );
	// create a toolbar
    QToolBar* pToolBar = new QToolBar( this, "toolbar" );



    pButtonReconnect_ = new QToolButton( pToolBar );
    pButtonReconnect_->setText( "Reconnect" );
    connect( pButtonReconnect_, SIGNAL( clicked( void ) ), SLOT( SlotPushButtonReconnect( void ) ) );


    pButtonRequestListMission_ = new QToolButton( pToolBar );
    pButtonRequestListMission_->setText( "RequestMissions" );
    connect( pButtonRequestListMission_, SIGNAL( clicked( void ) ), SLOT( SlotPushButtonRequestListMission( void ) ) );

    pButtonRequestListSim_ = new QToolButton( pToolBar );
    pButtonRequestListSim_->setText( "Request Existent Sims" );
    connect( pButtonRequestListSim_, SIGNAL( clicked( void ) ), SLOT( SlotPushButtonRequestListSim( void ) ) );

    pButtonRequestListCheckpoint_ = new QToolButton( pToolBar );
    pButtonRequestListCheckpoint_->setText( "RequestCheckpoints" );
    connect( pButtonRequestListCheckpoint_, SIGNAL( clicked( void ) ), SLOT( SlotPushButtonRequestListCheckpoint( void ) ) );

    

////    pButtonLaunchSelectedMission_ = new QToolButton( pToolBar );
////    pButtonLaunchSelectedMission_->setText( "LaunchSelectedMission" );
////    connect( pButtonLaunchSelectedMission_, SIGNAL( clicked( void ) ), SLOT( SlotPushButtonLaunchSelectedMission( void ) ) );

    
    const int nHauteurInCellOfList = 6; // should be pair

    QWidget* pCentral = new QWidget( this );
    setCentralWidget( pCentral );

    QGridLayout* pLayout = new QGridLayout( pCentral, 4+nHauteurInCellOfList*2, 2, 2, 2 );

    int nIndexInGrid = 0;

    // Line Host
    pLineEditHostname_ = new QLineEdit( "localhost", pCentral );
    pLayout->addWidget( pLineEditHostname_, nIndexInGrid, 0 );
    ++nIndexInGrid;


    // Line listmission + launch
    pListBoxExistingMission_ = new QListBox( pCentral );
    pListBoxExistingMission_->setCaption( "Exercice existant" );
    pLayout-> addMultiCellWidget( pListBoxExistingMission_, nIndexInGrid, nIndexInGrid+nHauteurInCellOfList/2-1, 0, 0 );
//    pListBoxExistingMission_->resize( 200, 100 );

    pListBoxExistingCheckpoint_ = new QListBox( pCentral );
    pListBoxExistingCheckpoint_->setCaption( "Exercice existant" );
    pLayout-> addMultiCellWidget( pListBoxExistingCheckpoint_, nIndexInGrid+nHauteurInCellOfList/2, nIndexInGrid+nHauteurInCellOfList, 0, 0 );
//    pListBoxExistingMission_->resize( 200, 100 );
    

    pLineEditOption_ = new QLineEdit( "-nodisplay", pCentral );
    pLayout->addWidget( pLineEditOption_, nIndexInGrid, 1 );

    pButtonLaunchSelectedMission_ = new QPushButton( "LaunchSelectedMission", pCentral );
    connect( pButtonLaunchSelectedMission_, SIGNAL( clicked( void ) ), SLOT( SlotPushButtonLaunchSelectedMission( void ) ) );
    pLayout->addWidget( pButtonLaunchSelectedMission_, nIndexInGrid+1, 1 );

    pButtonLaunchSelectedCheckpoint_ = new QPushButton( "LaunchSelectedCheckpoint", pCentral );
    connect( pButtonLaunchSelectedCheckpoint_, SIGNAL( clicked( void ) ), SLOT( SlotPushButtonLaunchSelectedCheckpoint( void ) ) );
    pLayout->addWidget( pButtonLaunchSelectedCheckpoint_, nIndexInGrid+2, 1 );

    

    nIndexInGrid += nHauteurInCellOfList+1;


    // line sim status
    const int nNbrCols = 6;
    const int nNbrRows = 8;
    pTableSimState_ = new QTable( pCentral );
    pTableSimState_->setCaption( "SimState" );
    pTableSimState_->setNumRows( nNbrRows );
    pTableSimState_->setNumCols( nNbrCols );

    int nNumCol = 0;
    pTableSimState_->horizontalHeader()->setLabel( nNumCol, "Launched" );++nNumCol;
    pTableSimState_->horizontalHeader()->setLabel( nNumCol, "Init (ec)" );++nNumCol;
    pTableSimState_->horizontalHeader()->setLabel( nNumCol, "Running" );++nNumCol;
////    pTableSimState_->horizontalHeader()->setLabel( nNumCol, "Stopped" );++nNumCol;
    pTableSimState_->horizontalHeader()->setLabel( nNumCol, "Success" );++nNumCol;
    pTableSimState_->horizontalHeader()->setLabel( nNumCol, "Port" );++nNumCol;
    pTableSimState_->horizontalHeader()->setLabel( nNumCol, "Magic" );++nNumCol;
    

    int i, j;

    // init de la table:
    for( j = 0; j < nNbrRows; ++j )
    {
        pTableSimState_->verticalHeader()->setLabel( j, MT_FormatString( "Sim%d", j ).c_str() );
        pTableSimState_->setItem( j, 0, new QTableItem( pTableSimState_, QTableItem::Never, "false" ) );

        for( i = 1; i < nNbrCols; ++i )
        {
            pTableSimState_->setItem( j, i, new QTableItem( pTableSimState_, QTableItem::Never, "" ) );
        }
    }
    


    pLayout->addMultiCellWidget( pTableSimState_, nIndexInGrid, nIndexInGrid+nHauteurInCellOfList, 0, 0 );
//    pTableSimState_->resize( 200, 100 );

    for( j = 0; j < nNbrRows; ++j )
    {
        pTableSimState_->adjustRow( j );
    }

    for( i = 0; i < nNbrCols; ++i )
    {
        pTableSimState_->adjustColumn( i );
    }


    nIndexInGrid += nHauteurInCellOfList+1;

    
    
    // line sim status
    pListBoxSimStatus_ = new QListBox( pCentral );
    pListBoxSimStatus_->setCaption( "SimStatus" );
    pLayout->addMultiCellWidget( pListBoxSimStatus_, nIndexInGrid, nIndexInGrid+nHauteurInCellOfList, 0, 0 );
//    pListBoxSimStatus_->resize( 200, 100 );

    nIndexInGrid += nHauteurInCellOfList+1;


    // line stop sim
    pLineEditExerciceID_ = new QLineEdit( "0", pCentral );
    pLineEditExerciceID_->setValidator( new QIntValidator( pCentral ) );
    pLayout->addWidget( pLineEditExerciceID_, nIndexInGrid, 0 );

    pButtonStopSimulation_ = new QPushButton( "Stop Sim", pCentral );      // (not owned)
    connect( pButtonStopSimulation_, SIGNAL( clicked( void ) ), SLOT( SlotPushButtonStopSimulation( void ) ) );
    pLayout->addWidget( pButtonStopSimulation_, nIndexInGrid, 1 );
    ++nIndexInGrid;


    // line stop all sim
    pButtonStopAllSimulation_ = new QPushButton( "Stop All Sim", pCentral );      // (not owned)
    connect( pButtonStopAllSimulation_, SIGNAL( clicked( void ) ), SLOT( SlotPushButtonStopAllSimulation( void ) ) );
    pLayout->addWidget( pButtonStopAllSimulation_, nIndexInGrid, 1 );
    ++nIndexInGrid;
    

    // line status
    pLabelStatus_ = new QLabel( pCentral );
    pLabelStatus_->setText( "(status)" );
////    pLabelStatus_->resize( 200, 20 );
    pLayout->addWidget( pLabelStatus_, nIndexInGrid, 0 );
    ++nIndexInGrid;

    pLayout->setColStretch( 0, 15 );
    pLayout->setColStretch( 1, 1 ); // cette ligne ne change rien!


    
////    QPushButton* pb = new QPushButton( pCentral );
////    pb->show();
////    pLayout->addWidget( pb, 2, 1 );


    setIcon( * new QPixmap ( xpm_Icon ) );

    pPixmapError_ = new QPixmap ( xpm_IconError );
}



//-----------------------------------------------------------------------------
// Name   : LMP_MainWindow destructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
LMP_MainWindow::~LMP_MainWindow()
{

}


//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::SetConnected
// Created: AML 03-04-17
//-----------------------------------------------------------------------------
void LMP_MainWindow::SetConnected( const bool32 bNewVal )
{
    if( bNewVal )
    {
        pLabelStatus_->setText( "connected" );
    }
    else
    {
        pLabelStatus_->setText( "not connected" );
    }
}


//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::ShowLastLineOfListBox
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
void LMP_MainWindow::ShowLastLineOfListBox( void ) const
{
////    pListBoxExistingMission_->update();
////    pListBoxExistingMission_->updateContents();
////    pListBoxExistingMission_->show();

////    pListBoxExistingMission_->setCurrentItem( 8 );
////    pListBoxExistingMission_->centerCurrentItem();
////    pListBoxExistingMission_->setTopItem( pListBoxExistingMission_->count() - pListBoxExistingMission_->numItemsVisible() );
////    pListBoxExistingMission_->setBottomItem( pMsg->n );
////    pListBoxExistingMission_->setBottomItem( pListBoxExistingMission_->count() );
////    pListBoxExistingMission_->update();

    // se positionne sur la derniere ligne (marche pas la derniere fois, va chier va....)
    if( pListBoxExistingMission_->count() > 0 )
    {
        pListBoxExistingMission_->setBottomItem( pListBoxExistingMission_->count()-1 );
    }
    if( pListBoxExistingCheckpoint_->count() > 0 )
    {
        pListBoxExistingCheckpoint_->setBottomItem( pListBoxExistingCheckpoint_->count()-1 );
    }

    // se positionne sur la derniere ligne
    if( pListBoxSimStatus_->count() > 0 )
    {
        pListBoxSimStatus_->setBottomItem( pListBoxSimStatus_->count()-1 );
    }
}



//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::ClearListMission
// Created: AML 03-04-17
//-----------------------------------------------------------------------------
void LMP_MainWindow::ClearListMission()
{
    pListBoxExistingMission_->clear();
}


//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::ClearListCheckpoint
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
void LMP_MainWindow::ClearListCheckpoint()
{
    pListBoxExistingCheckpoint_->clear();
}



//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::UpdateListMission
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
void LMP_MainWindow::UpdateListMission( ASN1T_MsgLauMos_ListMission* pMsg )
{
    ClearListMission();

    uint i;
    for( i = 0; i < pMsg->n; ++i )
    {
        char buf[128];
        sprintf( buf, "%3d: %s (uuid:%s)", pMsg->elem[i].nID, pMsg->elem[i].libelle, pMsg->elem[i].uuid_odb );
        pListBoxExistingMission_->insertItem( buf  );
    }
    
    ShowLastLineOfListBox();
}


//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::UpdateListSim
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
void LMP_MainWindow::UpdateListSim( ASN1T_MsgLauMos_ListSim* pMsg )
{
    uint i;

    for( i = 0; i < 8; ++i )
    {
        // ResetTableInfoSim( i );
        pTableSimState_->item( i, 0 )->setText( "false" );
    }
    pTableSimState_->repaintContents();


    for( i = 0; i < pMsg->n; ++i )
    {
        char buf[128];
        if( pMsg->elem[i].m.launch_informationPresent )
        {
            sprintf( buf, "SIM ID %d: port:%d magic:%d errorCode:%s", pMsg->elem[i].id_exercice, pMsg->elem[i].launch_information.num_port, pMsg->elem[i].launch_information.din_magic_number, GetLibelleFromLaunchErrorCode( pMsg->elem[i].launch_information.error_code ) );
        }
        else
        {
            sprintf( buf, "SIM ID %d: (no info present (init not finished?))", pMsg->elem[i].id_exercice );
        }
        pListBoxSimStatus_->insertItem( buf  );

        if( pMsg->elem[i].m.launch_informationPresent )
        {
            UpdateTableInfoSim( pMsg->elem[i].id_exercice, 1, 1, pMsg->elem[i].launch_information.error_code, 1, -1, -1, pMsg->elem[i].launch_information.num_port, pMsg->elem[i].launch_information.din_magic_number );
        }
        else
        {
            UpdateTableInfoSim( pMsg->elem[i].id_exercice, 1, 0, -1, 0, 0, -1, 0, 0 );
        }

    }

    ShowLastLineOfListBox();
}


//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::UpdateListCheckpoint
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
void LMP_MainWindow::UpdateListCheckpoint( ASN1T_MsgLauMos_ListCheckpoint* pMsg )
{
    ClearListCheckpoint();

    char buf[512];
    uint i;
    for( i = 0; i < pMsg->list_checkpoint.n; ++i )
    {
        if( pMsg->list_checkpoint.elem[i].m.nom_checkpointPresent )
            sprintf( buf, "%3d: [%s] %s (%s)", pMsg->list_checkpoint.elem[i].nID, pMsg->list_checkpoint.elem[i].nom_checkpoint, pMsg->list_checkpoint.elem[i].libelle_scenario, pMsg->list_checkpoint.elem[i].libelle_checkpoint );
        else
            sprintf( buf, "%3d: %s (%s)", pMsg->list_checkpoint.elem[i].nID, pMsg->list_checkpoint.elem[i].libelle_scenario, pMsg->list_checkpoint.elem[i].libelle_checkpoint );
        pListBoxExistingCheckpoint_->insertItem( buf  );
    }
    
    ShowLastLineOfListBox();
}




//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::UpdateInfoLaunch
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
void LMP_MainWindow::UpdateInfoLaunch( const uint32 nExerciceID, const ASN1T_EnumCtrlErrorCodeLauncher nErrorCode, const uint16 nPort, const int nMagic )
{
    std::stringstream strStatus;

    strStatus << "SimExoID: " << nExerciceID << " - Launch: ";

    strStatus << GetLibelleFromLaunchErrorCode( nErrorCode );


    strStatus << ", Port: " << nPort;
    strStatus << ", Magic: " << nMagic;


    // pLabelStatus_->setText( strStatus.c_str() );
    pListBoxSimStatus_->insertItem( strStatus.str().c_str() );

    if( nErrorCode == EnumCtrlErrorCodeLauncher::no_error )
    {
        UpdateTableInfoSim( nExerciceID, -1, 1, nErrorCode, 1, -1, 0, nPort, nMagic );
    }
    else
    {
        UpdateTableInfoSim( nExerciceID, -1, 0, nErrorCode, 0, -1, 0, 0, 0 );
    }

    ShowLastLineOfListBox();

}


//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::UpdateInfoFailure
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
void LMP_MainWindow::UpdateInfoFailure( const uint32 nExerciceID )
{
    std::stringstream strStatus;
    strStatus << "Sim Exercice " << nExerciceID << " - FAILURE";

    pListBoxSimStatus_->insertItem( *pPixmapError_, strStatus.str().c_str() );

    UpdateTableInfoSim( nExerciceID, -1, -1, -1, 0, -1, 0 );


    ShowLastLineOfListBox();

}


//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::UpdateInfoExecSuccess
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
void LMP_MainWindow::UpdateInfoExecSuccess( const uint32 nExerciceID, const bool32 bFinishedState )
{
    std::stringstream strStatus;
    strStatus << "Sim Exercice " << nExerciceID << " - ExecSuccess - Finished state: " << (bFinishedState?"true":"false");

    pListBoxSimStatus_->insertItem( strStatus.str().c_str() );

    UpdateTableInfoSim( nExerciceID, -1, -1, -1, 0, -1, 1 );

    ShowLastLineOfListBox();

}


//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::ResetTableInfoSim
// Created: AML 03-06-20
//-----------------------------------------------------------------------------
void LMP_MainWindow::ResetTableInfoSim( const uint32 nExerciceID )
{
    int nNumCol = 0;

    for( nNumCol = 0; nNumCol < pTableSimState_->numCols(); ++nNumCol )
    {
        pTableSimState_->item( nExerciceID, nNumCol )->setText( nNumCol==0?"false":"" );
    }

    pTableSimState_->repaintContents();
}


//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::UpdateTableInfoSim
// Object: si un champ est different de -1, le met a jour dans la table
// Created: AML 03-06-16
//-----------------------------------------------------------------------------
#define BOOL_TO_STRING(b) (b!=0?"true":"false")
void LMP_MainWindow::UpdateTableInfoSim( const uint32 nExerciceID, const int32 nLaunched, const int32 nInit, const int32 nInitStatus, const int32 nRunning, const int32 nStopped, const int32 nExecSuccess, const int32 nPort, const int32 nMagic )
{
    int nNumCol = 0;

    

    if( nLaunched != -1 )
    {
        pTableSimState_->item( nExerciceID, nNumCol )->setText( BOOL_TO_STRING( nLaunched ) );
    }
    ++nNumCol;

    if( nInit != -1 )
    {
        pTableSimState_->item( nExerciceID, nNumCol )->setText( MT_FormatString( "%s (%s)", BOOL_TO_STRING( nInit ), GetLibelleFromLaunchErrorCode( nInitStatus ) ).c_str() );
    }
    ++nNumCol;

    if( nRunning != -1 )
    {
        pTableSimState_->item( nExerciceID, nNumCol )->setText( BOOL_TO_STRING( nRunning ) );
    }
    ++nNumCol;

    nStopped;
////    if( nStopped != -1 )
////    {
////        pTableSimState_->item( nExerciceID, nNumCol )->setText( BOOL_TO_STRING( nStopped ) );
////    }
////    ++nNumCol;

    if( nExecSuccess != -1 )
    {
        pTableSimState_->item( nExerciceID, nNumCol )->setText( BOOL_TO_STRING( nExecSuccess ) );
    }
    ++nNumCol;

    if( nPort != -1 )
    {
        pTableSimState_->item( nExerciceID, nNumCol )->setText( MT_FormatString( "%d", nPort ).c_str() );
        pTableSimState_->adjustColumn( nNumCol );
    }
    ++nNumCol;

    if( nMagic != -1 )
    {
        pTableSimState_->item( nExerciceID, nNumCol )->setText( MT_FormatString( "%d", nMagic ).c_str() );
    }
    ++nNumCol;

    pTableSimState_->repaintContents();
}





//=============================================================================
// SLOTS
//==========================================================================<===


//-----------------------------------------------------------------------------
// Name   : LMP_MainWindow::SlotPushButtonReconnect
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-03
//-----------------------------------------------------------------------------
void LMP_MainWindow::SlotPushButtonReconnect()
{
    pLabelStatus_->setText( "connecting..." );
    workSpace_.Reconnect( pLineEditHostname_->text().ascii() );
}


//-----------------------------------------------------------------------------
// Name   : LMP_MainWindow::SlotPushButtonRequestListMission
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
void LMP_MainWindow::SlotPushButtonRequestListMission()
{
    workSpace_.SendMsg_RequestListMission();
}


//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::SlotPushButtonRequestListSim
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
void LMP_MainWindow::SlotPushButtonRequestListSim()
{
    workSpace_.SendMsg_RequestListSim();
}


//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::SlotPushButtonRequestListCheckpoint
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
void LMP_MainWindow::SlotPushButtonRequestListCheckpoint()
{
    const char* pText = pLineEditExerciceID_->text();
    if( pText == NULL || *pText == '\0' )
    {
	    QMessageBox mb( "Error", "You must enter an ID number in the LineEditField", QMessageBox::Warning, QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton, QMessageBox::NoButton );
	    mb.exec();
        return;
    }

    workSpace_.SendMsg_RequestListCheckpoint( atoi( pLineEditExerciceID_->text() ) );

}



//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::SlotPushButtonLaunchSelectedMission
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
void LMP_MainWindow::SlotPushButtonLaunchSelectedMission()
{
    const char* szSelectedText = pListBoxExistingMission_->currentText();
    if( szSelectedText == NULL )
    {
	    QMessageBox mb( "Error", "You must select a scenario in the list (when connected, push RequestMissions button)", QMessageBox::Warning, QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton, QMessageBox::NoButton );
	    mb.exec();

        return;
    }

    int nNumSelectedMission = atoi( szSelectedText );

    if( nNumSelectedMission != 0 )
    {
        const char* pText = pLineEditOption_->text();
        if( pText == NULL || *pText == '\0' )
        {
            strCommandLineToSend_ = "";
        }
        else
        {
            strCommandLineToSend_ = pText;
        }

        workSpace_.SendMsg_LaunchSim( 0, nNumSelectedMission, strCommandLineToSend_ );
    }
}


//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::SlotPushButtonLaunchSelectedCheckpoint
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
void LMP_MainWindow::SlotPushButtonLaunchSelectedCheckpoint()
{
    const char* szSelectedText = pListBoxExistingCheckpoint_->currentText();
    if( szSelectedText == NULL )
    {
	    QMessageBox mb( "Error", "You must select a checkpoint in the list (when connected, push RequestCheckpoints button)", QMessageBox::Warning, QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton, QMessageBox::NoButton );
	    mb.exec();

        return;
    }

    int nNumSelectedCheckpoint = atoi( szSelectedText );

    if( nNumSelectedCheckpoint != 0 )
    {
        const char* pText = pLineEditOption_->text();
        if( pText == NULL || *pText == '\0' )
        {
            strCommandLineToSend_ = "";
        }
        else
        {
            strCommandLineToSend_ = pText;
        }

        workSpace_.SendMsg_LaunchSimFromCheckpoint( 0, nNumSelectedCheckpoint, strCommandLineToSend_ );
    }
}



//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::SlotPushButtonStopSimulation
// Created: AML 03-04-17
//-----------------------------------------------------------------------------
void LMP_MainWindow::SlotPushButtonStopSimulation()
{
    const char* pText = pLineEditExerciceID_->text();
    if( pText == NULL || *pText == '\0' )
    {
	    QMessageBox mb( "Error", "You must enter an ID number in the LineEditField", QMessageBox::Warning, QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton, QMessageBox::NoButton );
	    mb.exec();
        return;
    }

    workSpace_.SendMsg_StopSim( atoi( pLineEditExerciceID_->text() ) );
}

//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::SlotPushButtonStopAllSimulation
// Created: AML 03-04-17
//-----------------------------------------------------------------------------
void LMP_MainWindow::SlotPushButtonStopAllSimulation()
{
    int i;
    for( i = 0; i < 8; ++i )
    {
        workSpace_.SendMsg_StopSim( i );
    }
}



//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::GetLibelleFromLaunchErrorCode
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
const char* LMP_MainWindow::GetLibelleFromLaunchErrorCode( const int32 nLaunchErrorCode )
{
    static char aszLibelleFromState[][36] =
    {
      "no_error",
      "error_already_started",
      "error_not_started",
      "error_not_paused",
      "error_already_paused",
      "error_invalid_time_factor",
      "error_invalid_scenario_identifier",
      "error_error_scenario_init",
      "error_invalid_data_path",
      "error_invalid_data",
      "error_invalid_checkpoint_identifier",
      "error_error_checkpoint_init",
      "error_sim_killed_by_remote",
      "???",
    };

    const int eLaunchErrorCodeValueMax = EnumCtrlErrorCodeLauncher::error_sim_killed_by_remote + 1;

    if( (int)nLaunchErrorCode < EnumCtrlErrorCodeLauncher::no_error || (int)nLaunchErrorCode >= eLaunchErrorCodeValueMax )
    {
        return aszLibelleFromState[eLaunchErrorCodeValueMax];
    }

    return aszLibelleFromState[(int)nLaunchErrorCode];
}