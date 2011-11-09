// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentListView.h"
#include "moc_AgentListView.cpp"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Army.h"
#include "actions/Automat.h"
#include "actions/KnowledgeGroup.h"
#include "actions/KnowledgeGroupMagicAction.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Options.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/StaticModel.h"
#include "gaming/CommandPostAttributes.h"
#include "gaming/Attributes.h"
#include "icons.h"
#include "protocol/SimulationSenders.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
    : gui::HierarchyListView< kernel::CommunicationHierarchies >( pParent, controllers, factory, profile, icons )
    , actionsModel_        ( actionsModel )
    , static_              ( staticModel )
    , simulation_          ( simulation )
    , lock_                ( MAKE_PIXMAP( lock ) )
    , scisors_             ( MAKE_PIXMAP( scisors ) ) // LTO
    , commandPost_         ( MAKE_PIXMAP( commandpost ) )
    , changeSuperiorDialog_( 0 )
{
    controllers_.Register( *this );
    addColumn( "HiddenPuce", 15 );
    setColumnAlignment( 1, Qt::AlignCenter );
}

// -----------------------------------------------------------------------------
// Name: AgentListView destructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentListView::~AgentListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::hideEvent
// Created: JSR 2011-11-09
// -----------------------------------------------------------------------------
void AgentListView::hideEvent( QHideEvent* event )
{
    if( changeSuperiorDialog_ )
        changeSuperiorDialog_->hide();
    gui::HierarchyListView< kernel::CommunicationHierarchies >::hideEvent( event );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::viewportResizeEvent
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void AgentListView::viewportResizeEvent( QResizeEvent* e )
{
    Q3ScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::setColumnWidth
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void AgentListView::setColumnWidth( int column, int w )
{
    Q3ListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) : w );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void AgentListView::Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item )
{
    const AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >();
    if( decisions )
        item->setPixmap( 1, decisions->IsEmbraye() ? lock_ : QPixmap() );
    else if( const CommandPostAttributes* commandPost = entity.Retrieve< CommandPostAttributes >() )
        item->setPixmap( 1, commandPost->IsCommandPost() ? commandPost_ : QPixmap() );
    // LTO begin
    else if( const kernel::KnowledgeGroup_ABC* kg = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &entity ) ) // $$$$ _RC_ SLG 2009-12-21: TEMP
        item->setPixmap( 1, !kg->IsActivated() ? scisors_ : QPixmap() );
    // LTO end

    if( const Attributes* attributes = static_cast< const Attributes* >( entity.Retrieve< kernel::Attributes_ABC >() ) )
    {
        QColor color;
        if( attributes->bNeutralized_ )
            color = QColor( controllers_.options_.GetOption( "Color/Neutralized", QString( "" ) ).To< QString >() );
        if( attributes->nOpState_ == eOperationalStatus_DetruitTactiquement )
            color = QColor( controllers_.options_.GetOption( "Color/TacticallyDestroyed", QString( "" ) ).To< QString >() );
        if( attributes->nOpState_ == eOperationalStatus_DetruitTotalement )
            color = QColor( controllers_.options_.GetOption( "Color/TotallyDestroyed", QString( "" ) ).To< QString >() );
        item->SetBackgroundColor( color );
    }

    gui::HierarchyListView< kernel::CommunicationHierarchies >::Display( entity, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::CanChangeSuperior
// Created: JSR 2011-11-09
// -----------------------------------------------------------------------------
bool AgentListView::CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const
{
    const kernel::Automat_ABC*        automat = dynamic_cast< const kernel::Automat_ABC* >       ( &entity );
    const kernel::KnowledgeGroup_ABC* group   = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &superior );
    if( automat && group )
        return &entity.Get< kernel::CommunicationHierarchies >().GetTop() == &superior.Get< kernel::CommunicationHierarchies >().GetTop();
    else if( const kernel::KnowledgeGroup_ABC* knowledgegroup = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &entity ) )
    {
        const kernel::Entity_ABC* com = &knowledgegroup->Get< kernel::CommunicationHierarchies >().GetTop();
        if( const kernel::Entity_ABC* team = dynamic_cast< const kernel::Entity_ABC* >( &superior ) )
            return com == team;
        else if( group && ( knowledgegroup != group ) )
            return com == &superior.Get< kernel::CommunicationHierarchies >().GetTop();
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::DoChangeSuperior
// Created: JSR 2011-11-09
// -----------------------------------------------------------------------------
void AgentListView::DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior )
{
    Drop( entity, superior );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnChangeKnowledgeGroup
// Created: JSR 2011-11-09
// -----------------------------------------------------------------------------
void AgentListView::OnChangeKnowledgeGroup()
{
    if( gui::ValuedListItem* valuedItem = static_cast< gui::ValuedListItem* >( selectedItem() ) )
    {
        kernel::Entity_ABC& entity = *valuedItem->GetValue< kernel::Entity_ABC >();
        if( !changeSuperiorDialog_ )
            changeSuperiorDialog_ = new gui::ChangeSuperiorDialog( this, controllers_, *this, true );
        changeSuperiorDialog_->Show( entity );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const AutomatDecisions& decisions )
{
    const kernel::Entity_ABC* agent = & decisions.GetAgent();
    gui::ValuedListItem* item = gui::FindItem( agent, firstChild() );
    if( item )
        item->setPixmap( 1, decisions.IsEmbraye() ? lock_ : QPixmap() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: SBO 2006-08-18
// LTO
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const kernel::KnowledgeGroup_ABC& knowledgeGroup )
{
    const kernel::Entity_ABC* agent = (const kernel::Entity_ABC*)&knowledgeGroup;
    gui::ValuedListItem* item = gui::FindItem( agent, firstChild() );
    if( item )
        item->setPixmap( 1, !knowledgeGroup.IsActivated() ? scisors_ : QPixmap() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyContextMenu
// Created: JSR 2011-11-09
// -----------------------------------------------------------------------------
void AgentListView::NotifyContextMenu( const kernel::Automat_ABC& /*automat*/, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Command", tr( "Change knowledge group" ), this, SLOT( OnChangeKnowledgeGroup() ) );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyContextMenu
// Created: JSR 2011-11-09
// -----------------------------------------------------------------------------
void AgentListView::NotifyContextMenu( const kernel::KnowledgeGroup_ABC& /*group*/, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Command", tr( "Change knowledge group" ), this, SLOT( OnChangeKnowledgeGroup() ) );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target )
{
    const kernel::Agent_ABC* agent = dynamic_cast< const kernel::Agent_ABC* >( &item );
    if( agent )
    {
        const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &target );
        if( automat )
            return Drop( *agent, *automat );
        const kernel::Agent_ABC* targetAgent = dynamic_cast< const kernel::Agent_ABC* >( &target );
        if( targetAgent )
            return Drop( *agent, *targetAgent );
        return false;
    }

    const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &item );
    if( automat )
    {
        const kernel::KnowledgeGroup_ABC* group = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &target );
        return group && Drop( *automat, *group );
    }

    // LTO begin
    const kernel::KnowledgeGroup_ABC* knowledgeGroup = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &item );
    if( knowledgeGroup )
    {
        const kernel::KnowledgeGroup_ABC* groupParent = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &target );
        if( groupParent )
            return Drop( *knowledgeGroup, *groupParent );
        const kernel::Team_ABC* teamParent = dynamic_cast< const kernel::Team_ABC* >( &target );
        if( teamParent )
            return Drop( *knowledgeGroup, *teamParent );
    }
    // LTO end
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const kernel::Agent_ABC& item, const kernel::Agent_ABC& target )
{
    return Drop( item, target.Get< kernel::CommunicationHierarchies >().GetUp() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const kernel::Agent_ABC& item, const kernel::Automat_ABC& target )
{
    if( & item.Get< kernel::CommunicationHierarchies >().GetUp() == &target )
        return false;

    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "unit_change_superior" );
    UnitMagicAction* action = new UnitMagicAction( item, actionType, controllers_.controller_,  tr( "Unit Change Superior" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Automat( it.NextElement(), target, controllers_.controller_ ) );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &item, false ) );
    action->RegisterAndPublish( actionsModel_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const kernel::Automat_ABC& item, const kernel::KnowledgeGroup_ABC& target )
{
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "change_knowledge_group" );
    UnitMagicAction* action = new UnitMagicAction( item, actionType, controllers_.controller_, tr( "Change Knowledge Group" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::KnowledgeGroup( it.NextElement(), target, controllers_.controller_ ) );
    if( const kernel::Team_ABC *team = dynamic_cast< const kernel::Team_ABC* >( &target.Get< kernel::CommunicationHierarchies >().GetTop() ) )
        action->AddParameter( *new parameters::Army( it.NextElement(), *team, controllers_.controller_ ) );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &item, false ) );
    action->RegisterAndPublish( actionsModel_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: FHD 2010-01-04:
// LTO
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Team_ABC& target )
{
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "knowledge_group_update_side" );
    KnowledgeGroupMagicAction* action = new KnowledgeGroupMagicAction( item, actionType, controllers_.controller_, true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    if( const kernel::Team_ABC *team = dynamic_cast< const kernel::Team_ABC* >( &target.Get< kernel::CommunicationHierarchies >().GetTop() ) )
        action->AddParameter( *new parameters::Army( it.NextElement(), *team, controllers_.controller_ ) );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->RegisterAndPublish( actionsModel_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Drop
// Created: FHD 2010-01-04:
// LTO
// -----------------------------------------------------------------------------
bool AgentListView::Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::KnowledgeGroup_ABC& target )
{
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "knowledge_group_update_side_parent" );
    KnowledgeGroupMagicAction* action = new KnowledgeGroupMagicAction( item, actionType, controllers_.controller_, true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    if( const kernel::Team_ABC *team = dynamic_cast< const kernel::Team_ABC* >( &target.Get< kernel::CommunicationHierarchies >().GetTop() ) )
        action->AddParameter( *new parameters::Army( it.NextElement(), *team, controllers_.controller_ ) );
    action->AddParameter( *new parameters::KnowledgeGroup( it.NextElement(), target, controllers_.controller_ ) );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->RegisterAndPublish( actionsModel_ );
    return true;
}
