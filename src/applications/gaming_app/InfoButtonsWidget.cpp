// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoButtonsWidget.h"
#include "moc_InfoButtonsWidget.cpp"
#include "icons.h"
#include "InfoCompositionDialog.h"
#include "InfoMaintenanceDialog.h"
#include "InfoMedicalDialog.h"
#include "InfoSupplyDialog.h"
#include "InfoFuneralDialog.h"
#include "UnitStateDialog.h"
#include "LogisticDialog_ABC.h"
#include "LogButton.h"
#include "gaming/LogMaintenanceConsign.h"
#include "gaming/LogMedicalConsign.h"
#include "gaming/LogSupplyConsign.h"
#include "gaming/LogFuneralConsign.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/Model.h"
#include "gaming/Simulation.h"
#include "gaming/SimulationController.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/HistoryLogisticsModel.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_gui/ImageWrapper.h"
#include "tools/GeneralConfig.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/Protocol.h"

namespace
{
    QPixmap MakePixmap( const tools::Path& name )
    {
        return gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( tools::Path( "images/gaming" ) / name + ".png" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget constructor
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
InfoButtonsWidget::InfoButtonsWidget( QWidget* widget,
                                      kernel::Controllers& controllers,
                                      const tools::ExerciseConfig& config,
                                      gui::ItemFactory_ABC& factory,
                                      gui::DisplayExtractor& extractor,
                                      Model& model,
                                      const Simulation& simulation,
                                      const kernel::Profile_ABC& profile,
                                      SimulationController& simulationController )
    : Q3GroupBox( 2, Qt::Horizontal, widget, "InfoButtonsWidget" )
    , controllers_         ( controllers )
    , simulation_          ( simulation )
    , element_             ( 0 )
    , hasChanged_          ( false )
    , timer_               ( 0 )
    , lastTick_            ( 0 )
    , historyModel_        ( model.historyLogistics_ )
    , simulationController_( simulationController )
{
    setFlat( true );
    setFixedWidth( 100 );
    setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );
    layout()->setMargin( 0 );
    setInsideMargin( 0 );
    setInsideSpacing( 0 );

    timer_ = new QTimer( this );
    timer_->start( 1000 );

    connect( timer_, SIGNAL( timeout() ), SLOT( OnUpdate() ) );

    UnitStateDialog* unitStateDialog = new UnitStateDialog( topLevelWidget(), controllers, config, model.static_, model.actions_, simulation, profile, extractor );
    AddButton< InfoCompositionDialog >( MakePixmap( "composition" ), controllers, factory );
    AddButton( unitStateDialog, MakePixmap( "ordnance" ), unitStateDialog->GetResourceToolTip(), SLOT( ToggleResource( bool ) ), SIGNAL( OnToggleResource( bool ) ) );

    AddLogisticButton< InfoMedicalDialog >    ( MakePixmap( "health"      ), controllers, extractor, profile, model );
    AddLogisticButton< InfoMaintenanceDialog >( MakePixmap( "maintenance" ), controllers, extractor, profile, model );
    AddLogisticButton< InfoSupplyDialog >     ( MakePixmap( "supply"      ), controllers, factory, extractor, profile, model );
    AddLogisticButton< InfoFuneralDialog >    ( MakePixmap( "mortuary"    ), controllers, extractor, profile, model );

    simulationController_.RegisterSimHandler( [&]( const sword::SimToClient& message ) {
        if( message.message().has_list_logistic_requests_ack() )
            FillRequests( message.message().list_logistic_requests_ack() ); }
    );
    simulationController_.RegisterReplayHandler( [&]( const sword::ReplayToClient& message ) {
        if( message.message().has_list_logistic_requests_ack() )
            FillRequests( message.message().list_logistic_requests_ack() );
        hasChanged_ = hasChanged_ || message.message().has_control_resume_ack() ||
            message.message().has_control_skip_to_tick_ack(); }
    );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget destructor
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
InfoButtonsWidget::~InfoButtonsWidget()
{
    controllers_.Unregister( *this );
}

namespace
{
    QPushButton* CreateButton( QWidget* parent, const QPixmap& pixmap )
    {
        QPushButton* btn = new QPushButton( parent );
        btn->setToggleButton( true );
        btn->setPixmap( pixmap );
        btn->setFixedSize( 50, 50 );
        btn->setDisabled( true );
        return btn;
    }
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::AddButton
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
template< typename Dialog >
void InfoButtonsWidget::AddButton( const QPixmap& pixmap, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
{
    QPushButton* btn = CreateButton( this, pixmap );
    QDialog* dialog = new Dialog( topLevelWidget(), controllers, factory );
    QToolTip::add( btn, dialog->caption() );
    connect( btn, SIGNAL( toggled( bool ) ), dialog, SLOT( OnToggle( bool ) ) );
    connect( dialog, SIGNAL( Closed() ), btn, SLOT( toggle() ) );
    connect( dialog, SIGNAL( Disabled( bool ) ), btn, SLOT( setDisabled( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::AddLogisticButton
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
template< typename Dialog >
void InfoButtonsWidget::AddLogisticButton( const QPixmap& pixmap, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory
    , gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile, Model& model )
{
    LogButton* btn = new LogButton( this, controllers, pixmap );
    Dialog* dialog = new Dialog( topLevelWidget(), controllers, factory, extractor, profile, simulationController_, model );
    QToolTip::add( btn, dialog->caption() );
    connect( btn, SIGNAL( toggled( bool ) ), dialog, SLOT( OnToggle( bool ) ) );
    connect( dialog, SIGNAL( Closed() ), btn, SLOT( toggle() ) );
    connect( dialog, SIGNAL( Disabled( bool ) ), btn, SLOT( ChangePixmap( bool ) ) );
    connect( dialog, SIGNAL( Shown() ), SLOT( Refresh() ) );
    logisticDialogs_.push_back( dialog );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::AddLogisticButton
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
template< typename Dialog >
void InfoButtonsWidget::AddLogisticButton( const QPixmap& pixmap, kernel::Controllers& controllers
                                 , gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile, Model& model )
{
    LogButton* btn = new LogButton( this, controllers, pixmap );
    Dialog* dialog = new Dialog( topLevelWidget(), controllers, extractor, profile, simulationController_, model );
    QToolTip::add( btn, dialog->caption() );
    connect( btn, SIGNAL( toggled( bool ) ), dialog, SLOT( OnToggle( bool ) ) );
    connect( dialog, SIGNAL( Closed() ), btn, SLOT( toggle() ) );
    connect( dialog, SIGNAL( Disabled( bool ) ), btn, SLOT( ChangePixmap( bool ) ) );
    connect( dialog, SIGNAL( Shown() ), SLOT( Refresh() ) );
    logisticDialogs_.push_back( dialog );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::AddButton
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void InfoButtonsWidget::AddButton( QDialog* dialog, const QPixmap& pixmap, const QString& tooltips, const char* toggleSlot, const char* toggleSignal )
{
    QPushButton* btn = CreateButton( this, pixmap );
    QToolTip::add( btn, tooltips );
    connect( btn, SIGNAL( toggled( bool ) ), dialog, toggleSlot );
    connect( dialog, toggleSignal, btn, SLOT( setOn( bool ) ) );
    connect( dialog, SIGNAL( Disabled( bool ) ), btn, SLOT( setDisabled( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::NotifyDeleted
// Created: LDC 2014-04-30
// -----------------------------------------------------------------------------
void InfoButtonsWidget::NotifyDeleted( const kernel::Entity_ABC& element )
{
    if( element_ == &element )
        NotifySelected( 0 );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::NotifySelected
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void InfoButtonsWidget::NotifySelected( const kernel::Entity_ABC* element )
{
    element_ = element;
    entities_.clear();
    if( element_ )
    {
        bool needRequests = false;
        for( auto it = logisticDialogs_.begin(); it != logisticDialogs_.end(); ++it )
            if( (*it)->IsVisible() )
            {
                (*it)->FillCurrentModel( *element_ );
                needRequests = true;
            }
        logistic_helpers::VisitEntityAndSubordinatesUpToBaseLog( *element, [&]( const kernel::Entity_ABC& entity ) {
            entities_.insert( entity.GetId() );
        } );

        if( !entities_.empty() && needRequests )
            simulationController_.SendLogisticRequests( entities_ );
    }
    else
        for( auto it = logisticDialogs_.begin(); it != logisticDialogs_.end(); ++it )
            (*it)->Purge();
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::FillRequests
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void InfoButtonsWidget::FillRequests( const sword::ListLogisticRequestsAck& /*message*/ )
{
    if( element_ )
    {
        for( auto it = logisticDialogs_.begin(); it != logisticDialogs_.end(); ++it )
            if( (*it)->IsVisible() )
                (*it)->FillHistoryModel();
    }
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::Update
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
template< typename Extension >
void InfoButtonsWidget::Update( const Extension& consigns )
{
    if( element_ && logistic_helpers::HasRetrieveEntityAndSubordinatesUpToBaseLog( *element_, &consigns ) )
        hasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::NotifyUpdated
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void InfoButtonsWidget::NotifyUpdated( const LogMedicalConsigns& consigns )
{
    Update( consigns );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::NotifyUpdated
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void InfoButtonsWidget::NotifyUpdated( const LogFuneralConsigns& consigns )
{
    Update( consigns );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::NotifyUpdated
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void InfoButtonsWidget::NotifyUpdated( const LogSupplyConsigns& consigns )
{
    Update( consigns );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::NotifyUpdated
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void InfoButtonsWidget::NotifyUpdated( const LogMaintenanceConsigns& consigns )
{
    Update( consigns );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::UpdateConsign
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void InfoButtonsWidget::UpdateConsign( const LogisticsConsign_ABC& consign )
{
    if( !hasChanged_ && element_ && consign.RefersToAgent( entities_ ) )
        hasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::NotifyUpdated
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void InfoButtonsWidget::NotifyUpdated( const LogMedicalConsign& consign )
{
    UpdateConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::NotifyUpdated
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void InfoButtonsWidget::NotifyUpdated( const LogMaintenanceConsign& consign )
{
    UpdateConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::NotifyUpdated
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void InfoButtonsWidget::NotifyUpdated( const LogSupplyConsign& consign )
{
    UpdateConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::NotifyUpdated
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void InfoButtonsWidget::NotifyUpdated( const LogFuneralConsign& consign )
{
    UpdateConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::OnUpdate
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void InfoButtonsWidget::OnUpdate()
{
    if( element_ && hasChanged_ )
    {
        unsigned int tick = simulation_.GetCurrentTick();
        if( lastTick_ != tick )
        {
            NotifySelected( element_ );
            hasChanged_ = false;
            lastTick_ = tick;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: InfoButtonsWidget::Refresh
// Created: LGY 2014-02-05
// -----------------------------------------------------------------------------
void InfoButtonsWidget::Refresh()
{
    if( element_ )
        NotifySelected( element_ );
}
