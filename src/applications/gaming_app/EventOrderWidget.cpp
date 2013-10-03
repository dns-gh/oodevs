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
#include "clients_gui/RichLabel.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWarnWidget.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatDecisions_ABC.h"
#include "clients_kernel/FragOrder.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Time_ABC.h"
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
#include <timeline/api.h>

const QColor disableColor_ = Qt::gray;

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
    , lastGivenOrder_( 0 )
    , planningMode_( false )
{
    // Top
    missionTypeCombo_ = new gui::RichWarnWidget< QComboBox >( "event-order-mission-type-combobox" );
    missionComboLayout_ = new QVBoxLayout();
    missionComboLayout_->setMargin( 0 );
    missionComboLayout_->setSpacing( 0 );
    targetLabel_ = new gui::RichLabel( "event-order-target-label", "---" );
    gui::RichPushButton* removeTargetButton = new gui::RichPushButton( "removeTargetButton", qApp->style()->standardIcon( QStyle::SP_DialogCloseButton ), "" );
    connect( removeTargetButton, SIGNAL( clicked() ), this, SLOT( OnTargetRemoved() ) );

    for( int i = 0; i < eNbrMissionTypes; ++i )
        missionTypeCombo_->insertItem( i, QString::fromStdString( ENT_Tr::ConvertFromMissionType( static_cast< E_MissionType >( i ) ) ) );
    missionTypeCombo_->setCurrentIndex( eMissionType_Pawn );
    previousMissionType_ = eMissionType_Pawn;
    currentMissionType_ = eMissionType_Pawn;
    previousEntityType_ = eNbrMissionTypes;
    currentEntityType_ = eNbrMissionTypes;

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
    mainLayout_->addWidget( missionInterface_, 1 );

    controllers_.Register( *this );

    // Connections
    connect( missionInterface_, SIGNAL( PlannedMission( const actions::Action_ABC&, timeline::Event* ) ), this, SLOT( OnPlannedMission( const actions::Action_ABC&, timeline::Event* ) ) );
    connect( this, SIGNAL( SelectEntity( const kernel::Entity_ABC&, E_MissionType ) ), this, SLOT( OnSelectEntity( const kernel::Entity_ABC&, E_MissionType ) ) );
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
    selectedEntity_ = 0;
    SetTarget( 0 );
    missionInterface_->Purge();
    lastGivenOrder_ = 0;

    missionTypeCombo_->clear();
    missionTypeCombo_->blockSignals( true );
    for( int i = 0; i < eNbrMissionTypes; ++i )
        missionTypeCombo_->insertItem( i, QString::fromStdString( ENT_Tr::ConvertFromMissionType( static_cast< E_MissionType >( i ) ) ) );
    missionTypeCombo_->blockSignals( false );
    previousEntityType_ = eNbrMissionTypes;
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

        // Fill combo with action infos
        missionTypeCombo_->setCurrentIndex( eventAction.GetMissionType() );

        kernel::Entity_ABC* entity = model_.agents_.tools::Resolver< kernel::Agent_ABC >::Find( mission->GetEntityId() );
        if( entity == 0 )
            entity = model_.agents_.tools::Resolver< kernel::Automat_ABC >::Find( mission->GetEntityId() );
        if( entity == 0 )
            entity = model_.agents_.tools::Resolver< kernel::Population_ABC >::Find( mission->GetEntityId() );
        SetTarget( entity );

        FillMission();
        missionCombo_->setCurrentIndex( missionCombo_->findText( order.GetName().c_str() ) );
        FillMissionInterface( eventAction );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventOrderWidget::Commit( timeline::Event& event ) const
{
    missionInterface_->SetPlanned( true );
    Publish( &event );
    event.action.target = CREATE_EVENT_TARGET( EVENT_ORDER_PROTOCOL, EVENT_SIMULATION_SERVICE );
    event.action.apply = IsValid(); // $$$$ ABR 2013-06-14: TODO: Separate missionInterface->CheckValidity() and missionInterface->Warn()
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Trigger
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::Trigger() const
{
    missionInterface_->SetPlanned( false );
    Publish();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::IsValid
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
bool EventOrderWidget::IsValid() const
{
    return target_ && AreTargetAndMissionCompatible() && missionInterface_->CheckValidity();
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
    else if( !AreTargetAndMissionCompatible() )
        WarnTargetAndMission();
    missionInterface_->CheckValidity(); // $$$$ ABR 2013-06-14: TODO Use missionInterface_->Warn() when ready
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Publish
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::Publish( timeline::Event* event /* = 0 */ ) const
{
    if( currentMissionType_ == eMissionType_FragOrder )
        missionInterface_->Publish< kernel::FragOrderType >( model_.actions_, event );
    else
        missionInterface_->Publish< kernel::MissionType >( model_.actions_, event );
}

namespace
{
    void DisableItem( gui::RichWarnWidget< QComboBox >* comboBox, int index, bool isMission )
    {
        if( isMission )
            comboBox->setItemData( index, Qt::NoItemFlags, Qt::UserRole - 1 );
        else
        {
            QFont italicFont;
            italicFont.setItalic( true );
            comboBox->setItemData( index, QVariant( italicFont ), Qt::FontRole );
            comboBox->setItemData( index, disableColor_, Qt::TextColorRole);
        }
    }
    void EnableItem( gui::RichWarnWidget< QComboBox >* comboBox, int index )
    {
        comboBox->setItemData( index, QVariant( Qt::ItemIsSelectable | Qt::ItemIsEnabled ), Qt::UserRole - 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::AddSingleMission
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
template< typename T >
void EventOrderWidget::AddSingleOrder( const T& mission, bool disable )
{
    QVariant* variant = new QVariant();
    variant->setValue( kernel::VariantPointer( &mission ) );
    QString missionName = QString::fromStdString( mission.GetName() );
    if( missionCombo_->findText( missionName ) == -1 )
    {
        missionCombo_->insertItem( mission.GetId(), missionName, *variant );
        if( disable )
            DisableItem( missionCombo_, missionCombo_->findText( missionName ), currentMissionType_ != eMissionType_FragOrder );
    }
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
        if( mission.GetType() == currentMissionType_ )
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
    const kernel::Mission* currentMission = decisions.GetCurrentMission();
    AddCompatibleOrders< kernel::FragOrder >( decisions.GetFragOrders() );
    auto it = decisions.GetMissions();
    while( it.HasMoreElements() )
    {
        const kernel::Mission& mission = it.NextElement();
        auto itFragO = mission.CreateIterator();
        while( itFragO.HasMoreElements() )
        {
            const kernel::OrderType& order = itFragO.NextElement().GetType();
            bool active = ( !currentMission || currentMission->GetType().GetId() != mission.GetType().GetId() ) && !planningMode_;
            if( !active && missionCombo_->findText( order.GetName().c_str() ) != -1 )
                missionCombo_->removeItem( missionCombo_->findText( order.GetName().c_str() ) );
            AddSingleOrder( order, active );
        }
    }
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

namespace
{

    void SortAndSelect( QComboBox* combo, const std::string& name )
    {
        // Sort
        combo->model()->sort( 0 );
        // Select
        if( !name.empty() )
            combo->setCurrentIndex( combo->findText( name.c_str() ) );
        else
        {
            // Select first item enabled
            QVariant disabled( Qt::NoItemFlags );
            for( int index = 0; index < combo->count(); ++index )
                if( combo->itemData( index, Qt::UserRole - 1 ) != disabled )
                {
                    combo->setCurrentIndex( index );
                    return;
                }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::FillMission
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
        if( currentMissionType_ == eMissionType_FragOrder )
            AddCompatibleFragOrders( *decisions );
        else
            AddCompatibleOrders< kernel::Mission >( decisions->GetMissions() );

        if( lastGivenOrder_ && previousEntityType_ == currentEntityType_ && currentMissionType_ == previousMissionType_ )
        {
            AddSingleOrder( *lastGivenOrder_ );
            SortAndSelect( missionCombo_, lastGivenOrder_->GetName().c_str() );
            if( !AreTargetAndMissionCompatible( lastGivenOrder_ ) )
            {
                DisableItem( missionCombo_, missionCombo_->currentIndex(), currentMissionType_ != eMissionType_FragOrder );
                WarnTargetAndMission();
            }
        }
        else
            SortAndSelect( missionCombo_, "" );
    }
    else
    {
        if( currentMissionType_ == eMissionType_FragOrder )
            AddAllOrders< kernel::FragOrderType >();
        else
            AddAllOrders< kernel::MissionType >();
        SortAndSelect( missionCombo_, "" );
    }
    connect( missionCombo_, SIGNAL( currentIndexChanged( int ) ), this, SLOT( OnMissionChanged( int ) ) );
    missionChoosed_ = false;
    BuildMissionInterface( previousEntityType_ != currentEntityType_ || currentMissionType_ != previousMissionType_ );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::SetTarget
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::SetTarget( const kernel::Entity_ABC* entity )
{
    target_ = entity;
    targetLabel_->setText( ( target_ ) ? target_->GetName() : "---" );
    missionInterface_->SetEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::AreTargetAndMissionCompatible
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
bool EventOrderWidget::AreTargetAndMissionCompatible() const
{
    const kernel::OrderType* order = static_cast< const kernel::OrderType* >( missionCombo_->itemData( missionCombo_->currentIndex() ).value< kernel::VariantPointer >().ptr_ );
    return AreTargetAndMissionCompatible( order );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::AreTargetAndMissionCompatible
// Created: NPT 2013-07-30
// -----------------------------------------------------------------------------
bool EventOrderWidget::AreTargetAndMissionCompatible( const kernel::OrderType* currentOrder ) const
{
    if( target_ && currentOrder )
    {
        bool needWarning = true;
        const Decisions_ABC* decisions = GetTargetDecision();
        assert( decisions != 0 );
        if( currentMissionType_ == eMissionType_FragOrder )
        {
            auto it = decisions->GetFragOrders();
            while( needWarning && it.HasMoreElements() )
                needWarning = it.NextElement().GetName() != currentOrder->GetName();

            auto itMission = decisions->GetMissions();
            while( needWarning && itMission.HasMoreElements() )
            {
                auto itFragO = itMission.NextElement().CreateIterator();
                while( needWarning && itFragO.HasMoreElements() )
                    needWarning = itFragO.NextElement().GetName() != currentOrder->GetName();
            }
        }
        else
        {
            auto it = decisions->GetMissions();
            while( needWarning && it.HasMoreElements() )
                needWarning = it.NextElement().GetName() != currentOrder->GetName();
        }
        return !needWarning;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::WarnTargetAndMission
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void EventOrderWidget::WarnTargetAndMission() const
{
    missionInterface_->SetEntity( 0 );
    missionCombo_->Warn();
    targetGroupBox_->Warn();
    targetLabel_->Warn();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::BuildMissionInterface
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::BuildMissionInterface( bool resetAll )
{
    int tabIndex = missionInterface_->currentIndex();
    if( resetAll )
    {
        missionInterface_->Purge();
        const kernel::OrderType* order = static_cast< const kernel::OrderType* >( missionCombo_->itemData( missionCombo_->currentIndex() ).value< kernel::VariantPointer >().ptr_ );
        if( order )
            missionInterface_->Build( interfaceBuilder_, *order, currentMissionType_ );
    }
    missionInterface_->SetEntity( target_ );
    missionInterface_->setCurrentIndex( tabIndex );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::FillMissionInterface
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::FillMissionInterface( const EventAction& event )
{
    if( const actions::Action_ABC* action = event.GetAction() )
        missionInterface_->FillFrom( *action );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnMissionTypeChanged
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::OnMissionTypeChanged( int value )
{
    // Mission comboBox is empty
    if( value == -1 )
        return;

    missionChoosed_ = false;
    previousEntityType_ = currentEntityType_;
    previousMissionType_ = currentMissionType_;
    if( currentEntityType_ == eNbrMissionTypes )
        currentEntityType_ = eMissionType_Pawn;
    if( missionTypeCombo_->count() == 4 )
        currentMissionType_ = static_cast< E_MissionType >( value );
    else
        currentMissionType_ = static_cast< E_MissionType >( value == 0 ? currentEntityType_ : eMissionType_FragOrder );
    FillMission();
    emit EnableTriggerEvent( true );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnMissionChanged
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::OnMissionChanged( int )
{
    missionChoosed_ = true;
    BuildMissionInterface( true );
    if( !AreTargetAndMissionCompatible() )
        WarnTargetAndMission();
    else if( lastGivenOrder_ && !AreTargetAndMissionCompatible( lastGivenOrder_ ) )
        missionCombo_->removeItem( missionCombo_->findText( lastGivenOrder_->GetName().c_str() ) );
    QColor itemColor = missionCombo_->itemData( missionCombo_->currentIndex(), Qt::TextColorRole ).value< QColor >();
    emit EnableTriggerEvent( currentMissionType_ != eMissionType_FragOrder || itemColor != disableColor_ );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnTargetSelected
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::OnTargetSelected()
{
    if( !selectedEntity_ )
        return;
    SetTarget( selectedEntity_ );
    if( missionChoosed_ )
    {
        if( !AreTargetAndMissionCompatible() )
            WarnTargetAndMission();
    }
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
// Name: EventOrderWidget::OnSelectMission
// Created: ABR 2013-07-03
// -----------------------------------------------------------------------------
void EventOrderWidget::OnSelectEntity( const kernel::Entity_ABC& entity, E_MissionType type )
{
    if( missionCombo_ && missionTypeCombo_->count() != 4 )
    {
        int index = missionCombo_->currentIndex();
        index = index;
        lastGivenOrder_ = static_cast< const kernel::OrderType* >( missionCombo_->itemData( missionCombo_->currentIndex() ).value< kernel::VariantPointer >().ptr_ );
    }

    if( previousEntityType_ != type || missionTypeCombo_->count() != 2 )
    {
        missionTypeCombo_->clear();
        missionTypeCombo_->blockSignals( true );
        missionTypeCombo_->addItem( QString::fromStdString( ENT_Tr::ConvertFromMissionType( type ) ) );
        missionTypeCombo_->addItem( QString::fromStdString( ENT_Tr::ConvertFromMissionType( eMissionType_FragOrder ) ) );
        missionTypeCombo_->blockSignals( false );
        OnMissionTypeChanged( 0 );
        lastGivenOrder_ = 0;
    }
    previousMissionType_ = currentMissionType_;
    SetTarget( &entity );
    FillMission();
    BuildMissionInterface( missionInterface_->IsEmpty() );

    QColor itemColor = missionCombo_->itemData( missionCombo_->currentIndex(), Qt::TextColorRole ).value< QColor >();
    emit EnableTriggerEvent( itemColor != disableColor_ );

}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnTargetRemoved
// Created: NPT 2013-07-30
// -----------------------------------------------------------------------------
void EventOrderWidget::OnTargetRemoved()
{
    missionTypeCombo_->clear();
    for( int i = 0; i < eNbrMissionTypes; ++i )
        missionTypeCombo_->insertItem( i, QString::fromStdString( ENT_Tr::ConvertFromMissionType( static_cast< E_MissionType >( i ) ) ) );
    Purge();
    FillMission();
    currentEntityType_ = eNbrMissionTypes;
    previousEntityType_ = eNbrMissionTypes;
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
        Purge();
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

    previousEntityType_ = currentEntityType_;
    currentEntityType_ = selectedEntity_->GetTypeName() == kernel::Population_ABC::typeName_? eMissionType_Population : selectedEntity_->GetTypeName() == kernel::Automat_ABC::typeName_ ? eMissionType_Automat : eMissionType_Pawn ;

    const kernel::Entity_ABC& entity = *selectedEntity_;
    E_MissionType missionType = currentEntityType_;
    if( previousEntityType_ != currentEntityType_ )
        emit StartCreation( eEventTypes_Order, simulation_.GetDateTime() );
    else
        emit UpdateCreation( eEventTypes_Order, simulation_.GetDateTime() );

    emit SelectEntity( entity, missionType );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyUpdated
// Created: LGY 2013-08-22
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyUpdated( const Decisions_ABC& decisions )
{
    if( selectedEntity_ && selectedEntity_->GetId() == decisions.GetAgent().GetId() &&
        currentMissionType_ == eMissionType_FragOrder )
    {
        FillMission();
        SortAndSelect( missionCombo_, "" );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnPlanningModeToggled
// Created: LGY 2013-08-29
// -----------------------------------------------------------------------------
void EventOrderWidget::OnPlanningModeToggled( bool value )
{
    planningMode_ = value;
    if( currentMissionType_ == eMissionType_FragOrder )
    {
        if( value )
            for( int index = 0; index < missionCombo_->count(); ++index )
                EnableItem( missionCombo_, index );
    }
}
