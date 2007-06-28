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
#include "clients_kernel/Profile_ABC.h"
#include "clients_gui/resources.h"
#include "gaming/statusicons.h"

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

    class AutoFitPopupMenu : public QPopupMenu
    {
    public:
        explicit AutoFitPopupMenu( QWidget* parent ) : QPopupMenu( parent ) {}
        virtual ~AutoFitPopupMenu() {}
        virtual void showEvent( QShowEvent* event )
        {
            QPopupMenu::showEvent( event );
            adjustSize();
        }
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
    , connectPix_   ( MAKE_ICON( notconnected ) )
    , disconnectPix_( MAKE_ICON( connected ) )
    , playPix_      ( MAKE_ICON( play ) )
    , stopPix_      ( MAKE_ICON( stop ) )
{
    setLabel( tr( "Simulation control" ) );

    pConnectButton_ = new QToolButton( this );
    pConnectButton_->setAccel( Key_C );
    pConnectButton_->setIconSet( disconnectPix_ );
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

    pCheckpointButton_ = new QToolButton( this );
    pCheckpointButton_->setIconSet( MAKE_ICON( checkpoint ) );
    pCheckpointButton_->setTextLabel( tr( "Save checkpoint" ) );
    pCheckpointButton_->setEnabled( false );
    pCheckpointButton_->setShown( false );

    {
        QPopupMenu* popup = new AutoFitPopupMenu( pCheckpointButton_ );
        pCheckpointButton_->setPopup( popup );
        pCheckpointButton_->setPopupDelay( 0 );
        QHBox* box = new QHBox( popup );
        box->setMargin( 5 );
        new QLabel( tr( "Checkpoint name: " ), box );
        QLineEdit* lineEdit = new QLineEdit( box );
        popup->insertItem( box );
        connect( lineEdit, SIGNAL( returnPressed() ), SLOT( SlotNamedCheckPoint() ) );
    }

    pConnectDlg_ = new ConnectDialog( this, network );
    pConnectDlg_->SetContextMenu( pConnectButton_ );
    pConnectDlg_->hide();

    pDisconnectDlg_ = new DisconnectDialog( this, network );
    pDisconnectDlg_->hide();

    connect( pConnectButton_, SIGNAL( clicked() ), SLOT( SlotConnectDisconnect() ) );
    connect( pPlayButton_,    SIGNAL( clicked() ), SLOT( SlotPlayPause() ) );
    connect( pSpeedButton_,   SIGNAL( clicked() ), SLOT( SlotSpeedChange() ) );
    connect( pSpeedSpinBox_ , SIGNAL( valueChanged( int ) ), SLOT( SlotOnSpinBoxChange( int ) ) );
    connect( pCheckpointButton_, SIGNAL( clicked() ), SLOT( SlotCheckpoint() ) );

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
        pConnectButton_->setIconSet( disconnectPix_ );
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
            pConnectButton_->setIconSet( connectPix_ );
            pConnectButton_->setTextLabel( tr( "Disconnect (C)" ) );
            pConnectButton_->setPopup( 0 );
            pPlayButton_->setEnabled( true );
            if( !pSpeedSpinBox_->isEnabled() )
                pSpeedSpinBox_->setEnabled( true );
            if( !pCheckpointButton_->isEnabled() )
                pCheckpointButton_->setEnabled( true );
        }
        else
        {
            pConnectButton_->setIconSet( disconnectPix_ );
            pConnectButton_->setTextLabel( tr( "Connect (C)" ) );
            pConnectDlg_->SetContextMenu( pConnectButton_ );
            pPlayButton_->setEnabled( false );
            pSpeedSpinBox_->setEnabled( false );
            pSpeedButton_->setEnabled( false );
            pCheckpointButton_->setEnabled( false );
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

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::NotifyUpdated
// Created: SBO 2007-06-18
// -----------------------------------------------------------------------------
void SIMControlToolbar::NotifyUpdated( const kernel::Profile_ABC& profile )
{
    pCheckpointButton_->setShown( profile.IsSupervision() );
}

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::SlotCheckpoint
// Created: SBO 2007-06-18
// -----------------------------------------------------------------------------
void SIMControlToolbar::SlotCheckpoint()
{
    RequestCheckpoint( "" );
}

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::SlotNamedCheckPoint
// Created: SBO 2007-06-18
// -----------------------------------------------------------------------------
void SIMControlToolbar::SlotNamedCheckPoint()
{
    const QObject* obj = sender();
    if( obj->isA( "QLineEdit" ) )
    {
        const QLineEdit* edit = static_cast< const QLineEdit* >( obj );
        RequestCheckpoint( edit->text().ascii() );
    }
}

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::RequestCheckpoint
// Created: SBO 2007-06-18
// -----------------------------------------------------------------------------
void SIMControlToolbar::RequestCheckpoint( const std::string& name )
{
    ASN_MsgControlCheckPointSaveNow asn;
    asn().m.namePresent = ! name.empty();
    if( asn().m.namePresent )
        asn().name = name.c_str();
    asn.Send( publisher_ );
    pCheckpointButton_->popup()->hide();
}
