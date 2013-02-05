// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "CommunicationTreeView.h"
#include "moc_CommunicationTreeView.cpp"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Army.h"
#include "actions/Automat.h"
#include "actions/UnitMagicAction.h"
#include "actions/KnowledgeGroup.h"
#include "actions/KnowledgeGroupMagicAction.h"
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_gui/ItemPixmapDelegate.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatDecisions_ABC.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Options.h"
#include "gaming/Attributes.h"
#include "gaming/StaticModel.h"
#include "icons.h"
#include <boost/bind.hpp>

namespace
{
    bool IsCommandPost( const kernel::Entity_ABC& entity )
    {
        if( const kernel::CommandPostAttributes_ABC* pAttributes = entity.Retrieve< kernel::CommandPostAttributes_ABC >() )
            return pAttributes->IsCommandPost();
        return false;
    }

    bool IsEngaged( const kernel::Entity_ABC& entity )
    {
        if( const kernel::AutomatDecisions_ABC* decisions = entity.Retrieve< kernel::AutomatDecisions_ABC >() )
            return decisions->IsEmbraye();
        return false;
    }

    bool IsKgDeactivated( const kernel::Entity_ABC& entity )
    {
        if( const kernel::KnowledgeGroup_ABC* kg = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &entity ) )
            return !kg->IsActivated();
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView constructor
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
CommunicationTreeView::CommunicationTreeView( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, const gui::EntitySymbols& symbols, const StaticModel& staticModel, const kernel::Time_ABC& simulation, actions::ActionsModel& actionsModel, QWidget* parent /*= 0*/ )
    : gui::HierarchyTreeView< kernel::CommunicationHierarchies >( controllers, profile, modelObserver, symbols, parent )
    , static_( staticModel )
    , simulation_( simulation )
    , actionsModel_( actionsModel )
    , changeSuperiorDialog_( 0 )
    , lock_( MAKE_PIXMAP( lock ) )
    , scisors_( MAKE_PIXMAP( scisors ) ) // LTO
    , commandPost_( MAKE_PIXMAP( commandpost ) )
{
    setItemDelegate( new gui::ItemPixmapDelegate( dataModel_, boost::bind( &CommunicationTreeView::GetEntityPixmap, this, _1 ), this ) );
    setEditTriggers( 0 );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView destructor
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
CommunicationTreeView::~CommunicationTreeView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::GetEntityPixmap
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
std::vector< const QPixmap* > CommunicationTreeView::GetEntityPixmap( const kernel::Entity_ABC& entity )
{
    std::vector< const QPixmap* > ret;
    if( IsCommandPost( entity ) )
        ret.push_back( &commandPost_ );
    if( IsEngaged( entity ) )
        ret.push_back( &lock_ );
    else if( IsKgDeactivated( entity ) )
        ret.push_back( &scisors_ );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::LessThan
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
bool CommunicationTreeView::LessThan( const QModelIndex& left, const QModelIndex& right, bool& valid ) const
{
    const kernel::Entity_ABC* entity1 = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( left );
    const kernel::Entity_ABC* entity2 = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( right );
    if( !entity1 || !entity2 )
        return false;
    valid = true;
    if( IsCommandPost( *entity1 ) )
        return false;
    if( IsCommandPost( *entity2 ) )
        return true;
    return entity2->GetId() < entity1->GetId();
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::CanChangeSuperior
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
bool CommunicationTreeView::CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const
{
    const kernel::Agent_ABC*          agent   = dynamic_cast< const kernel::Agent_ABC* >         ( &entity );
    const kernel::Automat_ABC*        automat = dynamic_cast< const kernel::Automat_ABC* >       ( &entity );
    const kernel::KnowledgeGroup_ABC* group   = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &superior );
    if( automat && group )
        return &entity.Get< kernel::CommunicationHierarchies >().GetTop() == &superior.Get< kernel::CommunicationHierarchies >().GetTop();
    else if( const kernel::KnowledgeGroup_ABC* knowledgegroup = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &entity ) )
    {
        const kernel::Entity_ABC* com = &knowledgegroup->Get< kernel::CommunicationHierarchies >().GetTop();
        const kernel::Entity_ABC* team = dynamic_cast< const kernel::Entity_ABC* >( &superior );
        if( com && com == team )
            return true;
        else if( group && ( knowledgegroup != group ) )
            return com == &superior.Get< kernel::CommunicationHierarchies >().GetTop();
    }
    else if( agent )
    {
        const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &superior );
        if( !automat )
            automat = dynamic_cast< const kernel::Automat_ABC* >( &superior.Get< kernel::CommunicationHierarchies >().GetUp() );
        if( automat != &agent->Get< kernel::CommunicationHierarchies >().GetUp() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::DoChangeSuperior
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
void CommunicationTreeView::DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior )
{
    kernel::Automat_ABC* automat = dynamic_cast< kernel::Automat_ABC* >( &entity );
    kernel::KnowledgeGroup_ABC* kg = dynamic_cast< kernel::KnowledgeGroup_ABC* >( &entity );
    if( automat )
        Drop( *automat, superior );
    else if( kg )
        Drop( *kg, superior );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::OnChangeKnowledgeGroup
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
void CommunicationTreeView::OnChangeKnowledgeGroup()
{
    QModelIndexList list = selectionModel()->selectedIndexes();
    if( list.size() == 1)
    {
        kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( list.front() );
        if( entity )
        {
            if( !changeSuperiorDialog_ )
                changeSuperiorDialog_ = new gui::ChangeSuperiorDialog( this, controllers_, *this, true );
            changeSuperiorDialog_->Show( *entity );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::drawRow
// Created: JSR 2012-10-10
// -----------------------------------------------------------------------------
void CommunicationTreeView::drawRow( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex &index ) const
{
    QStandardItem* item = dataModel_.GetItemFromIndex( dataModel_.GetMainModelIndex( index ) );
    if( item && item->data( gui::Roles::MimeTypeRole).isValid() &&
        item->data( gui::Roles::MimeTypeRole).toString() == typeid( kernel::Agent_ABC ).name() )
    {
        if( const kernel::Agent_ABC* agent = dataModel_.GetDataFromItem< const kernel::Agent_ABC >( *item ) )
        {
            if( const Attributes* attributes = static_cast< const Attributes* >( agent->Retrieve< kernel::Attributes_ABC >() ) )
            {
                if( attributes->nOpState_ == eOperationalStatus_DetruitTotalement )
                    painter->fillRect( options.rect, QColor( controllers_.options_.GetOption( "Color/TotallyDestroyed", QString( "" ) ).To< QString >() ) );
                else if( attributes->nOpState_ == eOperationalStatus_DetruitTactiquement )
                    painter->fillRect( options.rect, QColor( controllers_.options_.GetOption( "Color/TacticallyDestroyed", QString( "" ) ).To< QString >() ) );
                if( attributes->bNeutralized_ )
                    painter->fillRect( options.rect, QColor( controllers_.options_.GetOption( "Color/Neutralized", QString( "" ) ).To< QString >() ) );
            }
        }
    }
    gui::HierarchyTreeView< kernel::CommunicationHierarchies >::drawRow( painter, options, index );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyCreated
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyCreated( const kernel::Entity_ABC& entity )
{
    gui::HierarchyTreeView< kernel::CommunicationHierarchies >::ElementObserver_ABC< kernel::Entity_ABC >::NotifyCreated( entity );
    UpdateLongName( entity );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyUpdated
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    gui::HierarchyTreeView< kernel::CommunicationHierarchies >::ElementObserver_ABC< kernel::Entity_ABC >::NotifyUpdated( entity );
    UpdateLongName( entity );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::UpdateLongName
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
void CommunicationTreeView::UpdateLongName( const kernel::Entity_ABC& entity )
{
    QStandardItem* item = dataModel_.FindDataItem( entity );
    if( item )
        item->setEditable( !gui::longname::SetItemLongName( entity, *item ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyContextMenu
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyContextMenu( const kernel::Automat_ABC& /*automat*/, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Command", tr( "Change knowledge group" ), this, SLOT( OnChangeKnowledgeGroup() ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyContextMenu
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyContextMenu( const kernel::KnowledgeGroup_ABC& /*group*/, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Command", tr( "Change knowledge group" ), this, SLOT( OnChangeKnowledgeGroup() ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::Drop
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
void CommunicationTreeView::Drop( const kernel::Agent_ABC& item, const kernel::Entity_ABC& target )
{
    if( const kernel::Agent_ABC* agent = dynamic_cast< const kernel::Agent_ABC* >( &target ) )
        Drop( item, target.Get< kernel::CommunicationHierarchies >().GetUp() );
    else if( const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &target ) )
    {
        if( &item.Get< kernel::CommunicationHierarchies >().GetUp() != automat )
        {
            kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "unit_change_superior" );
            actions::UnitMagicAction* action = new actions::UnitMagicAction( item, actionType, controllers_.controller_,  tr( "Unit Change Superior" ), true );
            tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
            action->AddParameter( *new actions::parameters::Automat( it.NextElement(), *automat, controllers_.controller_ ) );
            action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
            action->Attach( *new actions::ActionTasker( &item, false ) );
            action->RegisterAndPublish( actionsModel_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::Drop
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
void CommunicationTreeView::Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target )
{
    if( const kernel::KnowledgeGroup_ABC* group = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &target ) )
    {
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "change_knowledge_group" );
        actions::UnitMagicAction* action = new actions::UnitMagicAction( item, actionType, controllers_.controller_, tr( "Change Knowledge Group" ), true );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new actions::parameters::KnowledgeGroup( it.NextElement(), *group, controllers_.controller_ ) );
        if( const kernel::Team_ABC *team = dynamic_cast< const kernel::Team_ABC* >( &group->Get< kernel::CommunicationHierarchies >().GetTop() ) )
            action->AddParameter( *new actions::parameters::Army( it.NextElement(), *team, controllers_.controller_ ) );
        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new actions::ActionTasker( &item, false ) );
        action->RegisterAndPublish( actionsModel_ );
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::Drop
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
void CommunicationTreeView::Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Entity_ABC& target )
{
    actions::KnowledgeGroupMagicAction* action = 0;
    if( const kernel::KnowledgeGroup_ABC* groupParent = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &target ) )
    {
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "knowledge_group_update_side_parent" );
        action = new actions::KnowledgeGroupMagicAction( item, actionType, controllers_.controller_, true );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        if( const kernel::Team_ABC *team = dynamic_cast< const kernel::Team_ABC* >( &groupParent->Get< kernel::CommunicationHierarchies >().GetTop() ) )
            action->AddParameter( *new actions::parameters::Army( it.NextElement(), *team, controllers_.controller_ ) );
        action->AddParameter( *new actions::parameters::KnowledgeGroup( it.NextElement(), *groupParent, controllers_.controller_ ) );
    }
    else if( const kernel::Team_ABC* teamParent = dynamic_cast< const kernel::Team_ABC* >( &target ) )
    {
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "knowledge_group_update_side" );
        action = new actions::KnowledgeGroupMagicAction( item, actionType, controllers_.controller_, true );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        if( const kernel::Team_ABC *team = dynamic_cast< const kernel::Team_ABC* >( &teamParent->Get< kernel::CommunicationHierarchies >().GetTop() ) )
            action->AddParameter( *new actions::parameters::Army( it.NextElement(), *team, controllers_.controller_ ) );
    }
    if( action )
    {
        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
        action->RegisterAndPublish( actionsModel_ );
    }
}
