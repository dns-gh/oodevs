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
#include "actions/ActionWithTarget_ABC.h"
#include "actions_gui/MissionInterface.h"
#include "clients_gui/GLToolColors.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/RichWarnWidget.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatDecisions_ABC.h"
#include "clients_kernel/FragOrder.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/VariantPointer.h"
#include "ENT/ENT_Tr.h"
#include "gaming/AgentsModel.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/Decisions.h"
#include "gaming/EventAction.h"
#include "gaming/Model.h"
#include "gaming/PopulationDecisions.h"
#include "gaming/StaticModel.h"
#include "gaming/TimelinePublisher.h"
#include "timeline/api.h"

// -----------------------------------------------------------------------------
// Name: EventOrderWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventOrderWidget::EventOrderWidget( kernel::Controllers& controllers, Model& model, const tools::ExerciseConfig& config,
                                    actions::gui::InterfaceBuilder_ABC& interfaceBuilder, const kernel::Profile_ABC& profile,
                                    gui::GlTools_ABC& tools )
    : EventWidget_ABC()
    , controllers_( controllers )
    , model_( model )
    , interfaceBuilder_( interfaceBuilder )
    , profile_( profile )
    , tools_( tools )
    , contextMenuEntity_( controllers )
    , target_( controllers )
    , missionInterface_( 0 )
    , missionCombo_( 0 )
{
    // Top
    missionTypeCombo_ = new gui::RichWarnWidget< QComboBox >( "event-order-mission-type-combobox" );
    missionComboLayout_ = new QVBoxLayout();
    missionComboLayout_->setMargin( 0 );
    missionComboLayout_->setSpacing( 0 );
    targetLabel_ = new gui::RichLabel( "event-order-target-label", "---" );

    for( int i = 0; i < eNbrMissionTypes; ++i )
        missionTypeCombo_->insertItem( i, QString::fromStdString( ENT_Tr::ConvertFromMissionType( static_cast< E_MissionType >( i ) ) ) );
    missionTypeCombo_->setCurrentIndex( eMissionType_Pawn );
    currentType_ = eMissionType_Pawn;

    connect( missionTypeCombo_, SIGNAL( currentIndexChanged( int ) ), this, SLOT( OnMissionTypeChanged( int ) ) );

    targetGroupBox_ = new gui::RichGroupBox( "event-order-target-groupbox", tr( "Target" ) );
    QVBoxLayout* targetLayout = new QVBoxLayout( targetGroupBox_ );
    targetLayout->setContentsMargins( 5, 0, 5, 5 );
    targetLayout->addWidget( targetLabel_, 0, Qt::AlignCenter );

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setMargin( 5 );
    topLayout->setSpacing( 5 );
    topLayout->addWidget( missionTypeCombo_ );
    topLayout->addLayout( missionComboLayout_ );
    //topLayout->addStretch( 1 );
    topLayout->addWidget( targetGroupBox_, 1 );

    // Body
    missionInterface_ = new actions::gui::MissionInterface( 0, "event-mission-interface", controllers, config );
    connect( missionInterface_, SIGNAL( PlannedMission( const actions::Action_ABC&, timeline::Event* ) ), this, SLOT( OnPlannedMission( const actions::Action_ABC&, timeline::Event* ) ) );

    // Layout
    mainLayout_->addLayout( topLayout );
    mainLayout_->addWidget( missionInterface_, 1 );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventOrderWidget::~EventOrderWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Purge
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::Purge()
{
    contextMenuEntity_ = 0;
    SetTarget( 0 );
    missionTypeCombo_->setCurrentIndex( eMissionType_Pawn );
    currentType_ = eMissionType_Pawn;
    if( missionInterface_ )
        missionInterface_->Purge();
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

        // Fill combo with with action infos
        missionTypeCombo_->setCurrentIndex( eventAction.GetMissionType() );

        kernel::Entity_ABC* entity = model_.agents_.tools::Resolver< kernel::Agent_ABC >::Find( mission->GetEntityId() );
        if( entity == 0 )
            entity = model_.agents_.tools::Resolver< kernel::Automat_ABC >::Find( mission->GetEntityId() );
        if( entity == 0 )
            entity = model_.agents_.tools::Resolver< kernel::Population_ABC >::Find( mission->GetEntityId() );
        if( entity == 0 )
            throw MASA_EXCEPTION( "Entity not found" );
        SetTarget( entity );

        FillMission();
        missionCombo_->setCurrentIndex( missionCombo_->findText( order.GetName().c_str() ) );
        FillMissionInterface( eventAction );
    }
    else
       FillMission();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventOrderWidget::Commit( timeline::Event& event ) const
{
    if( !missionInterface_ )
        return;
    missionInterface_->SetPlanned( true );
    Publish( &event );
    event.action.target = CREATE_EVENT_TARGET( EVENT_ORDER_PROTOCOL, EVENT_SIMULATION_SERVICE );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Trigger
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::Trigger() const
{
    if( !missionInterface_ )
        return;
    missionInterface_->SetPlanned( false );
    Publish();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::IsValid
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
bool EventOrderWidget::IsValid() const
{
    return ( missionInterface_ ) ? missionInterface_->CheckValidity() : false;
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Publish
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::Publish( timeline::Event* event /* = 0 */ ) const
{
    if( !missionInterface_ )
        return;
    if( currentType_ == eMissionType_FragOrder )
        missionInterface_->Publish< kernel::FragOrderType >( model_.actions_, event );
    else
        missionInterface_->Publish< kernel::MissionType >( model_.actions_, event );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::AddSingleMission
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
template< typename T >
void EventOrderWidget::AddSingleOrder( const T& mission )
{
    QVariant* variant = new QVariant();
    variant->setValue( kernel::VariantPointer( &mission ) );
    QString missionName = QString::fromStdString( mission.GetName() );
    if( missionCombo_->findText( missionName ) == -1 )
        missionCombo_->insertItem( mission.GetId(), missionName, *variant );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::AddAllMissions
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
template< typename T >
void EventOrderWidget::AddAllOrders()
{
    auto it =  model_.static_.types_.tools::Resolver< T >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const T& mission = it.NextElement();
        if( mission.GetType() == currentType_ )
            AddSingleOrder( mission );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::AddCompatibleOrders
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
template< typename T >
void EventOrderWidget::AddCompatibleOrders( tools::Iterator< const T& > it )
{
    while( it.HasMoreElements() )
        AddSingleOrder( it.NextElement().GetType() );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::AddCompatibleFragOrders
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
void EventOrderWidget::AddCompatibleFragOrders( const Decisions_ABC& decisions )
{
    AddCompatibleOrders< kernel::FragOrder >( decisions.GetFragOrders() );

    auto it = decisions.GetMissions();
    while( it.HasMoreElements() )
    {
        auto itFragO = it.NextElement().CreateIterator();
        while( itFragO.HasMoreElements() )
            AddSingleOrder( itFragO.NextElement().GetType() );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::GetTargetDecision
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
// Name: EventOrderWidget::FillComboMission
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::FillMission()
{
    delete missionCombo_;
    missionCombo_ = new gui::RichWarnWidget< QComboBox >( "event-order-mission-combobox" );

    QSortFilterProxyModel* proxy = new QSortFilterProxyModel( missionCombo_ );
    proxy->setSourceModel( missionCombo_->model() );
    missionCombo_->model()->setParent( proxy );
    missionCombo_->setModel( proxy );
    missionComboLayout_->addWidget( missionCombo_ );

    if( target_ )
    {
        const Decisions_ABC* decisions = GetTargetDecision();
        assert( decisions != 0 );
        // $$$$ ABR 2013-06-11: Find what to do with a unit in an engaged automat, for now we display his available unit missions
        // if( !decisions->CanBeOrdered() )
        //     return;
        if( currentType_ == eMissionType_FragOrder )
            AddCompatibleFragOrders( *decisions );
        else
            AddCompatibleOrders< kernel::Mission >( decisions->GetMissions() );
    }
    else
    {
        if( currentType_ == eMissionType_FragOrder )
            AddAllOrders< kernel::FragOrderType >();
        else
            AddAllOrders< kernel::MissionType >();
    }
    missionCombo_->model()->sort( 0 );
    missionCombo_->setCurrentIndex( 0 );
    connect( missionCombo_, SIGNAL( currentIndexChanged( int ) ), this, SLOT( OnMissionChanged( int ) ) );
    missionChoosed_ = false;
    BuildMissionInterface();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::SetTarget
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::SetTarget( const kernel::Entity_ABC* entity )
{
    target_ = entity;
    targetLabel_->setText( ( target_ ) ? target_->GetName() : "---" );
    if( missionInterface_ )
        missionInterface_->SetEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::WarnIfTargetAndMissionAreNotCompatible
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::WarnIfTargetAndMissionAreNotCompatible() const
{
    if( target_ )
    {
        bool needWarning = true;
        const kernel::OrderType* order = static_cast< const kernel::OrderType* >( missionCombo_->itemData( missionCombo_->currentIndex() ).value< kernel::VariantPointer >().ptr_ );
        const Decisions_ABC* decisions = GetTargetDecision();
        assert( decisions != 0 && order != 0 );
        if( currentType_ == eMissionType_FragOrder )
        {
            auto it = decisions->GetFragOrders();
            while( needWarning && it.HasMoreElements() )
                needWarning = it.NextElement().GetName() != order->GetName();

            auto itMission = decisions->GetMissions();
            while( needWarning && itMission.HasMoreElements() )
            {
                auto itFragO = itMission.NextElement().CreateIterator();
                while( itFragO.HasMoreElements() )
                    needWarning = itFragO.NextElement().GetName() != order->GetName();
            }
        }
        else
        {
            auto it = decisions->GetMissions();
            while( needWarning && it.HasMoreElements() )
                needWarning = it.NextElement().GetName() != order->GetName();
        }
        if( needWarning )
        {
            if( missionInterface_ )
                missionInterface_->SetEntity( 0 );
            missionCombo_->Warn();
            targetGroupBox_->Warn();
            targetLabel_->Warn();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::BuildMissionInterface
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::BuildMissionInterface()
{
    if( !missionInterface_ )
        return;
    missionInterface_->Purge();
    const kernel::OrderType* order = static_cast< const kernel::OrderType* >( missionCombo_->itemData( missionCombo_->currentIndex() ).value< kernel::VariantPointer >().ptr_ );
    if( order )
        missionInterface_->Fill( interfaceBuilder_, *order, currentType_ );
    missionInterface_->SetEntity( target_ );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::FillMissionInterface
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::FillMissionInterface( const EventAction& /*event*/ )
{
    // Fill parameters with current action
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnMissionTypeChanged
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::OnMissionTypeChanged( int )
{
    if( !missionInterface_ )
        return;
    missionChoosed_ = false;
    currentType_ = static_cast< E_MissionType >( missionTypeCombo_->currentIndex() );
    if( target_ )
        SetTarget( 0 );
    FillMission();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnMissionChanged
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::OnMissionChanged( int )
{
    missionChoosed_ = true;
    BuildMissionInterface();
    WarnIfTargetAndMissionAreNotCompatible();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnTargetSelected
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::OnTargetSelected()
{
    if( !contextMenuEntity_ )
        return;
    SetTarget( contextMenuEntity_ );
    if( missionChoosed_ )
        WarnIfTargetAndMissionAreNotCompatible();
    else
        FillMission();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::PlanMission
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::OnPlannedMission( const actions::Action_ABC& action, timeline::Event* event ) const
{
    assert( event );
    action.Publish( model_.timelinePublisher_, 0 );
    event->action.payload = model_.timelinePublisher_.GetPayload();
    event->name = action.GetName();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::AddTargetToMenu
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
void EventOrderWidget::AddTargetToMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu, E_MissionType allowedType )
{
    if( profile_.CanBeOrdered( entity ) && isVisible() && ( currentType_ == allowedType || currentType_ == eMissionType_FragOrder ) )
    {
        contextMenuEntity_ = &entity;
        menu.InsertItem( "Target", tr( "Target" ), this, SLOT( OnTargetSelected() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyContextMenu
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    AddTargetToMenu( agent, menu, eMissionType_Pawn );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyContextMenu
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    AddTargetToMenu( automat, menu, eMissionType_Automat );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyContextMenu
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyContextMenu( const kernel::Population_ABC& population, kernel::ContextMenu& menu )
{
    AddTargetToMenu( population, menu, eMissionType_Population );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyDeleted
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( target_ == &entity || contextMenuEntity_ == &entity )
        Purge();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Draw
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void EventOrderWidget::Draw( gui::Viewport_ABC& viewport )
{
    if( isVisible() && missionInterface_ )
    {
        glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glLineWidth( 2.f );
        glColor4f( COLOR_PARAM );
        missionInterface_->Draw( tools_, viewport );
        glPopAttrib();
    }
}
