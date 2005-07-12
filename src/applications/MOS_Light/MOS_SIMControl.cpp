//*****************************************************************************
//
// $Created: FBD 03-01-14 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_SIMControl.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 18 $
// $Workfile: MOS_SIMControl.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_SIMControl.h"
#include "moc_MOS_SIMControl.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_SIMControl.inl"
#endif

#include "resources.h"

#include "MOS_Value.h"
#include "MOS_ConnectDialog.h"
#include "MOS_DisconnectDialog.h"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentServerMsgMgr.h"
#include "MOS_AgentServerController.h"
#include "MOS_AgentServerConnectionMgr.h"
#include "MOS_MOSServer.h"
#include "MOS_LimaListView.h"
#include "MOS_LimitListView.h"
#include "MOS_AgentEditor.h"
#include "MOS_AgentListView.h"

#include "MOS_ASN_Messages.h"
#include "MOS_TestManager.h"

//-----------------------------------------------------------------------------
// Name: MOS_SIMControl constructor
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
MOS_SIMControl::MOS_SIMControl( QWidget* pParent )
    : QFrame        ( pParent )
{
    QVBoxLayout* pLayout = new QVBoxLayout( this, 3, 3 );

    QGridLayout* pGrid = new QGridLayout( pLayout, 3, 3 );
    pConnectButton_ = new QToolButton( this );
    pConnectButton_->setAccel( Key_C );
    pConnectButton_->setIconSet( MAKE_ICON( l_red ) );
    pConnectButton_->setMaximumWidth( 30 );
    pConnectButton_->setMaximumHeight( 30 );
    
    pStateRunButton_ = new QToolButton( this );
    pStateRunButton_->setAccel( Key_P );
    pStateRunButton_->setIconSet( MAKE_ICON( l_stop ) );
    pStateRunButton_->setMaximumWidth( 30 );
    pStateRunButton_->setMaximumHeight( 30 );
    pStateRunButton_->hide();

    pGrid->addWidget( pConnectButton_, 0, 0 );
    pGrid->addWidget( pStateRunButton_, 0, 1 );

    // Attribute Speed
    pSpeed_ = new MOS_Value( eTypeValueInt, this, "Simulation Speed", MOSToolValueModif | MOSToolValueSpeedo | MOSToolValueTitle | MOSToolValueText );
    pSpeed_->setMaximumWidth( 100 );

    pLayout->addWidget( pSpeed_ );


    pConnectDlg_ = new MOS_ConnectDialog( this );
    pConnectDlg_->hide();

    pDisconnectDlg_ = new MOS_DisconnectDialog( this );
    pDisconnectDlg_->hide();

    connect( pConnectButton_, SIGNAL( clicked() ), this, SLOT( SlotConnection() ) );
    connect( pStateRunButton_, SIGNAL( clicked() ), this, SLOT( SlotPause() ) );
    connect( pSpeed_->GetSpeedo(), SIGNAL( dialReleased() ), this, SLOT( SlotSpeedChange() ) );
}

//-----------------------------------------------------------------------------
// Name: MOS_SIMControl destructor
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
MOS_SIMControl::~MOS_SIMControl()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_SIMControl::SlotConnection
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
void MOS_SIMControl::SlotConnection()
{
    if ( MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
    {
        pDisconnectDlg_->move( QCursor::pos() - QPoint( 0, 300 ) );
        pDisconnectDlg_->show();
    }
    else
    {
        pConnectButton_->setIconSet( MAKE_ICON( l_white ) );
        pConnectDlg_->Initialize();
        pConnectDlg_->move( QCursor::pos() - QPoint( 0, 300 ) );
        pConnectDlg_->show();
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_SIMControl::SlotSpeedChange
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
void MOS_SIMControl::SlotSpeedChange()
{
    if( MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
    {
        MOS_ASN_MsgCtrlChangeTimeFactor asnMsg;
        asnMsg.GetAsnMsg() = pSpeed_->GetValueWidget();
        asnMsg.Send();
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_SIMControl::SetInitSpeed
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_SIMControl::SetInitSpeed()
{
    pSpeed_->SetValueMinWidget( 1 );
    pSpeed_->SetValueWidget( 10 );
    pSpeed_->SetValueMaxWidget( 255 );
    pSpeed_->hide();
}
//-----------------------------------------------------------------------------
// Name: MOS_SIMControl::SlotPause
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
void MOS_SIMControl::SlotPause()
{
    if ( MOS_App::GetApp().GetMOSServer().GetController().GetMessageMgr().IsPaused() )
    {
        pStateRunButton_->hide();
        MOS_ASN_MsgCtrlResume asnMsg;
        asnMsg.Send();
    }
    else
    {
        pStateRunButton_->hide();
        MOS_ASN_MsgCtrlPause asnMsg;
        asnMsg.Send();
    }
    pStateRunButton_->show();
}


//-----------------------------------------------------------------------------
// Name: MOS_SIMControl::NotifyStatePause
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
void MOS_SIMControl::NotifyStatePause( bool bPaused )
{
    if ( bPaused )
    {
        pStateRunButton_->show();
        pStateRunButton_->setIconSet( MAKE_ICON( l_play ) );
    }
    else
    {
        pStateRunButton_->show();
        pStateRunButton_->setIconSet( MAKE_ICON( l_stop ) );
    }

}

//-----------------------------------------------------------------------------
// Name: MOS_SIMControl::NotifyConnection
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
void MOS_SIMControl::NotifyConnection( bool bConnected )
{
    if ( bConnected )
    {
        MOS_App::GetApp().GetMainWindow().GetLimaEditor().Read();
        MOS_App::GetApp().GetMainWindow().GetLimitEditor().Read();

        if ( !MOS_App::GetApp().GetTestManager().GetFlagRec() && !MOS_App::GetApp().GetTestManager().GetFlagTest() && !MOS_App::GetApp().GetTestManager().GetFlagReplay() )
            pConnectButton_->setIconSet( MAKE_ICON( l_green ) );
        else
            pConnectButton_->setIconSet( MAKE_ICON( affinity ) );

        SetInitSpeed();
        SlotSpeedChange();
        pSpeed_->show();

        pStateRunButton_->show();
    }
    else
    {
        MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().NotifyDisconnected();
        pConnectButton_->setIconSet( MAKE_ICON( l_red ) );
        pSpeed_->hide();
        pStateRunButton_->hide();
        MOS_App::GetApp().GetTestManager().SetFlagTest( false );
        MOS_App::GetApp().GetTestManager().SetFlagReplay( false );
        MOS_App::GetApp().GetTestManager().SetFlagRec( false );

    }

}

