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
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "gaming/SimulationController.h"
#include "protocol/ReplaySenders.h"

using namespace sword;

namespace
{
    template< class Widget >
    class EditWidget : public Widget
    {
    public:
        EditWidget() {}
        virtual ~EditWidget() {}
        virtual void stepBy( int steps )
        {
            Widget::stepBy( steps );
            emit editingFinished();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar constructor
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
ReplayerToolbar::ReplayerToolbar( QMainWindow* pParent, kernel::Controllers& controllers, SimulationController& simulationController, Publisher_ABC& network )
    : gui::RichToolBar( controllers, pParent, "replay control toolbar" )
    , controllers_( controllers )
    , simulationController_( simulationController )
    , network_( network )
    , slider_( 0 )
    , lastTickSkip_( 0 )
{
    setWindowTitle( tr( "Replay control" ) );
    QLabel* label = new QLabel( this );
    addWidget( label );
    label->setPixmap( MAKE_PIXMAP( replayer ) );
    controllers_.Update( *this );
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
    if( controllers_.GetCurrentMode() == eModes_Replay )
    {
        const unsigned int maxTick = simulation.GetTickCount();
        if( ! slider_ )
        {
            minTime_ = new QLabel( this );
            addWidget( minTime_ );
            slider_ = new QSlider( Qt::Horizontal, this );
            addWidget( slider_ );
            const unsigned int firstTick = simulation.GetFirstTick();
            slider_->setMinValue( firstTick == std::numeric_limits< unsigned int >::max() ? maxTick : firstTick );
            slider_->setPageStep( 1 );
            slider_->setMinimumWidth( 200 );
            slider_->setTickmarks( QSlider::TicksBelow );
            maxTime_ = new QLabel( this );
            addWidget( maxTime_ );
            addSeparator();
            button_ = new QPushButton();
            menu_ = new kernel::ContextMenu( button_ );
            button_->setPopup( menu_ );
            button_->setText( tr( "Ticks" ) );
            menu_->insertItem( tr( "Ticks" ), 0 );
            menu_->insertItem( tr( "Time" ), 1 );
            menu_->setItemChecked( 0, true );
            addWidget( button_ );
            QSpinBox* spinBox = new EditWidget< QSpinBox >();
            spinBoxAction_ = addWidget( spinBox );
            spinBoxAction_->setVisible( true );
            QDateTimeEdit* dateTime = new EditWidget< QDateTimeEdit >();
            dateTime->setDisplayFormat( "dd/MM/yy HH:mm:ss" );
            dateTime->setButtonSymbols( QAbstractSpinBox::NoButtons );
            dateTimeAction_ = addWidget( dateTime );
            dateTimeAction_->setVisible( false );
            addSeparator();
            QToolButton* pTimeTableButton = new QToolButton();
            pTimeTableButton->setIconSet( MAKE_ICON( tic_temps ) );
            pTimeTableButton->setTextLabel( tr( "Time table" ) );
            addWidget( pTimeTableButton );
            addSeparator();
            QToolButton* pRefreshButton = new QToolButton();
            pRefreshButton->setIconSet( MAKE_ICON( refresh ) );
            pRefreshButton->setTextLabel( tr( "Refresh" ) );
            addWidget( pRefreshButton );
            connect( slider_, SIGNAL( sliderPressed() ), SLOT( OnSliderPressed() ) );
            connect( slider_, SIGNAL( sliderReleased() ), SLOT( OnSliderReleased() ) );
            connect( slider_, SIGNAL( valueChanged( int ) ), SLOT( OnSliderMoved( int ) ) );
            connect( spinBox, SIGNAL( editingFinished() ), SLOT( OnSpinBoxChanged() ) );
            connect( dateTime, SIGNAL( editingFinished() ), SLOT( OnDateTimeChanged() ) );
            connect( pTimeTableButton, SIGNAL( clicked() ), SLOT( OnTimeTable() ) );
            connect( pRefreshButton, SIGNAL( clicked() ), SLOT( OnRefresh() ) );
            connect( menu_, SIGNAL( activated( int ) ), SLOT( OnMenuActivated( int ) ) );
        }
        const auto SetDateTime = [&]( QLabel& dst, int tick ){
            dst.setText( simulation.GetTime( tick ).toString( "dd/MM/yy\nHH:mm:ss" ) );
        };
        const auto first = simulation.GetFirstTick();
        SetDateTime( *minTime_, first );
        SetDateTime( *maxTime_, maxTick );
        SpinBox()->setRange( first, maxTick );
        SpinBox()->setSuffix( QString( " / %1" ).arg( maxTick ) );
        slider_->setRange( first, maxTick );
        slider_->setTickInterval( slider_->maxValue() / 20 );
        slider_->blockSignals( true );
        slider_->setValue( simulation.GetCurrentTick() );
        slider_->blockSignals( false );
        SpinBox()->setValue( simulation.GetCurrentTick() );
        DateTimeEdit()->setDateTime( simulation.GetDateTime() );
        SpinBox()->setEnabled( simulation.IsPaused() );
        DateTimeEdit()->setEnabled( simulation.IsPaused() );
    }
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
                QDateTime date = tools::IsoStringToQDateTime( map.real_time().data().c_str() );
                date.setTimeSpec( Qt::UTC );
                table->setItem( i, 2, new QTableWidgetItem( tools::QDateTimeToGDHString( date.toLocalTime() ).c_str() ) );
                table->item( i, 0 )->setFlags( Qt::ItemIsEnabled );
                table->item( i, 1 )->setFlags( Qt::ItemIsEnabled );
                table->item( i, 2 )->setFlags( Qt::ItemIsEnabled );
            }
            if( timeTable.has_partial() && timeTable.partial() )
            {
                QString format( "<font color=\"red\">%1</font>" );
                QLabel* label = new QLabel( format.arg( tools::translate( "TimeTableDialog", "Requested time range is too large, only a subset of it is displayed." ) ) );
                label->setWordWrap( true );
                layout->addWidget( label );
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
// Created: JSR 2013-03-11
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnSliderMoved( int value )
{
    SpinBox()->setValue( value );
    DateTimeEdit()->setDateTime( simulationController_.GetTime( value ) );
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnSpinBoxChanged
// Created: JSR 2013-03-11
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnSpinBoxChanged()
{
    SkipToTick( SpinBox()->value() );
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnDateTimeChanged
// Created: JSR 2013-03-12
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnDateTimeChanged()
{
    simulationController_.SkipToDate( DateTimeEdit()->dateTime() );
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnSliderPressed
// Created: JSR 2012-11-28
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnSliderPressed()
{
    if( !simulationController_.IsPaused() )
        simulationController_.Pause();
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnSliderReleased
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnSliderReleased()
{
    SkipToTick( slider_->value() );
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnMenuActivated
// Created: JSR 2013-03-13
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnMenuActivated( int index )
{
    bool tickMode = index == 0;
    spinBoxAction_->setVisible( tickMode );
    dateTimeAction_->setVisible( !tickMode );
    button_->setText( tickMode ? tr( "Ticks" ) : tr( "Time" ) );
    menu_->setItemChecked( 0, tickMode );
    menu_->setItemChecked( 1, !tickMode );
    SpinBox()->setEnabled( simulationController_.IsPaused() );
    DateTimeEdit()->setEnabled( simulationController_.IsPaused() );
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnTimeTable
// Created: JSR 2011-07-26
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnTimeTable()
{
    TimeTableRequestDialog( this, network_, simulationController_.GetTickCount() ).exec();
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnRefresh
// Created: MMC 2012-09-14
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnRefresh()
{
    simulationController_.ForceReplayDataRequest();
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::SkipToTick
// Created: JSR 2013-03-12
// -----------------------------------------------------------------------------
void ReplayerToolbar::SkipToTick( unsigned int tick )
{
    if( lastTickSkip_ == tick )
        return;
    simulationController_.SkipToTick( tick - 1 );
    lastTickSkip_ = tick;
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::SpinBox
// Created: JSR 2013-03-12
// -----------------------------------------------------------------------------
QSpinBox* ReplayerToolbar::SpinBox()
{
    return static_cast< QSpinBox* >( widgetForAction( spinBoxAction_ ) );
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::DateTimeEdit
// Created: JSR 2013-03-12
// -----------------------------------------------------------------------------
QDateTimeEdit* ReplayerToolbar::DateTimeEdit()
{
    return static_cast< QDateTimeEdit* >( widgetForAction( dateTimeAction_ ) );
}
