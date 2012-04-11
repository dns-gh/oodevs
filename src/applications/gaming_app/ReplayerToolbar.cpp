// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ReplayerToolbar.h"
#include "moc_ReplayerToolbar.cpp"
#include "TimeTableRequestDialog.h"
#include "icons.h"
#include "clients_gui/Tools.h"
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
    , network_    ( network )
    , maxTick_    ( 0 )
    , slider_     ( 0 )
    , userMove_   ( true )
{
    setWindowTitle( tr( "Replay control" ) );
    QLabel* label = new QLabel( this );
    addWidget( label );
    label->setPixmap( MAKE_PIXMAP( replayer ) );
    setProperty( "notAppropriate", QVariant( true ) );
    controllers_.Register( *this );
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
            slider_->setMinValue( 0 );
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
            connect( slider_, SIGNAL( sliderReleased() ), SLOT( OnSliderReleased() ) );
            connect( slider_, SIGNAL( valueChanged( int ) ), SLOT( OnSliderMoved( int ) ) );
            connect( pTimeTableButton, SIGNAL( clicked() ), SLOT( OnTimeTable() ) );
        }
        userMove_ = false;
        slider_->setMaxValue( maxTick_ - 1 );
        slider_->setTickInterval( slider_->maxValue() / 20 );
        slider_->setValue( simulation.GetCurrentTick() );
        OnSliderMoved( simulation.GetCurrentTick() );
        userMove_ = true;
        if( ! isVisible() )
        {
            show();
            setProperty( "notAppropriate", QVariant() );
        }
    }
    else if( isVisible() )
        hide();
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
            Q3VBox* box = new Q3VBox( this );
            layout->add( box );

            Q3Table* table = new Q3Table( timeTable.time_table_item_size(), 3, box );
            table->verticalHeader()->hide();
            table->setLeftMargin( 0 );
            table->horizontalHeader()->setLabel( 0, tools::translate( "TimeTableDialog", "Tick" ) );
            table->horizontalHeader()->setLabel( 1, tools::translate( "TimeTableDialog", "Sim time" ) );
            table->horizontalHeader()->setLabel( 2, tools::translate( "TimeTableDialog", "Real time" ) );
            for( int i = 0; i < timeTable.time_table_item_size(); ++i )
            {
                const sword::TimeTable_TimeMapping& map = timeTable.time_table_item( i );
                table->setItem( i, 0, new Q3TableItem( table, Q3TableItem::Never, locale().toString( map.tick() ) ) );
                table->setItem( i, 1, new Q3TableItem( table, Q3TableItem::Never, map.simulation_time().data().c_str() ) );
                table->setItem( i, 2, new Q3TableItem( table, Q3TableItem::Never, map.real_time().data().c_str() ) );
            }
            connect( new QPushButton(  tools::translate( "TimeTableDialog", "Ok" ), box ), SIGNAL( clicked() ), SLOT( accept() ) );
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
    value_->setText( tr( "Tick %L1" ).arg( frame + 1 ) );
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnSliderReleased
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnSliderReleased()
{
    if( userMove_ )
    {
        replay::ControlSkipToTick skip;
        skip().set_tick( slider_->value() );
        skip.Send( network_ );
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
