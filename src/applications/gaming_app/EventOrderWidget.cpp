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
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/EventOrderPresenter.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWarnWidget.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatDecisions_ABC.h"
#include "clients_kernel/EventAction.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/TimelinePublisher.h"
#include "ENT/ENT_Tr.h"
#include "gaming/AgentsModel.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/Decisions.h"
#include "gaming/Model.h"
#include "gaming/PopulationDecisions.h"
#include "gaming/StaticModel.h"
#include "gaming/MissionParameters.h"
#include "tools/GeneralConfig.h"
#include <timeline/api.h>
#include <boost/assign/list_of.hpp>
#include <boost/make_shared.hpp>

namespace
{
    const QBrush disabledColor = Qt::darkGray;
    const QBrush invalidColor = Qt::red;
    const QBrush defaultColor = Qt::black;
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventOrderWidget::EventOrderWidget( kernel::Controllers& controllers, Model& model, const tools::ExerciseConfig& config,
                                    actions::gui::InterfaceBuilder_ABC& interfaceBuilder, const kernel::Profile_ABC& profile,
                                    gui::GlTools_ABC& tools, const kernel::Time_ABC& simulation, const gui::EntitySymbols& entitySymbols )
    : EventWidget_ABC()
    , controllers_( controllers )
    , model_( model )
    , interfaceBuilder_( interfaceBuilder )
    , profile_( profile )
    , tools_( tools )
    , simulation_( simulation )
    , entitySymbols_( entitySymbols )
    , selectedEntity_( 0 )
    , alternateSelectedEntity_( 0 )
    , target_( 0 )
    , missionInterface_( new actions::gui::MissionInterface( 0, "event-mission-interface", controllers, config ) )
    , missionCombo_( 0 )
    , planningMode_( false )
    , manager_( new gui::EventOrderPresenter( *this, model.static_.types_, interfaceBuilder, *missionInterface_ ) )
    , context_( 0 )
{
    // Top
    missionTypeCombo_ = new gui::RichWarnWidget< QComboBox >( "event-order-mission-type-combobox" );
    missionComboLayout_ = new QVBoxLayout();
    missionComboLayout_->setMargin( 0 );
    missionComboLayout_->setSpacing( 0 );
    targetLabel_ = new gui::RichLabel( "event-order-target-label", "---" );
    symbolLabel_ = new gui::RichLabel( "event-order-target-symbol-label" );
    activateTargetButton_ = new gui::RichPushButton( "activateTargetButton", gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/center_time.png" ) ), "" );
    activateTargetButton_->setToolTip( tr( "Select" ) );
    connect( activateTargetButton_, SIGNAL( clicked() ), this, SLOT( OnTargetActivated() ) );

    removeTargetButton_ = new gui::RichPushButton( "removeTargetButton", qApp->style()->standardIcon( QStyle::SP_DialogCloseButton ), "" );
    removeTargetButton_->setToolTip( tr( "Remove" ) );
    connect( removeTargetButton_, SIGNAL( clicked() ), this, SLOT( OnTargetRemoved() ) );

    missionCombo_ = new gui::RichWarnWidget< QComboBox >( "event-order-mission-combobox" );
    QSortFilterProxyModel* proxy = new QSortFilterProxyModel( missionCombo_ );
    proxy->setSourceModel( missionCombo_->model() );
    missionCombo_->model()->setParent( proxy );
    missionCombo_->setModel( proxy );
    missionComboLayout_->addWidget( missionCombo_ );

    connect( missionCombo_, SIGNAL( currentIndexChanged( int ) ), this, SLOT( SelectWhenTargetOrMissionChanged() ) );
    connect( missionTypeCombo_, SIGNAL( currentIndexChanged( int ) ), this, SLOT( SelectWhenMissionTypeChanged() ) );

    targetGroupBox_ = new gui::RichGroupBox( "event-order-target-groupbox", tr( "Recipient" ) );
    QHBoxLayout* internalTargetLayout = new QHBoxLayout( targetGroupBox_ );
    internalTargetLayout->setContentsMargins( 5, 0, 5, 5 );
    QWidget* symbolWidget = new QWidget();
    QHBoxLayout* symbolLayout = new QHBoxLayout( symbolWidget );
    symbolLayout->setMargin( 0 );
    symbolLayout->addWidget( symbolLabel_ );
    symbolLayout->addWidget( targetLabel_ );
    internalTargetLayout->addWidget( symbolWidget, 10, Qt::AlignCenter );
    internalTargetLayout->addWidget( activateTargetButton_, 1, Qt::AlignRight );
    internalTargetLayout->addWidget( removeTargetButton_, 1, Qt::AlignRight );

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
    missionInterface_->Purge();
    selectedEntity_ = 0;
    alternateSelectedEntity_ = 0;
    SetTarget( 0 );
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
    UpdateTriggerAction();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventOrderWidget::Fill( const kernel::Event& event )
{
    const kernel::EventAction& eventAction = static_cast< const kernel::EventAction& >( event );
    const actions::Action_ABC* action = eventAction.GetAction();
    if( action != 0 )
    {
        const actions::ActionWithTarget_ABC* mission = static_cast< const actions::ActionWithTarget_ABC* >( action );
        kernel::Entity_ABC* entity = model_.agents_.tools::Resolver< kernel::Agent_ABC >::Find( mission->GetEntityId() );
        if( entity == 0 )
            entity = model_.agents_.tools::Resolver< kernel::Automat_ABC >::Find( mission->GetEntityId() );
        if( entity == 0 )
            entity = model_.agents_.tools::Resolver< kernel::Population_ABC >::Find( mission->GetEntityId() );
        SetTarget( entity );
        SelectWhenEventExist( *mission, eventAction.GetMissionType() );
    }
    else
        SelectWhenTargetOrMissionChanged(); // use this selection so it keep existing mission type && mission
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventOrderWidget::Commit( timeline::Event& event )
{
    Publish( &event, true );
    event.action.target = CREATE_EVENT_TARGET( EVENT_ORDER_PROTOCOL, EVENT_SIMULATION_SERVICE );
    event.action.apply = IsValid(); // $$$$ ABR 2013-06-14: TODO: Separate missionInterface->CheckValidity() and missionInterface->Warn()*/
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Trigger
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::Trigger()
{
    Publish( 0, false );
}

namespace
{
    bool IsValidTarget( const kernel::Entity_ABC* entity )
    {
        if( !entity )
            return false;
        if( entity->GetTypeName() == kernel::Agent_ABC::typeName_ )
        {
            if( auto automat = static_cast< const kernel::Automat_ABC* >( entity->Get< kernel::TacticalHierarchies >().GetSuperior() ) )
                if( auto decisions = automat->Retrieve< kernel::AutomatDecisions_ABC >() )
                    if( decisions->IsEmbraye() )
                        return false;
        }
        else
            if( entity->GetTypeName() == kernel::Automat_ABC::typeName_ )
                if( auto decisions = entity->Retrieve< kernel::AutomatDecisions_ABC >() )
                    if( !decisions->IsEmbraye() )
                        return false;
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::IsValid
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
bool EventOrderWidget::IsValid() const
{
    QVariant missionColor = missionCombo_->itemData( missionCombo_->currentIndex(), Qt::ForegroundRole );
    return IsValidTarget( target_ ) &&                          // check target validity
           missionInterface_->CheckValidity() &&                // check parameters validity
           missionColor.isValid() &&                            // check mission validity
           missionColor.value< QBrush >() != disabledColor &&
           missionColor.value< QBrush >() != invalidColor;
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Publish
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::Publish( timeline::Event* event, bool planned )
{
    manager_->Publish( model_.actions_, event, planned, ++context_ );
}

// -----------------------------------------------------------------------------
// Name: EventOrerWidget::GetTargetDecision
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
const kernel::Decisions_ABC* EventOrderWidget::GetTargetDecision() const
{
    const kernel::Decisions_ABC* decisions = 0;
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
    bool hasTarget = target_ != 0;
    targetLabel_->setText( hasTarget ? target_->GetName() : "---" );
    activateTargetButton_->setEnabled( hasTarget );
    removeTargetButton_->setEnabled( hasTarget );

    QPixmap pixmap;
    if( hasTarget )
        if( auto symbol = entity->Retrieve< kernel::TacticalHierarchies >() )
        {
            pixmap = entitySymbols_.GetSymbol( *entity, symbol->GetSymbol(), symbol->GetLevel(),
                                                QSize( 64, 64 ), gui::EntitySymbols::eColorWithModifier );
            pixmap = pixmap.scaled( QSize( 48, 48 ), Qt::KeepAspectRatio, Qt::SmoothTransformation );
        }
    symbolLabel_->setPixmap( pixmap );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::GetMissionType
// Created: ABR 2013-10-31
// -----------------------------------------------------------------------------
E_MissionType EventOrderWidget::GetMissionType() const
{
    QVariant variant = missionTypeCombo_->itemData( missionTypeCombo_->currentIndex() );
    if( !variant.isValid() )
        return eNbrMissionType;
    return static_cast< E_MissionType >( variant.toUInt() );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::SelectDefault
// Created: ABR 2013-10-31
// -----------------------------------------------------------------------------
void EventOrderWidget::SelectDefault()
{
    if( const kernel::Decisions_ABC* decisions = GetTargetDecision() )
        manager_->Select( *decisions );
    else
        manager_->Select();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::SelectWhenEventExist
// Created: ABR 2013-10-31
// -----------------------------------------------------------------------------
void EventOrderWidget::SelectWhenEventExist( const actions::ActionWithTarget_ABC& action, E_MissionType type )
{
    if( const kernel::Decisions_ABC* decisions = GetTargetDecision() )
        manager_->Select( *decisions, type, action.GetType().GetName(), &action );
    else
        manager_->Select( type, action.GetType().GetName(), &action );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::SelectWhenTargetOrMissionChanged
// Created: ABR 2013-10-31
// -----------------------------------------------------------------------------
void EventOrderWidget::SelectWhenTargetOrMissionChanged()
{
    E_MissionType type = GetMissionType();
    if( type == eNbrMissionType )
        SelectDefault();
    else if( const kernel::Decisions_ABC* decisions = GetTargetDecision() )
        manager_->Select( *decisions, type, missionCombo_->currentText().toStdString() );
    else
        manager_->Select( type, missionCombo_->currentText().toStdString() );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::SelectWhenMissionTypeChanged
// Created: ABR 2013-10-31
// -----------------------------------------------------------------------------
void EventOrderWidget::SelectWhenMissionTypeChanged()
{
    E_MissionType type = GetMissionType();
    if( type == eNbrMissionType )
        SelectDefault();
    else if( const kernel::Decisions_ABC* decisions = GetTargetDecision() )
        manager_->Select( *decisions, type );
    else
        manager_->Select( type );
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
    if( !target_ )
        throw MASA_EXCEPTION( "Can't remove an unset target" );
    SetTarget( 0 );
    SelectWhenTargetOrMissionChanged();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnTargetActivated
// Created: ABR 2013-10-31
// -----------------------------------------------------------------------------
void EventOrderWidget::OnTargetActivated() const
{
    if( !target_ )
        throw MASA_EXCEPTION( "Can't activate an unset target" );
    target_->Select( controllers_.actions_ );
    target_->MultipleSelect( controllers_.actions_, boost::assign::list_of( target_ ) );
    target_->Activate( controllers_.actions_ );
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
            {
                action->setIcon( MAKE_PIXMAP( lock ) );
                alternateSelectedEntity_ = &agent;
                menu.InsertItem( "Order", tr( "Order (unit)" ), this, SLOT( ActivateMissionPanelOnUnit() ), false, 3 );
            }
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
    if( target_ == &entity )
    {
        SetTarget( 0 );
        manager_->Select();
    }
    if( selectedEntity_ == &entity )
        selectedEntity_ = 0;
    if( alternateSelectedEntity_ == &entity )
        alternateSelectedEntity_ = 0;
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
    emit StartCreation( eEventTypes_Order, simulation_.GetDateTime(), false );
    SetTarget( selectedEntity_ );
    SelectWhenTargetOrMissionChanged();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::ActivateMissionPanelOnUnit
// Created: ABR 2013-10-31
// -----------------------------------------------------------------------------
void EventOrderWidget::ActivateMissionPanelOnUnit()
{
    if( !alternateSelectedEntity_ )
        return;
    emit StartCreation( eEventTypes_Order, simulation_.GetDateTime(), false );
    SetTarget( alternateSelectedEntity_ );
    SelectWhenTargetOrMissionChanged();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyUpdated
// Created: LGY 2013-08-22
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyUpdated( const kernel::Decisions_ABC& decisions )
{
   if( selectedEntity_ && selectedEntity_->GetId() == decisions.GetAgent().GetId() ||
       alternateSelectedEntity_ && alternateSelectedEntity_->GetId() == decisions.GetAgent().GetId() )
        SelectWhenTargetOrMissionChanged();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyUpdated
// Created: ABR 2013-11-13
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyUpdated( const actions::gui::Param_ABC& param )
{
    if( missionInterface_->HasParameter( param ) )
    {
        UpdateTriggerAction();
        emit EventChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::UpdateTriggerAction
// Created: ABR 2013-11-13
// -----------------------------------------------------------------------------
void EventOrderWidget::UpdateTriggerAction()
{
    emit EnableTriggerEvent( IsValid() );
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
        SelectWhenTargetOrMissionChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Build
// Created: LGY 2013-08-29
// -----------------------------------------------------------------------------
void EventOrderWidget::Build( const gui::EventOrderViewState& state )
{
    missionTypeCombo_->blockSignals( true );
    // CLEAR
    missionTypeCombo_->clear();
    // FILL
    for( auto it = state.types_.begin(); it != state.types_.end(); ++it )
        missionTypeCombo_->addItem( QString::fromStdString( ENT_Tr::ConvertFromMissionType( *it ) ),
                                    QVariant( static_cast< unsigned int >( *it ) ) );
    // SELECT
    missionTypeCombo_->setCurrentIndex( missionTypeCombo_->findText(
        QString::fromStdString( ENT_Tr::ConvertFromMissionType( state.currentType_ ) ) ) );
    missionTypeCombo_->blockSignals( false );
    missionCombo_->blockSignals( true );
    // CLEAR
    missionCombo_->clear();
    // FILL
    for( auto it = state.missions_.begin(); it != state.missions_.end(); ++it )
    {
        const std::string& name = *it;
        missionCombo_->addItem( name.c_str() );
        QBrush missionColor = defaultColor;
        if( name == state.currentMission_ )
        {
            if( state.invalid_ )
                missionColor = invalidColor;
            else if( state.missionSelector_ )
                missionColor = disabledColor;
        }
        else
            missionColor = std::find( state.disabledMissions_.begin(), state.disabledMissions_.end(), name ) == state.disabledMissions_.end()
                ? defaultColor
                : disabledColor;
        missionCombo_->setItemData( missionCombo_->count() - 1, missionColor, Qt::ForegroundRole );
    }
    // SELECT
    missionCombo_->setCurrentIndex( missionCombo_->findText( QString::fromStdString( state.currentMission_ ) ) );
    // Disable invalid mission
    if( state.invalid_ || state.missionSelector_ )
        missionCombo_->setItemData( missionCombo_->currentIndex(), Qt::NoItemFlags, Qt::UserRole - 1 );
    missionCombo_->EnableStaticWarning( state.invalid_ );
    targetGroupBox_->EnableStaticWarning( state.invalid_ );
    targetLabel_->EnableStaticWarning( state.invalid_ );
    missionCombo_->blockSignals( false );

    // Update actions
    UpdateTriggerAction();
    emit EventChanged();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyUpdated
// Created: LGY 2013-08-29
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyUpdated( const MissionParameters& extension )
{
    if( target_ && target_->GetId() == extension.GetEntityId() )
    {
        const actions::Action_ABC* action = extension.GetLastMission();
        if( action && action->GetContext() == context_ )
            emit GetMissionAck( *action );
    }
}
