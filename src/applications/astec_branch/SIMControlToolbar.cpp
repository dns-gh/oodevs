//*****************************************************************************
//
// $Created: FBD 03-01-14 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/SIMControlToolbar.cpp $
// $Author: Ape $
// $Modtime: 7/10/04 17:07 $
// $Revision: 2 $
// $Workfile: SIMControlToolbar.cpp $
//
//*****************************************************************************

#include "astec_pch.h"

#include "SIMControlToolbar.h"
#include "moc_SIMControlToolbar.cpp"

#include "ASN_Messages.h"
#include "ConnectDialog.h"
#include "DisconnectDialog.h"
#include "MT_SpinBox.h"
#include "Simulation.h"
#include "Controllers.h"

//-----------------------------------------------------------------------------
// Name: SIMControlToolbar constructor
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
SIMControlToolbar::SIMControlToolbar( QMainWindow* pParent, Controllers& controllers )
    : QToolBar( pParent, "sim control toolbar" )
    , controllers_( controllers )
    , connected_( false )
    , paused_( false )
{
    setLabel( tr( "Contrôle SIM" ) );

    pConnectButton_ = new QToolButton( this );
    pConnectButton_->setAccel( Key_C );
    pConnectButton_->setIconSet( MAKE_ICON( connexionred ) );
    pConnectButton_->setTextLabel( tr( "Connection (C)" ) );
    
    pPlayButton_ = new QToolButton( this );
    pPlayButton_->setAccel( Key_P );
    pPlayButton_->setIconSet( MAKE_ICON( stop ) );
    pPlayButton_->setTextLabel( tr( "Pause (P)" ) );
    pPlayButton_->setEnabled( false );

    new QLabel( " ", this );
    new QLabel( tr( "Vitesse:" ), this );

    pSpeedSpinBox_ = new MT_SpinBox( 1, 500, 1, this );
    pSpeedSpinBox_->setButtonSymbols( QSpinBox::PlusMinus );
    pSpeedSpinBox_->setValue( 1 );
    pSpeedSpinBox_->setEnabled( false );

    pSpeedButton_ = new QToolButton( this );
    pSpeedButton_->setText( tr( "Ok" ) );
    pSpeedButton_->setEnabled( false );

    pConnectDlg_ = new ConnectDialog( this );
    pConnectDlg_->hide();

    pDisconnectDlg_ = new DisconnectDialog( this );
    pDisconnectDlg_->hide();

    connect( pConnectButton_, SIGNAL( clicked() ), this, SLOT( SlotConnectDisconnect() ) );
    connect( pPlayButton_,    SIGNAL( clicked() ), this, SLOT( SlotPlayPause() ) );
    connect( pSpeedButton_,   SIGNAL( clicked() ), this, SLOT( SlotSpeedChange() ) );
    connect( pSpeedSpinBox_ , SIGNAL( valueChanged( int ) ), this, SLOT( SlotOnSpinBoxChange() ) );
    connect( pSpeedSpinBox_ , SIGNAL( enterPressed() ), this, SLOT( SlotOnSpinBoxEnterPressed() ) );

    controllers_.Register( *this );
}

//-----------------------------------------------------------------------------
// Name: SIMControlToolbar destructor
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
SIMControlToolbar::~SIMControlToolbar()
{
    controllers_.Remove( *this );
}

//-----------------------------------------------------------------------------
// Name: SIMControlToolbar::SlotConnectDisconnect
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
void SIMControlToolbar::SlotConnectDisconnect()
{
    if ( connected_ )
    {
        pDisconnectDlg_->show();
    }
    else
    {
        pConnectButton_->setIconSet( MAKE_ICON( connexionorange ) );
        pConnectButton_->setTextLabel( tr( "Connection (C)" ) );
        int nResult = pConnectDlg_->exec();
        if( nResult == QDialog::Rejected )
        {
            pConnectButton_->setIconSet( MAKE_ICON( connexionred ) );
            pConnectButton_->setTextLabel( tr( "Deconnection (C)" ) );
        }
    }
}

//-----------------------------------------------------------------------------
// Name: SIMControlToolbar::SlotPlayPause
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
void SIMControlToolbar::SlotPlayPause()
{
    if ( paused_ )
    {
        ASN_MsgCtrlResume asnMsg;
        asnMsg.Send();
    }
    else
    {
        ASN_MsgCtrlPause asnMsg;
        asnMsg.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::SlotSpeedChange
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void SIMControlToolbar::SlotSpeedChange()
{
    if( connected_ )
    {
        ASN_MsgCtrlChangeTimeFactor asnMsg;
        asnMsg.GetAsnMsg() = pSpeedSpinBox_->value();
        asnMsg.Send();
    }
}


// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::SlotOnSpinBoxChange
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void SIMControlToolbar::SlotOnSpinBoxChange()
{
    pSpeedButton_->setEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::SlotOnSpinBoxEnterPressed
// Created: SBO 2005-11-07
// -----------------------------------------------------------------------------
void SIMControlToolbar::SlotOnSpinBoxEnterPressed()
{
    SlotSpeedChange();
}

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::NotifyUpdated
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void SIMControlToolbar::NotifyUpdated( const Simulation& simulation )
{
    connected_ = simulation.IsConnected();
    if( connected_ )
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

    paused_ = simulation.IsPaused();
    if( paused_ )
    {
        pPlayButton_->setIconSet( MAKE_ICON( play ) );
        pPlayButton_->setTextLabel( tr( "Reprise (P)" ) );
    }
    else
    {
        pPlayButton_->setIconSet( MAKE_ICON( stop ) );
        pPlayButton_->setTextLabel( tr( "Pause (P)" ) );
    }

    pSpeedSpinBox_->setValue( simulation.GetSpeed() );
    pSpeedButton_->setEnabled( false );
}
