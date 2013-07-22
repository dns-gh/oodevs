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
#include "gaming/SimulationController.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_gui/resources.h"
#include "clients_gui/RichSpinBox.h"
#include "gaming/statusicons.h"

using namespace sword;

namespace
{
    class SpinBox : public gui::RichSpinBox
    {
    public:
        SpinBox( const QString& objectName, int minValue, int maxValue, int step, QWidget* parent, SIMControlToolbar& toolbar )
            : gui::RichSpinBox( objectName, parent, minValue, maxValue, step )
            , toolBar_( toolbar )
        {
            // NOTHING
        }

        ~SpinBox()
        {
            // NOTHING
        }

    private:

        virtual bool event( QEvent* event )
        {
            if( event->type() == QEvent::KeyPress )
            {
                const int key = static_cast< QKeyEvent* >( event )->key();
                emit valueChanged( value() );
                if( key == Qt::Key_Enter || key == Qt::Key_Return )
                    toolBar_.SlotSpeedChange();
            }
            return QSpinBox::event( event );
        }

    private:
        SIMControlToolbar& toolBar_;
    };

    class AutoFitPopupMenu : public kernel::ContextMenu
    {
    public:
        explicit AutoFitPopupMenu( QWidget* parent )
            : kernel::ContextMenu( parent )
        {
            // NOTHING
        }
        virtual ~AutoFitPopupMenu()
        {
            // NOTHING
        }
        virtual void showEvent( QShowEvent* event )
        {
            kernel::ContextMenu::showEvent( event );
            adjustSize();
        }
    };

    class LineEdit : public QWidgetAction
    {
    public:
        LineEdit( QWidget* toolbar, QObject* parent )
            : QWidgetAction( parent )
            , toolbar_( toolbar )
        {
            // NOTHING
        }
        virtual ~LineEdit()
        {
            // NOTHING
        }
    private:
        virtual QWidget* createWidget( QWidget* parent )
        {
            Q3HBox* box = new Q3HBox( parent );
            box->setMargin( 5 );
            new QLabel( tr( "Checkpoint name: " ), box );
            QLineEdit* lineEdit = new QLineEdit( box );
            connect( lineEdit, SIGNAL( returnPressed() ), toolbar_, SLOT( SlotNamedCheckPoint() ) );
            return box;
        }

    private:
        QWidget* toolbar_;
    };
}

//-----------------------------------------------------------------------------
// Name: SIMControlToolbar constructor
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
SIMControlToolbar::SIMControlToolbar( QMainWindow* pParent, kernel::Controllers& controllers, SimulationController& simulationController, Network& network, kernel::Logger_ABC& logger )
    : gui::RichToolBar( controllers, pParent, "sim control toolbar" )
    , controllers_( controllers )
    , simulationController_( simulationController )
    , speed_( 4212 )
    , connected_( false )
    , paused_( false )
    , connectPix_   ( MAKE_ICON( notconnected ) )
    , disconnectPix_( MAKE_ICON( connected ) )
    , playPix_      ( MAKE_ICON( play ) )
    , stopPix_      ( MAKE_ICON( stop ) )
{
    setObjectName( "simControl" );
    setWindowTitle( tr( "Simulation control" ) );
    pConnectButton_ = new QToolButton( this );
    pConnectButton_->setIconSet( disconnectPix_ );
    pConnectButton_->setTextLabel( tr( "Connect (Alt+C)" ) );
    pConnectButton_->setShortcut( QKeySequence( Qt::ALT + Qt::Key_C ) );

    pPlayButton_ = new QToolButton( this );
    pPlayButton_->setIconSet( MAKE_ICON( stop ) );
    pPlayButton_->setTextLabel( tr( "Pause (Alt+P)" ) );
    pPlayButton_->setShortcut( QKeySequence( Qt::ALT + Qt::Key_P ) );
    pPlayButton_->setEnabled( false );

    pStepButton_ = new QToolButton( this );
    pStepButton_->setAccel( Qt::Key_S );
    pStepButton_->setIconSet( MAKE_ICON( step ) );
    pStepButton_->setTextLabel( tr( "Step (Alt+S)" ) );
    pStepButton_->setEnabled( false );
    pStepButton_->setShortcut( QKeySequence( Qt::ALT + Qt::Key_S ) );

    pSpeedSpinBox_ = new SpinBox( "pSpeedSpinBox", 1, 100000, 1, this, *this );
    pSpeedSpinBox_->setButtonSymbols( QSpinBox::PlusMinus );
    pSpeedSpinBox_->setValue( 1 );
    pSpeedSpinBox_->setEnabled( false );
    pSpeedSpinBox_->setSuffix( "x" );

    pSpeedButton_ = new QToolButton( this );
    pSpeedButton_->setText( tr( "Ok" ) );
    pSpeedButton_->setEnabled( false );

    pCheckpointButton_ = new QToolButton( this );
    pCheckpointButton_->setIconSet( MAKE_ICON( checkpoint ) );
    pCheckpointButton_->setTextLabel( tr( "Save checkpoint" ) );
    pCheckpointButton_->setEnabled( false );
    {
        kernel::ContextMenu* popup = new AutoFitPopupMenu( pCheckpointButton_ );
        pCheckpointButton_->setPopup( popup );
        pCheckpointButton_->setPopupMode( QToolButton::MenuButtonPopup );
        popup->addAction( new LineEdit( this, popup ) );
    }

    pConnectDlg_ = new ConnectDialog( pParent, network, logger );
    {
        kernel::ContextMenu* popup = new kernel::ContextMenu( pConnectButton_ );
        pConnectDlg_->FillPopupMenu( popup );
        pConnectButton_->setPopup( popup );
        pConnectButton_->setPopupMode( QToolButton::MenuButtonPopup );
        pConnectButton_->setPopupDelay( 0 );
    }
    pConnectDlg_->hide();

    pDisconnectDlg_ = new DisconnectDialog( this, network );
    pDisconnectDlg_->hide();

    addWidget( pConnectButton_ );
    addWidget( pPlayButton_ );
    addWidget( pStepButton_ );
    addWidget( new QLabel( " ", this ) );
    addWidget( new QLabel( tr( "Speed factor: " ), this ) );
    addWidget( pSpeedSpinBox_ );
    addWidget( pSpeedButton_ );
    pCheckpointAction_ = addWidget( pCheckpointButton_ );
    pCheckpointAction_->setVisible( false );

    connect( pConnectButton_, SIGNAL( clicked() ), SLOT( SlotConnectDisconnect() ) );
    connect( pPlayButton_,    SIGNAL( clicked() ), SLOT( SlotPlayPause() ) );
    connect( pPlayButton_,    SIGNAL( clicked() ), pParent, SLOT( PlayPauseSoundControl() ) );
    connect( pStepButton_,    SIGNAL( clicked() ), SLOT( SlotStep() ) );
    connect( pSpeedButton_,   SIGNAL( clicked() ), SLOT( SlotSpeedChange() ) );
    connect( pSpeedSpinBox_ , SIGNAL( valueChanged( int ) ), SLOT( SlotOnSpinBoxChange( int ) ) );
    connect( pCheckpointButton_, SIGNAL( clicked() ), SLOT( SlotCheckpoint() ) );

    controllers_.Update( *this );
}

//-----------------------------------------------------------------------------
// Name: SIMControlToolbar destructor
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
SIMControlToolbar::~SIMControlToolbar()
{
    controllers_.Unregister( *this );
}

//-----------------------------------------------------------------------------
// Name: SIMControlToolbar::SlotConnectDisconnect
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
void SIMControlToolbar::SlotConnectDisconnect()
{
    if( connected_ )
        pDisconnectDlg_->show();
    else
    {
        pConnectButton_->setIconSet( MAKE_ICON( connecting ) );
        pConnectButton_->setTextLabel( tr( "Connecting" ) );
        pConnectDlg_->exec();
        pConnectButton_->setIconSet( disconnectPix_ );
        pConnectButton_->setTextLabel( tr( "Connect (Alt+C)" ) );
        pConnectButton_->setShortcut( QKeySequence( Qt::ALT + Qt::Key_C ) ); // bug QT setText() function overwrites the already defined shortcut
    }
}

//-----------------------------------------------------------------------------
// Name: SIMControlToolbar::SlotPlayPause
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
void SIMControlToolbar::SlotPlayPause()
{
    if( simulationController_.IsPaused() )
        simulationController_.Resume();
    else
        simulationController_.Pause();
}

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::SlotStep
// Created: SBO 2011-05-11
// -----------------------------------------------------------------------------
void SIMControlToolbar::SlotStep()
{
    simulationController_.Step();
}

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::SlotSpeedChange
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void SIMControlToolbar::SlotSpeedChange()
{
    if( connected_ )
        simulationController_.ChangeTimeFactor( pSpeedSpinBox_->value() );
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
            pConnectButton_->setPopupMode( QToolButton::InstantPopup );
            pConnectButton_->setIconSet( connectPix_ );
            pConnectButton_->setTextLabel( tr( "Disconnect (Alt+C)" ) );
            pConnectButton_->setPopup( 0 );
            pPlayButton_->setEnabled( true );
            pStepButton_->setEnabled( true );
            if( !pSpeedSpinBox_->isEnabled() )
                pSpeedSpinBox_->setEnabled( true );
            if( !pCheckpointButton_->isEnabled() )
                pCheckpointButton_->setEnabled( true );
        }
        else
        {
            pConnectButton_->setPopupMode( QToolButton::MenuButtonPopup );
            pConnectButton_->setIconSet( disconnectPix_ );
            pConnectButton_->setTextLabel( tr( "Connect (Alt+C)" ) );
            pConnectButton_->setFocus();
            {
                kernel::ContextMenu* popup = new kernel::ContextMenu( pConnectButton_ );
                pConnectDlg_->FillPopupMenu( popup );
                pConnectButton_->setPopup( popup );
                pConnectButton_->setPopupDelay( 0 );
            }
            pPlayButton_->setEnabled( false );
            pStepButton_->setEnabled( false );
            pSpeedSpinBox_->setEnabled( false );
            pSpeedButton_->setEnabled( false );
            pCheckpointButton_->setEnabled( false );
        }
        pConnectButton_->setShortcut( QKeySequence( Qt::ALT + Qt::Key_C ) ); // Bug Qt : setText() function overwrites the already defined shortcut
    }

    if( paused_ != simulation.IsPaused() )
    {
        paused_ = simulation.IsPaused();
        if( paused_ )
        {
            pPlayButton_->setIconSet( playPix_ );
            pPlayButton_->setTextLabel( tr( "Unpause (Alt+P)" ) );
        }
        else
        {
            pPlayButton_->setIconSet( stopPix_ );
            pPlayButton_->setTextLabel( tr( "Pause (Alt+P)" ) );
        }
        pPlayButton_->setShortcut( QKeySequence( Qt::ALT + Qt::Key_P ) );
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
    const bool super = profile.IsSupervision();
    pPlayButton_->setEnabled( super );
    pSpeedSpinBox_->setEnabled( super );
    pSpeedButton_->setEnabled( super );
    pCheckpointButton_->setEnabled( super );
    pCheckpointAction_->setVisible( super && controllers_.GetCurrentMode() != eModes_Replay );
}

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::NotifyModeChanged
// Created: NPT 2013-02-28
// -----------------------------------------------------------------------------
void SIMControlToolbar::NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode )
{
    RichToolBar::NotifyModeChanged( newMode, useDefault, firstChangeToSavedMode );
    pCheckpointButton_->setVisible( newMode != eModes_Replay );
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
        RequestCheckpoint( edit->text().toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: SIMControlToolbar::RequestCheckpoint
// Created: SBO 2007-06-18
// -----------------------------------------------------------------------------
void SIMControlToolbar::RequestCheckpoint( const std::string& name )
{
    simulationController_.RequestCheckpoint( name );
    pCheckpointButton_->popup()->hide();
}
