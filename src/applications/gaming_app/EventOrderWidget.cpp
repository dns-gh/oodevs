// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventOrderWidget.h"
#include "moc_EventOrderWidget.cpp"
#include "icons.h"
#include "actions/ActionWithTarget_ABC.h"
#include "actions_gui/MissionInterface.h"
#include "clients_gui/GLToolColors.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/EventManager.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWarnWidget.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatDecisions_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Time_ABC.h"
#include "ENT/ENT_Tr.h"
#include "gaming/AgentsModel.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/Decisions.h"
#include "gaming/EventAction.h"
#include "gaming/Model.h"
#include "gaming/PopulationDecisions.h"
#include "gaming/StaticModel.h"
#include "gaming/TimelinePublisher.h"
#include <timeline/api.h>
#include <boost/make_shared.hpp>

namespace
{
    const QBrush disabledColor = Qt::darkGray;
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventOrderWidget::EventOrderWidget( kernel::Controllers& controllers, Model& model, const tools::ExerciseConfig& config,
                                    actions::gui::InterfaceBuilder_ABC& interfaceBuilder, const kernel::Profile_ABC& profile,
                                    gui::GlTools_ABC& tools, const kernel::Time_ABC& simulation )
    : EventWidget_ABC()
    , controllers_( controllers )
    , model_( model )
    , interfaceBuilder_( interfaceBuilder )
    , profile_( profile )
    , tools_( tools )
    , simulation_( simulation )
    , selectedEntity_( 0 )
    , target_( 0 )
    , missionInterface_( new actions::gui::MissionInterface( 0, "event-mission-interface", controllers, config ) )
    , missionCombo_( 0 )
    , planningMode_( false )
    , manager_( new gui::EventManager( *this, model.static_.types_, interfaceBuilder, *missionInterface_ ) )
{
    // Top
    missionTypeCombo_ = new gui::RichWarnWidget< QComboBox >( "event-order-mission-type-combobox" );
    missionComboLayout_ = new QVBoxLayout();
    missionComboLayout_->setMargin( 0 );
    missionComboLayout_->setSpacing( 0 );
    targetLabel_ = new gui::RichLabel( "event-order-target-label", "---" );
    gui::RichPushButton* removeTargetButton = new gui::RichPushButton( "removeTargetButton", qApp->style()->standardIcon( QStyle::SP_DialogCloseButton ), "" );
    connect( removeTargetButton, SIGNAL( clicked() ), this, SLOT( OnTargetRemoved() ) );

    missionCombo_ = new gui::RichWarnWidget< QComboBox >( "event-order-mission-combobox" );
    QSortFilterProxyModel* proxy = new QSortFilterProxyModel( missionCombo_ );
    proxy->setSourceModel( missionCombo_->model() );
    missionCombo_->model()->setParent( proxy );
    missionCombo_->setModel( proxy );
    missionComboLayout_->addWidget( missionCombo_ );

    connect( missionCombo_, SIGNAL( currentIndexChanged( int ) ), this, SLOT( OnMissionChanged( int ) ) );
    connect( missionTypeCombo_, SIGNAL( currentIndexChanged( int ) ), this, SLOT( OnMissionTypeChanged( int ) ) );

    targetGroupBox_ = new gui::RichGroupBox( "event-order-target-groupbox", tr( "Target" ) );
    QHBoxLayout* internalTargetLayout = new QHBoxLayout( targetGroupBox_ );
    internalTargetLayout->setContentsMargins( 5, 0, 5, 5 );
    internalTargetLayout->addWidget( targetLabel_, 10, Qt::AlignCenter );
    internalTargetLayout->addWidget( removeTargetButton, 1, Qt::AlignRight );

    QHBoxLayout* targetLayout = new QHBoxLayout();
    targetLayout->addWidget( targetGroupBox_ );

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setSpacing( 5 );
    topLayout->addWidget( missionTypeCombo_ );
    topLayout->addLayout( missionComboLayout_ );

    // Layout
    mainLayout_->setSpacing( 5 );
    mainLayout_->addLayout( targetLayout );
    mainLayout_->addLayout( topLayout );
    mainLayout_->addWidget( missionInterface_.get(), 1 );

    controllers_.Register( *this );

    // Connections
    connect( missionInterface_.get(), SIGNAL( PlannedMission( const actions::Action_ABC&, timeline::Event* ) ), this, SLOT( OnPlannedMission( const actions::Action_ABC&, timeline::Event* ) ) );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventOrderWidget::~EventOrderWidget()
{
    controllers_.Unregister( *this );
}

namespace
{
    void PurgeComboBox( gui::RichWarnWidget< QComboBox >& combo )
    {
        combo.blockSignals( true );
        combo.clear();
        combo.blockSignals( false );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Purge
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::Purge()
{
    selectedEntity_ = 0;
    target_ = 0;
    PurgeComboBox( *missionTypeCombo_ );
    PurgeComboBox( *missionCombo_ );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Purge
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::Reset()
{
    missionInterface_->Rebuild( interfaceBuilder_ );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventOrderWidget::Fill( const Event& event )
{
    const EventAction& eventAction = static_cast< const EventAction& >( event );
    const actions::Action_ABC* action = eventAction.GetAction();
    if( action != 0 )
    {
        const actions::ActionWithTarget_ABC* mission = static_cast< const actions::ActionWithTarget_ABC* >( action );
        const kernel::OrderType& order = mission->GetType();

        kernel::Entity_ABC* entity = model_.agents_.tools::Resolver< kernel::Agent_ABC >::Find( mission->GetEntityId() );
        if( entity == 0 )
            entity = model_.agents_.tools::Resolver< kernel::Automat_ABC >::Find( mission->GetEntityId() );
        if( entity == 0 )
            entity = model_.agents_.tools::Resolver< kernel::Population_ABC >::Find( mission->GetEntityId() );

        target_ = entity;
        targetLabel_->setText( target_ ? target_->GetName() : "---" );

        const actions::Action_ABC* action = eventAction.GetAction();
        auto type = eventAction.GetMissionType() ;
        if( const Decisions_ABC* decisions = GetTargetDecision() )
            manager_->Select( *decisions, type, order.GetName(), action );
        else
            manager_->Select( type, order.GetName(), action );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventOrderWidget::Commit( timeline::Event& event ) const
{
    Publish( &event, true );
    event.action.target = CREATE_EVENT_TARGET( EVENT_ORDER_PROTOCOL, EVENT_SIMULATION_SERVICE );
    event.action.apply = IsValid(); // $$$$ ABR 2013-06-14: TODO: Separate missionInterface->CheckValidity() and missionInterface->Warn()*/
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Trigger
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::Trigger() const
{
    Publish( 0, false );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::IsValid
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
bool EventOrderWidget::IsValid() const
{
    return target_ && missionInterface_->CheckValidity() && !HasInvalidMission();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Warn
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void EventOrderWidget::Warn() const
{
    if( !target_ )
    {
        targetGroupBox_->Warn();
        targetLabel_->Warn();
    }
    if( HasInvalidMission() )
        missionCombo_->Warn();
    missionInterface_->CheckValidity(); // $$$$ ABR 2013-06-14: TODO Use missionInterface_->Warn() when ready
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Publish
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::Publish( timeline::Event* event, bool planned ) const
{
    manager_->Publish( model_.actions_, event, planned );
}

// -----------------------------------------------------------------------------
// Name: EventOrerWidget::GetTargetDecision
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
const Decisions_ABC* EventOrderWidget::GetTargetDecision() const
{
    const Decisions_ABC* decisions = 0;
    if( target_ )
    {
        if( target_->GetTypeName() == kernel::Agent_ABC::typeName_ )
            decisions = target_->Retrieve< Decisions >();
        if( target_->GetTypeName() == kernel::Automat_ABC::typeName_ )
            decisions = static_cast< const AutomatDecisions* >( target_->Retrieve< kernel::AutomatDecisions_ABC >() );
        if( target_->GetTypeName() == kernel::Population_ABC::typeName_ )
            decisions = target_->Retrieve< PopulationDecisions >();
    }
    return decisions;
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::SetTarget
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::SetTarget( const kernel::Entity_ABC* entity )
{
    target_ = entity;
    targetLabel_->setText( target_ ? target_->GetName() : "---" );

    if( const Decisions_ABC* decisions = GetTargetDecision() )
    {
        QVariant variant = missionTypeCombo_->itemData( missionTypeCombo_->currentIndex() );
        if( variant.isValid() )
            manager_->Select( *decisions, static_cast< E_MissionType >( variant.toUInt() ),
                              missionCombo_->currentText().toStdString() );
        else
            manager_->Select( *decisions );
    }
    else
    {
        QVariant variant = missionTypeCombo_->itemData( missionTypeCombo_->currentIndex() );
        if( variant.isValid() )
            manager_->Select( static_cast< E_MissionType >( variant.toUInt() ),
                              missionCombo_->currentText().toStdString() );
        else
            manager_->Select();
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnMissionTypeChanged
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::OnMissionTypeChanged( int value )
{
    QVariant variant = missionTypeCombo_->itemData( value );
    if( variant.isValid() )
    {
        auto type = static_cast< E_MissionType >( variant.toUInt() );
        if( const Decisions_ABC* decisions = GetTargetDecision() )
            manager_->Select( *decisions, type );
        else
            manager_->Select( type );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnMissionChanged
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::OnMissionChanged( int /*value*/ )
{
    QVariant variant = missionTypeCombo_->itemData( missionTypeCombo_->currentIndex() );
    if( variant.isValid() )
    {
        auto type = static_cast< E_MissionType >( variant.toUInt() );
        if( const Decisions_ABC* decisions = GetTargetDecision() )
            manager_->Select( *decisions, type, missionCombo_->currentText().toStdString() );
        else
            manager_->Select( type, missionCombo_->currentText().toStdString(), 0 );
        QVariant missionVariant = missionCombo_->itemData( missionCombo_->currentIndex(), Qt::ForegroundRole );
        if( missionVariant.isValid() )
        {
            QPalette palette = missionCombo_->palette();
            QBrush brush = missionVariant.value< QBrush >();
            palette.setColor( QPalette::Text, brush );
            missionCombo_->setPalette( palette );
            emit EnableTriggerEvent( brush != disabledColor );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::PlanMission
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::OnPlannedMission( const actions::Action_ABC& action, timeline::Event* event ) const
{
    if( event )
    {
        action.Publish( model_.timelinePublisher_, 0 );
        event->action.payload = model_.timelinePublisher_.GetPayload();
        event->name = action.GetName();
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnTargetRemoved
// Created: NPT 2013-07-30
// -----------------------------------------------------------------------------
void EventOrderWidget::OnTargetRemoved()
{
    SetTarget( 0 );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyContextMenu
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    if( const kernel::Automat_ABC* automat = static_cast< const kernel::Automat_ABC* >( agent.Get< kernel::TacticalHierarchies >().GetSuperior() ) )
        if( const kernel::AutomatDecisions_ABC* decisions = automat->Retrieve< kernel::AutomatDecisions_ABC >() )
        {
            const kernel::Entity_ABC* selectedEntity = 0;
            if( decisions->IsEmbraye() && profile_.CanBeOrdered( *automat ) )
                selectedEntity = automat;
            else if( profile_.CanBeOrdered( agent ) )
                selectedEntity = &agent;

            if( !selectedEntity )
                return;
            selectedEntity_ = selectedEntity;

            QAction* action = menu.InsertItem( "Order", tr( "Order" ), this, SLOT( ActivateMissionPanel() ), false, 2 );
            if( decisions->IsEmbraye() )
                action->setIcon( MAKE_PIXMAP( lock ) );
        }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyContextMenu
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    selectedEntity_ = &automat;
    if( profile_.CanBeOrdered( automat ) )
    {
        const kernel::AutomatDecisions_ABC* decisions = automat.Retrieve< kernel::AutomatDecisions_ABC >();
        QAction* action = menu.InsertItem( "Order", tr( "Order" ), this, SLOT( ActivateMissionPanel() ), false, 2 );
        if( decisions && decisions->IsEmbraye() )
            action->setIcon( MAKE_PIXMAP( lock ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyContextMenu
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyContextMenu( const kernel::Population_ABC& population, kernel::ContextMenu& menu )
{
    selectedEntity_ = &population;
    if( profile_.CanBeOrdered( population ) )
        menu.InsertItem( "Mission", tr( "Order" ), this, SLOT( ActivateMissionPanel() ), false, 2 );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyDeleted
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( target_ == &entity || selectedEntity_ == &entity )
        manager_->Select();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Draw
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void EventOrderWidget::Draw( gui::Viewport_ABC& viewport )
{
    if( isVisible() )
    {
        glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glLineWidth( 2.f );
        glColor4f( COLOR_PARAM );
        missionInterface_->Draw( tools_, viewport );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::ActivateMissionPanel
// Created: NPT 2013-07-29
// -----------------------------------------------------------------------------
void EventOrderWidget::ActivateMissionPanel()
{
    if( !selectedEntity_ )
        return;

    emit StartCreation( eEventTypes_Order, simulation_.GetDateTime() );
    SetTarget( selectedEntity_ );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyUpdated
// Created: LGY 2013-08-22
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyUpdated( const Decisions_ABC& decisions )
{
   if( selectedEntity_ && selectedEntity_->GetId() == decisions.GetAgent().GetId() )
        OnMissionChanged( 0 );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnPlanningModeToggled
// Created: LGY 2013-08-29
// -----------------------------------------------------------------------------
void EventOrderWidget::OnPlanningModeToggled( bool value )
{
    manager_->SetPlanningMode( value );
    if( planningMode_ != value )
    {
        planningMode_ = value;
        OnMissionChanged( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Build
// Created: LGY 2013-08-29
// -----------------------------------------------------------------------------
void EventOrderWidget::Build( const std::vector< E_MissionType >& types, E_MissionType currentType,
                              const std::vector< std::string >& missions, const std::string& currentMission,
                              const std::vector< std::string >& disabledMissions, bool invalid )
{
    missionTypeCombo_->blockSignals( true );
    // CLEAR
    missionTypeCombo_->clear();
    // FILL
    for( auto it = types.begin(); it != types.end(); ++it )
        missionTypeCombo_->addItem( QString::fromStdString( ENT_Tr::ConvertFromMissionType( *it ) ),
                                    QVariant( static_cast< unsigned int >( *it ) ) );
    // SELECT
    missionTypeCombo_->setCurrentIndex( missionTypeCombo_->findText(
        QString::fromStdString( ENT_Tr::ConvertFromMissionType( currentType ) ) ) );
    missionTypeCombo_->blockSignals( false );
    missionCombo_->blockSignals( true );
    // CLEAR
    missionCombo_->clear();
    // FILL
    for( auto it = missions.begin(); it != missions.end(); ++it )
    {
        const std::string& name = *it;
        missionCombo_->addItem( name.c_str() );
        QBrush missionColor = std::find( disabledMissions.begin(), disabledMissions.end(), name ) == disabledMissions.end()
            ? Qt::black
            : disabledColor;
        missionCombo_->setItemData( missionCombo_->count() - 1, missionColor, Qt::ForegroundRole );
    }
    // SELECT
    missionCombo_->setCurrentIndex( missionCombo_->findText( QString::fromStdString( currentMission ) ) );
    // Disable invalid mission
    if( invalid )
    {
        missionCombo_->setItemData( missionCombo_->currentIndex(), Qt::NoItemFlags, Qt::UserRole - 1 );
        missionCombo_->Warn();
        targetGroupBox_->Warn();
        targetLabel_->Warn();
    }
    missionCombo_->blockSignals( false );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::HasInvalidMission
// Created: LGY 2013-08-29
// -----------------------------------------------------------------------------
bool EventOrderWidget::HasInvalidMission() const
{
     return missionCombo_->itemData( missionCombo_->currentIndex(), Qt::UserRole - 1 ) == QVariant( Qt::NoItemFlags );
}
