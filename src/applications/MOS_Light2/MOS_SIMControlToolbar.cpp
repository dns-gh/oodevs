//*****************************************************************************
//
// $Created: FBD 03-01-14 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_SIMControlToolbar.cpp $
// $Author: Ape $
// $Modtime: 7/10/04 17:07 $
// $Revision: 2 $
// $Workfile: MOS_SIMControlToolbar.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_SIMControlToolbar.h"
#include "moc_MOS_SIMControlToolbar.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentServerMsgMgr.h"
#include "MOS_AgentServerController.h"
#include "MOS_AgentServerConnectionMgr.h"
#include "MOS_MOSServer.h"
#include "MOS_ASN_Messages.h"
#include "MOS_ConnectDialog.h"
#include "MOS_DisconnectDialog.h"

//-----------------------------------------------------------------------------
// Name: MOS_SIMControlToolbar constructor
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
MOS_SIMControlToolbar::MOS_SIMControlToolbar( QMainWindow* pParent )
    : QToolBar( pParent, "sim control toolbar" )
{
    this->setLabel( tr( "Contrôle SIM" ) );

    pConnectButton_ = new QToolButton( this );
    pConnectButton_->setAccel( Key_C );
    pConnectButton_->setIconSet( MAKE_ICON( connexionred ) );
    
    pPlayButton_ = new QToolButton( this );
    pPlayButton_->setAccel( Key_P );
    pPlayButton_->setIconSet( MAKE_ICON( stop ) );
    pPlayButton_->setEnabled( false );

    new QLabel( " ", this );
    new QLabel( tr( "Vitesse:" ), this );

    pSpeedSpinBox_ = new QSpinBox( 1, 500, 1, this );
    pSpeedSpinBox_->setValue( 1 );
    pSpeedSpinBox_->setEnabled( false );

    pSpeedButton_ = new QToolButton( this );
    pSpeedButton_->setText( tr( "Ok" ) );
    pSpeedButton_->setEnabled( false );

    pConnectDlg_ = new MOS_ConnectDialog( this );
    pConnectDlg_->hide();

    pDisconnectDlg_ = new MOS_DisconnectDialog( this );
    pDisconnectDlg_->hide();

    connect( pConnectButton_, SIGNAL( clicked() ), this, SLOT( SlotConnectDisconnect() ) );
    connect( pPlayButton_,    SIGNAL( clicked() ), this, SLOT( SlotPlayPause() ) );
    connect( pSpeedButton_,   SIGNAL( clicked() ), this, SLOT( SlotSpeedChange() ) );
    connect( pSpeedSpinBox_, SIGNAL( valueChanged( int ) ), this, SLOT( SlotOnSpinBoxChange() ) );
    connect( &MOS_App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this, SLOT( SlotOnConnexionStatusChanged( bool ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PauseStatusChanged( bool ) ),     this, SLOT( SlotOnPauseStatusChanged( bool ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( SpeedChanged( int ) ),            this, SLOT( SlotOnSpeedChanged( int ) ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_SIMControlToolbar destructor
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
MOS_SIMControlToolbar::~MOS_SIMControlToolbar()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_SIMControlToolbar::SlotConnectDisconnect
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
void MOS_SIMControlToolbar::SlotConnectDisconnect()
{
    if ( MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
    {
        pDisconnectDlg_->show();
    }
    else
    {
        pConnectButton_->setIconSet( MAKE_ICON( connexionorange ) );
        int nResult = pConnectDlg_->exec();
        if( nResult == QDialog::Rejected )
            pConnectButton_->setIconSet( MAKE_ICON( connexionred ) );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_SIMControlToolbar::SlotPlayPause
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
void MOS_SIMControlToolbar::SlotPlayPause()
{
    if ( MOS_App::GetApp().GetMOSServer().GetController().GetMessageMgr().IsPaused() )
    {
        MOS_ASN_MsgCtrlResume asnMsg;
        asnMsg.Send();

        MT_LOG_INFO( "Demande reprise", eSent, 0 );
    }
    else
    {
        MOS_ASN_MsgCtrlPause asnMsg;
        asnMsg.Send();

        MT_LOG_INFO( "Demande pause", eSent, 0 );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_SIMControlToolbar::SlotSpeedChange
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void MOS_SIMControlToolbar::SlotSpeedChange()
{
    if( MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
    {
        MOS_ASN_MsgCtrlChangeTimeFactor asnMsg;
        asnMsg.GetAsnMsg() = pSpeedSpinBox_->value();
        asnMsg.Send();

        std::stringstream strMsg;
        strMsg << "Demande passage vitesse " << pSpeedSpinBox_->value();
        MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_SIMControlToolbar::SlotOnSpinBoxChange
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void MOS_SIMControlToolbar::SlotOnSpinBoxChange()
{
    pSpeedButton_->setEnabled( true );
}


// -----------------------------------------------------------------------------
// Name: MOS_SIMControlToolbar::SlotOnConnexionStatusChanged
/** @param  bConnected 
*/
// Created: APE 2004-03-08
// -----------------------------------------------------------------------------
void MOS_SIMControlToolbar::SlotOnConnexionStatusChanged( bool bConnected )
{
    if ( bConnected )
    {
        pConnectButton_->setIconSet( MAKE_ICON( connexiongreen ) );
        pPlayButton_->setEnabled( true );
        pSpeedSpinBox_->setEnabled( true );
    }
    else
    {
        pConnectButton_->setIconSet( MAKE_ICON( connexionred ) );
        pPlayButton_->setEnabled( false );
        pSpeedSpinBox_->setEnabled( false );
        pSpeedButton_->setEnabled( false );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_SIMControlToolbar::SlotOnPauseStatusChanged
/** @param  bPaused 
*/
// Created: APE 2004-03-08
// -----------------------------------------------------------------------------
void MOS_SIMControlToolbar::SlotOnPauseStatusChanged( bool bPaused )
{
    if ( bPaused )
        pPlayButton_->setIconSet( MAKE_ICON( play ) );
    else
        pPlayButton_->setIconSet( MAKE_ICON( stop ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_SIMControlToolbar::SlotOnSpeedChanged
/** @param  nSpeed 
*/
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void MOS_SIMControlToolbar::SlotOnSpeedChanged( int nSpeed )
{
    pSpeedSpinBox_->setValue( nSpeed );
    pSpeedButton_->setEnabled( false );
}
