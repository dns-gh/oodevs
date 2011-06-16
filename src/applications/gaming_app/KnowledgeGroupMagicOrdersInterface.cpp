// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// LTO
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "KnowledgeGroupMagicOrdersInterface.h"
#include "moc_KnowledgeGroupMagicOrdersInterface.cpp"
#include "KnowledgeAddInGroupDialog.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Bool.h"
#include "actions/Identifier.h"
#include "actions/KnowledgeGroupMagicAction.h"
#include "actions/MagicAction.h"
#include "actions/String.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "gaming/KnowledgeGroup.h"
#include "gaming/StaticModel.h"
#include "protocol/SimulationSenders.h"


using namespace actions;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface constructor
// Created: SLG 2009-12-16
// -----------------------------------------------------------------------------
KnowledgeGroupMagicOrdersInterface::KnowledgeGroupMagicOrdersInterface( QWidget* parent, Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, const Profile_ABC& profile, const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types )
    : QObject( parent )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , profile_( profile )
    , selectedEntity_( controllers )
    , knowledgeGroupTypes_( types )
    , pAddKnowledgeDialog_ ( 0 )
{
    controllers_.Register( *this );

    pAddKnowledgeDialog_ = new KnowledgeAddInGroupDialog( parent , profile, controllers, simulation, actionsModel, staticModel );
    pAddKnowledgeDialog_->hide();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface destructor
// Created: SLG 2009-12-16
// -----------------------------------------------------------------------------
KnowledgeGroupMagicOrdersInterface::~KnowledgeGroupMagicOrdersInterface()
{
    controllers_.Unregister( *this );
}


// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::NotifyContextMenu
// Created: SLG 2009-12-16
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::NotifyContextMenu( const KnowledgeGroup_ABC& entity, ContextMenu& menu )
{
    if( !profile_.CanDoMagic( entity ) )
        return;
    selectedEntity_ = &entity;
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    const KnowledgeGroup& knowledgeGroup = static_cast< const KnowledgeGroup& >( entity );

    if( knowledgeGroup.IsActivated() )
        AddMagic( tr( "Desactivate" ), SLOT( OnToggleKnowledgeGroupActivation() ), magicMenu );
    else
        AddMagic( tr( "Activate" ), SLOT( OnToggleKnowledgeGroupActivation() ), magicMenu );
    //AddMagic( tr( "Create child KnowledgeGroup" ), SLOT( OnCreateSubKnowledgeGroup() ), magicMenu );  // $$$$ _RC_ SBO 2010-03-05: Not implemented

    AddMagic( tr( "Add knowledge" ), SLOT( OnAddKnowledgeInGroup() ), magicMenu  );

    QPopupMenu* typeMenu = menu.SubMenu( "Type", tr( "Change Type" ) );
    tools::Iterator< const kernel::KnowledgeGroupType& > it = knowledgeGroupTypes_.CreateIterator();
    for( int id = 0; it.HasMoreElements(); ++id )
    {
        const KnowledgeGroupType& type = it.NextElement();
        AddMagicTypeItem( tr( type.GetName().c_str() ), SLOT( OnSetType( int ) ), typeMenu, type, id );
        typeMenu->setItemChecked( id, type.GetName() == knowledgeGroup.GetType() );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::OnToggleKnowledgeGroupActivation
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::OnToggleKnowledgeGroupActivation()
{
    if( selectedEntity_ )
    {
        // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "knowledge_group_enable" );
        KnowledgeGroupMagicAction* action = new KnowledgeGroupMagicAction( *selectedEntity_, actionType, controllers_.controller_, true );
        action->Rename( tools::translate( "gamig_app::Action", "Knowledge Group Activation Change" ) );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new parameters::Bool( it.NextElement(), ! selectedEntity_->IsActivated() ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( selectedEntity_, false ) );
        action->RegisterAndPublish( actionsModel_ );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::OnAddKnowledgeInGroup
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::OnAddKnowledgeInGroup()
{
    if( selectedEntity_ )
    {
        if ( pAddKnowledgeDialog_ )
            pAddKnowledgeDialog_->Show( selectedEntity_ );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::OnSetType
// Created: SLG 2009-12-22
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::OnSetType( int id )
{
    if( selectedEntity_ )
    {
        T_Items::const_iterator it = items_.find( id );
        if( it != items_.end() )
        {
            // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
            MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "knowledge_group_update_type" );
            KnowledgeGroupMagicAction* action = new KnowledgeGroupMagicAction( *selectedEntity_, actionType, controllers_.controller_, true );
            tools::Iterator< const OrderParameter& > paramIt = actionType.CreateIterator();
            action->AddParameter( *new parameters::String( paramIt.NextElement(), it->second->GetName() ) );
            action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
            action->Attach( *new ActionTasker( selectedEntity_, false ) );
            action->RegisterAndPublish( actionsModel_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::CreateSubKnowledgeGroup
// Created: FHD 2009-12-23
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::OnCreateSubKnowledgeGroup()
{
    if( selectedEntity_ )
        if( const kernel::CommunicationHierarchies* hierarchies = selectedEntity_->Retrieve< kernel::CommunicationHierarchies >() )
        {
            // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
            MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "create_knowledge_group" );
            MagicAction* action = new MagicAction( actionType, controllers_.controller_, tr( "Create Knowledge Group").ascii(), true );
            tools::Iterator< const OrderParameter& > paramIt = actionType.CreateIterator();
            action->AddParameter( *new parameters::Identifier( paramIt.NextElement(), hierarchies->GetTop().GetId() ) );
            action->AddParameter( *new parameters::Identifier( paramIt.NextElement(), selectedEntity_->GetId() ) );
            action->AddParameter( *new parameters::String( paramIt.NextElement(), "Standard" ) ); // $$$$ _RC_ SBO 2010-03-04: used kernel::KnowledgeGroupTypes::GetDefault() or something
            action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
            action->Attach( *new ActionTasker( selectedEntity_, false ) );
            action->RegisterAndPublish( actionsModel_ );
        }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::AddMagic
// Created: SLG 2009-12-16
// -----------------------------------------------------------------------------
int KnowledgeGroupMagicOrdersInterface::AddMagic( const QString& label, const char* slot, QPopupMenu* menu )
{
    return menu->insertItem( label, this, slot );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::AddMagicTypeItem
// Created: SLG 2009-12-16
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::AddMagicTypeItem( const QString& label, const char* slot, QPopupMenu* menu, const kernel::KnowledgeGroupType& type, int id )
{
    menu->insertItem( label, this, slot, 0, id );
    items_[ id ] = &type;
}
