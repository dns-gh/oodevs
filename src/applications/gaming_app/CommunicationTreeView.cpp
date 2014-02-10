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
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Army.h"
#include "actions/Automat.h"
#include "actions/Identifier.h"
#include "actions/MagicAction.h"
#include "actions/KnowledgeGroup.h"
#include "actions/KnowledgeGroupMagicAction.h"
#include "actions/String.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_gui/ItemPixmapDelegate.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_gui/SignalAdapter.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/KnowledgeGroupType.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "gaming/Attributes.h"
#include "gaming/StaticModel.h"
#include "icons.h"
#include <boost/bind.hpp>

namespace
{
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
CommunicationTreeView::CommunicationTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile,
                                              gui::ModelObserver_ABC& modelObserver, const gui::EntitySymbols& symbols, const StaticModel& staticModel,
                                              const kernel::Time_ABC& simulation, actions::ActionsModel& actionsModel, QWidget* parent /*= 0*/ )
    : gui::HierarchyTreeView< kernel::CommunicationHierarchies >( objectName, controllers, profile, modelObserver, symbols, parent )
    , profile_             ( profile )
    , static_              ( staticModel )
    , simulation_          ( simulation )
    , actionsModel_        ( actionsModel )
    , changeSuperiorDialog_( 0 )
    , lock_                ( MAKE_PIXMAP( lock ) )
    , scisors_             ( MAKE_PIXMAP( scisors ) ) // LTO
    , commandPost_         ( MAKE_PIXMAP( commandpost ) )
{
    setItemDelegate( new gui::ItemPixmapDelegate( dataModel_, boost::bind( &CommunicationTreeView::GetEntityPixmap, this, _1 ), this ) );
    setEditTriggers( 0 );
    SetLessThanEntityFunctor( &tools::LessThanByPC );
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
    if( tools::IsCommandPost( entity ) )
        ret.push_back( &commandPost_ );
    if( tools::IsEngaged( entity ) )
        ret.push_back( &lock_ );
    else if( IsKgDeactivated( entity ) )
        ret.push_back( &scisors_ );
    return ret;
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
    if( !index.isValid() )
        return;
    QStandardItem* item = dataModel_.GetItemFromIndex( dataModel_.GetMainModelIndex( index ) );
    if( item && item->data( gui::Roles::MimeTypeRole).isValid() &&
        item->data( gui::Roles::MimeTypeRole).toString() == typeid( kernel::Agent_ABC ).name() )
    {
        if( const kernel::Agent_ABC* agent = dataModel_.GetDataFromItem< const kernel::Agent_ABC >( *item ) )
        {
            if( const Attributes* attributes = agent->Retrieve< Attributes >() )
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
// Name: CommunicationTreeView::ApplyProfileFilter
// Created: JSR 2013-07-11
// -----------------------------------------------------------------------------
bool CommunicationTreeView::ApplyProfileFilter( QStandardItem& item, gui::StandardModel& model ) const
{
    if( item.data( gui::Roles::SafeRole ).isValid() && item.data( gui::Roles::SafeRole ).toBool() )
    {
        const kernel::Entity_ABC* entity = dataModel_.GetDataFromItem< kernel::Entity_ABC >( item );
        const kernel::KnowledgeGroup_ABC* kg = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( entity );
        if( kg && kg->IsCrowd() )
            return false;
        const kernel::Population_ABC* population = dynamic_cast< const kernel::Population_ABC* >( entity );
        if( population )
            return false;
    }
    return gui::HierarchyTreeView< kernel::CommunicationHierarchies >::ApplyProfileFilter( item, model );
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
    if( controllers_.GetCurrentMode() != eModes_Replay )
        menu.InsertItem( "Knowledge", tr( "Change superior" ), this, SLOT( OnChangeKnowledgeGroup() ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyContextMenu
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyContextMenu( const kernel::KnowledgeGroup_ABC& /*group*/, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    if( controllers_.GetCurrentMode() != eModes_Replay )
        menu.InsertItem( "Knowledge", tr( "Change superior" ), this, SLOT( OnChangeKnowledgeGroup() ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyContextMenu
// Created: SLI 2013-07-17
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    if( !profile_.CanDoMagic( team ) )
        return;
    kernel::ContextMenu* createKnowledgeGroup = menu.SubMenu( "Knowledge", tr( "Create Knowledge Group", false, 1 ) );
    const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types = static_.types_;
    for( auto it = types.CreateIterator(); it.HasMoreElements(); )
    {
        const kernel::KnowledgeGroupType& type = it.NextElement();
        QAction* action = createKnowledgeGroup->addAction( QString::fromStdString( type.GetName() ) );
        kernel::SafePointer< kernel::Entity_ABC > entity( controllers_, &team );
        gui::connect( action, SIGNAL( triggered() ), boost::bind( &CommunicationTreeView::OnCreateKnowledgeGroup, this, entity, type.GetName() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::OnCreateKnowledgeGroup
// Created: SLI 2013-07-17
// -----------------------------------------------------------------------------
void CommunicationTreeView::OnCreateKnowledgeGroup( const kernel::SafePointer< kernel::Entity_ABC >& entity, const std::string& type )
{
    if( entity )
        actionsModel_.PublishCreateKnowledgeGroup( entity->GetId(), type );
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
            std::unique_ptr< actions::Action_ABC > action( new actions::UnitMagicAction( actionType, controllers_.controller_, false ) );
            tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
            action->AddParameter( *new actions::parameters::Automat( it.NextElement(), *automat, controllers_.controller_ ) );
            action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
            action->Attach( *new actions::ActionTasker( controllers_.controller_, &item, false ) );
            actionsModel_.Publish( *action, 0 );
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
        std::unique_ptr< actions::Action_ABC > action( new actions::UnitMagicAction( actionType, controllers_.controller_, false ) );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new actions::parameters::KnowledgeGroup( it.NextElement(), *group, controllers_.controller_ ) );
        if( const kernel::Team_ABC *team = dynamic_cast< const kernel::Team_ABC* >( &group->Get< kernel::CommunicationHierarchies >().GetTop() ) )
            action->AddParameter( *new actions::parameters::Army( it.NextElement(), *team, controllers_.controller_ ) );
        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new actions::ActionTasker( controllers_.controller_, &item, false ) );
        actionsModel_.Publish( *action, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::Drop
// Created: JSR 2012-09-28
// -----------------------------------------------------------------------------
void CommunicationTreeView::Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Entity_ABC& target )
{
    std::unique_ptr< actions::Action_ABC > action;
    if( const kernel::KnowledgeGroup_ABC* groupParent = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &target ) )
    {
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "knowledge_group_update_side_parent" );
        action.reset( new actions::KnowledgeGroupMagicAction( actionType, controllers_.controller_, false ) );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        if( const kernel::Team_ABC *team = dynamic_cast< const kernel::Team_ABC* >( &groupParent->Get< kernel::CommunicationHierarchies >().GetTop() ) )
            action->AddParameter( *new actions::parameters::Army( it.NextElement(), *team, controllers_.controller_ ) );
        action->AddParameter( *new actions::parameters::KnowledgeGroup( it.NextElement(), *groupParent, controllers_.controller_ ) );
    }
    else if( const kernel::Team_ABC* teamParent = dynamic_cast< const kernel::Team_ABC* >( &target ) )
    {
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "knowledge_group_update_side" );
        action.reset( new actions::KnowledgeGroupMagicAction( actionType, controllers_.controller_, false ) );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        if( const kernel::Team_ABC *team = dynamic_cast< const kernel::Team_ABC* >( &teamParent->Get< kernel::CommunicationHierarchies >().GetTop() ) )
            action->AddParameter( *new actions::parameters::Army( it.NextElement(), *team, controllers_.controller_ ) );
    }
    if( action )
    {
        action->Attach( *new actions::ActionTasker( controllers_.controller_, &item, false ) );
        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
        actionsModel_.Publish( *action, 0 );
    }
}
