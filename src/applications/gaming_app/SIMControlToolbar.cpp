// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"

#include "SIMControlToolbar.h"
#include "moc_SIMControlToolbar.cpp"

#include "ConnectDialog.h"
#include "DisconnectDialog.h"
#include "gaming/ASN_Messages.h"
#include "gaming/Simulation.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/resources.h"

using namespace kernel;

namespace 
{
    class SpinBox : public QSpinBox
    {
    public:
        SpinBox( int minValue, int maxValue, int step, QWidget* parent, SIMControlToolbar& toolbar )
            : QSpinBox( minValue, maxValue, step, parent )
            , toolBar_( toolbar )
        {
            // NOTHING
        }

        ~SpinBox() {}

    private:
        SpinBox( const SpinBox& );
        SpinBox& operator=( const SpinBox& );

        virtual bool eventFilter( QObject*, QEvent* event )
        {
            if( event->type() == QEvent::KeyPress )
            {
                const int key = ((QKeyEvent*)event)->key();
                if( key == Qt::Key_Enter || key == Qt::Key_Return )
                    toolBar_.SlotSpeedChange();
            }
            return false;
        }

    private:
        SIMControlToolbar& toolBar_;
    };
}

//-----------------------------------------------------------------------------
// Name: SIMControlToolbar constructor
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
SIMControlToolbar::SIMControlToolbar( QMainWindow* pParent, Controllers& controllers, Network& network, Publisher_ABC& publisher )
    : QToolBar( pParent, "sim control toolbar" )
    , controllers_( controllers )
    , publisher_( publisher )
    , speed_( 4212 )
    , connected_( false )
    , paused_( false )
    , connectedPix_   ( MAKE_ICON( connected ) )
    , disconnectedPix_( MAKE_ICON( notconnected ) )
    , playPix_        ( MAKE_ICON( play ) )
    , stopPix_        ( MAKE_ICON( stop ) )
{
    setLabel( tr( "Simulation control" ) );

    pConnectButton_ = new QToolButton( this );
    pConnectButton_->setAccel( Key_C );
    pConnectButton_->setIconSet( MAKE_ICON( notconnected ) );
    pConnectButton_->setTextLabel( tr( "Connect (C)" ) );
    
    pPlayButton_ = new QToolButton( this );
    pPlayButton_->setAccel( Key_P );
    pPlayButton_->setIconSet( MAKE_ICON( stop ) );
    pPlayButton_->setTextLabel( tr( "Pause (P)" ) );
    pPlayButton_->setEnabled( false );

    new QLabel( " ", this );
    new QLabel( tr( "Speed factor:" ), this );

    pSpeedSpinBox_ = new SpinBox( 1, 100000, 1, this, *this );
    pSpeedSpinBox_->setButtonSymbols( QSpinBox::PlusMinus );
    pSpeedSpinBox_->setValue( 1 );
    pSpeedSpinBox_->setEnabled( false );

    pSpeedButton_ = new QToolButton( this );
    pSpeedButton_->setText( tr( "Ok" ) );
    pSpeedButton_->setEnabled( false );

    pConnectDlg_ = new ConnectDialog( this, network );
    pConnectDlg_->SetContextMenu( pConnectButton_ );
    pConnectDlg_->hide();

    pDisconnectDlg_ = new DisconnectDialog( this, network );
    pDisconnectDlg_->hide();

    connect( pConnectButton_, SIGNAL( clicked() ), this, SLOT( SlotConnectDisconnect() ) );
    connect( pPlayButton_,    SIGNAL( clicked() ), this, SLOT( SlotPlayPause() ) );
    connect( pSpeedButton_,   SIGNAL( clicked() ), this, SLOT( SlotSpeedChange() ) );
    connect( pSpeedSpinBox_ , SIGNAL( valueChanged( int ) ), this, SLOT( SlotOnSpinBoxChange( int ) ) );

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
        pConnectButton_->setIconSet( MAKE_ICON( connecting ) );
        pConnectButton_->setTextLabel( tr( "Connecting" ) );
        pConnectDlg_->exec();
        pConnectButton_->setIconSet( disconnectedPix_ );
        pConnectButton_->setTextLabel( tr( "Connect (C)" ) );
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
        ASN_MsgControlResume asnMsg;
        asnMsg.Send( publisher_ );
    }
    else
    {
        ASN_MsgControlPause asnMsg;
        asnMsg.Send( publisher_ );
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
        ASN_MsgControlChangeTimeFactor asnMsg;
        asnMsg() = pSpeedSpinBox_->value();
        asnMsg.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::SlotOnSpinBoxChange
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void SIMControlToolbar::SlotOnSpinBoxChange( int value )
{
    pSpeedButton_->setEnabled( value != speed_ );
}

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::NotifyUpdated
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void SIMControlToolbar::NotifyUpdated( const Simulation& simulation )
{
    if( simulation.IsConnected() != connected_ )
    {
        connected_ = simulation.IsConnected();
        if( connected_ )
        {
            pConnectButton_->setIconSet( connectedPix_ );
            pConnectButton_->setTextLabel( tr( "Disconnect (C)" ) );
            pConnectButton_->setPopup( 0 );
            pPlayButton_->setEnabled( true );
            if( !pSpeedSpinBox_->isEnabled() )
                pSpeedSpinBox_->setEnabled( true );
        }
        else
        {
            pConnectButton_->setIconSet( disconnectedPix_ );
            pConnectButton_->setTextLabel( tr( "Connect (C)" ) );
            pConnectDlg_->SetContextMenu( pConnectButton_ );
            pPlayButton_->setEnabled( false );
            pSpeedSpinBox_->setEnabled( false );
            pSpeedButton_->setEnabled( false );
        }
    }

    if( paused_ != simulation.IsPaused() )
    {
        paused_ = simulation.IsPaused();
        if( paused_ )
        {
            pPlayButton_->setIconSet( playPix_ );
            pPlayButton_->setTextLabel( tr( "Unpause (P)" ) );
        }
        else
        {
            pPlayButton_->setIconSet( stopPix_ );
            pPlayButton_->setTextLabel( tr( "Pause (P)" ) );
        }
    }

    if( speed_ != simulation.GetSpeed() )
    {
        speed_ = simulation.GetSpeed();
        pSpeedSpinBox_->setValue( speed_ );
        pSpeedButton_->setEnabled( false );
    }
}
