// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ReplayerToolbar.h"
#include "moc_ReplayerToolbar.cpp"
#include "TimeTableRequestDialog.h"
#include "icons.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"
#include "protocol/ReplaySenders.h"
#include "protocol/ServerPublisher_ABC.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar constructor
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
ReplayerToolbar::ReplayerToolbar( QMainWindow* pParent, kernel::Controllers& controllers, Publisher_ABC& network )
    : QToolBar( pParent, "replay control toolbar" )
    , controllers_( controllers )
    , network_( network )
    , maxTick_( 0 )
    , slider_( 0 )
    , isPlayingBeforeMove_( false )
    , replayPaused_( true )
{
    setWindowTitle( tr( "Replay control" ) );
    QLabel* label = new QLabel( this );
    addWidget( label );
    label->setPixmap( MAKE_PIXMAP( replayer ) );
    setProperty( "notAppropriate", QVariant( true ) );
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar destructor
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
ReplayerToolbar::~ReplayerToolbar()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::NotifyUpdated
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void ReplayerToolbar::NotifyUpdated( const Simulation& simulation )
{
    if( simulation.GetTickCount() != static_cast< unsigned int >( -1 ) )
    {
        maxTick_ = simulation.GetTickCount();
        if( ! slider_ )
        {
            slider_ = new QSlider( Qt::Horizontal, this );
            addWidget( slider_ );
            const unsigned int firstTick = simulation.GetFirstTick();
            slider_->setMinValue( firstTick == std::numeric_limits< unsigned int >::max() ? maxTick_ : firstTick );
            slider_->setPageStep( 1 );
            slider_->setMinimumWidth( 200 );
            slider_->setTickmarks( QSlider::TicksBelow );
            addSeparator();
            value_ = new QLabel( this );
            addWidget( value_ );
            value_->setMargin( 5 );
            addSeparator();
            QToolButton* pTimeTableButton = new QToolButton( this );
            pTimeTableButton->setIconSet( MAKE_ICON( tic_temps ) );
            pTimeTableButton->setTextLabel( tr( "Time table" ) );
            addWidget( pTimeTableButton );
            addSeparator();
            QToolButton* pRefreshButton = new QToolButton( this );
            pRefreshButton->setIconSet( MAKE_ICON( refresh ) );
            pRefreshButton->setTextLabel( tr( "Refresh" ) );
            addWidget( pRefreshButton );
            connect( slider_, SIGNAL( sliderPressed() ), SLOT( OnSliderPressed() ) );
            connect( slider_, SIGNAL( sliderReleased() ), SLOT( OnSliderReleased() ) );
            connect( slider_, SIGNAL( valueChanged( int ) ), SLOT( OnSliderMoved( int ) ) );
            connect( pTimeTableButton, SIGNAL( clicked() ), SLOT( OnTimeTable() ) );
            connect( pRefreshButton, SIGNAL( clicked() ), SLOT( OnRefresh() ) );
        }
        replayPaused_ = simulation.IsPaused();
        slider_->setMaxValue( maxTick_ );
        slider_->setTickInterval( slider_->maxValue() / 20 );
        if( const unsigned int currentTick = simulation.GetCurrentTick() )
            slider_->setValue( currentTick );
        if( !isVisible() )
        {
            setVisible( true );
            setProperty( "notAppropriate", QVariant() );
        }
    }
    else if( isVisible() )
        setVisible( false );
}

namespace
{
    class TimeTableDialog : public QDialog
    {
    public:
        TimeTableDialog( QWidget* parent, const sword::TimeTable& timeTable )
            : QDialog( parent, 0, true )
        {
            setCaption( tools::translate( "TimeTableDialog", "Time Table" ) );

            QLayout* layout = new QVBoxLayout( this );
            layout->setMargin( 5 );
            setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );

            QTableWidget* table = new QTableWidget();
            layout->addWidget( table );
            table->setRowCount( timeTable.time_table_item_size() );
            table->setColumnCount( 3 );
            table->verticalHeader()->hide();
            QStringList headers;
            headers << tools::translate( "TimeTableDialog", "Tick" )
                    << tools::translate( "TimeTableDialog", "Sim time" )
                    << tools::translate( "TimeTableDialog", "Real time" );
            table->setHorizontalHeaderLabels( headers );
            table->horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
            table->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
            table->horizontalHeader()->setResizeMode( 2, QHeaderView::Stretch );
            for( int i = 0; i < timeTable.time_table_item_size(); ++i )
            {
                const sword::TimeTable_TimeMapping& map = timeTable.time_table_item( i );
                table->setItem( i, 0, new QTableWidgetItem( locale().toString( map.tick() ) ) );
                table->setItem( i, 1, new QTableWidgetItem( map.simulation_time().data().c_str() ) );
                table->setItem( i, 2, new QTableWidgetItem( map.real_time().data().c_str() ) );
                table->item( i, 0 )->setFlags( Qt::ItemIsEnabled );
                table->item( i, 1 )->setFlags( Qt::ItemIsEnabled );
                table->item( i, 2 )->setFlags( Qt::ItemIsEnabled );
            }
            QPushButton* okButton = new QPushButton(  tools::translate( "TimeTableDialog", "Ok" ) );
            layout->addWidget( okButton );
            connect( okButton, SIGNAL( clicked() ), SLOT( accept() ) );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::NotifyUpdated
// Created: JSR 2011-07-26
// -----------------------------------------------------------------------------
void ReplayerToolbar::NotifyUpdated( const Simulation::sTimeTable& timeTable )
{
    TimeTableDialog( this, timeTable.timeTable_ ).exec();
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnSliderMoved
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnSliderMoved( int frame )
{
    value_->setText( tr( "Tick %L1" ).arg( frame ) );
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnSliderPressed
// Created: JSR 2012-11-28
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnSliderPressed()
{
    isPlayingBeforeMove_ = !replayPaused_;
    if( isPlayingBeforeMove_ )
    {
        replay::ControlPause message;
        message.Send( network_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnSliderReleased
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnSliderReleased()
{
    replay::ControlSkipToTick skip;
    skip().set_tick( slider_->value() - 1 );
    skip.Send( network_ );
    if( isPlayingBeforeMove_ )
    {
        replay::ControlResume message;
        message().set_tick( 1 );
        message.Send( network_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnTimeTable
// Created: JSR 2011-07-26
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnTimeTable()
{
    TimeTableRequestDialog( this, network_, maxTick_ ).exec();
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnRefresh
// Created: MMC 2012-09-14
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnRefresh()
{
    replay::ForceRefreshDataRequest reload;
    reload.Send( network_ );
}
