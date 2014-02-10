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
#include "actions/ActionsModel.h"
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
#include "clients_gui/SignalAdapter.h"
#include "gaming/KnowledgeGroup.h"
#include "gaming/StaticModel.h"
#include "protocol/SimulationSenders.h"

using namespace actions;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface constructor
// Created: SLG 2009-12-16
// -----------------------------------------------------------------------------
KnowledgeGroupMagicOrdersInterface::KnowledgeGroupMagicOrdersInterface( QWidget* parent, Controllers& controllers, actions::ActionsModel& actionsModel,
                                                                        const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, const Profile_ABC& profile,
                                                                        const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types )
    : QObject( parent )
    , controllers_        ( controllers )
    , actionsModel_       ( actionsModel )
    , static_             ( staticModel )
    , simulation_         ( simulation )
    , profile_            ( profile )
    , selectedEntity_     ( controllers )
    , types_              ( types )
    , pAddKnowledgeDialog_( new KnowledgeAddInGroupDialog( parent, controllers, simulation, actionsModel, staticModel ) )
{
    pAddKnowledgeDialog_->hide();
    controllers_.Register( *this );
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
    kernel::ContextMenu* magicMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ), false, 1 );
    if( entity.IsActivated() )
        magicMenu->insertItem( tr( "Desactivate" ), this, SLOT( OnToggleKnowledgeGroupActivation() ) );
    else
        magicMenu->insertItem( tr( "Activate" ), this, SLOT( OnToggleKnowledgeGroupActivation() ) );
    kernel::ContextMenu* createKnowledgeGroup = menu.SubMenu( "Knowledge", tr( "Create Knowledge Group", false, 1 ) );
    for( auto it = types_.CreateIterator(); it.HasMoreElements(); )
    {
        const KnowledgeGroupType& type = it.NextElement();
        QAction* action = createKnowledgeGroup->addAction( QString::fromStdString( type.GetName() ) );
        gui::connect( action, SIGNAL( triggered() ),
            boost::bind( &KnowledgeGroupMagicOrdersInterface::OnCreateKnowledgeGroup, this, kernel::SafePointer< kernel::Entity_ABC >( controllers_, &entity ), type.GetName() ) );
    }
    magicMenu->insertItem( tr( "Add knowledge" ), this, SLOT( OnAddKnowledgeInGroup() ) );
    kernel::ContextMenu* typeMenu = menu.SubMenu( "Knowledge", tr( "Change Type" ) );
    for( auto it = types_.CreateIterator(); it.HasMoreElements(); )
    {
        const KnowledgeGroupType& type = it.NextElement();
        int id = typeMenu->insertItem( QString::fromStdString( type.GetName() ), this, SLOT( OnSetType() ) );
        typeMenu->setItemChecked( id, type.GetName() == static_cast< const KnowledgeGroup& >( entity ).GetType() );
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
        std::unique_ptr< Action_ABC > action( new KnowledgeGroupMagicAction( actionType, controllers_.controller_, true ) );
        action->Rename( tools::translate( "gamig_app::Action", "Knowledge Group Activation Change" ) );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new parameters::Bool( it.NextElement(), ! selectedEntity_->IsActivated() ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
        actionsModel_.Publish( *action, 0 );
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
        if( pAddKnowledgeDialog_ )
            pAddKnowledgeDialog_->Show( selectedEntity_ );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::OnSetType
// Created: SLG 2009-12-22
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::OnSetType()
{
    if( selectedEntity_ )
    {
        const QAction& s = dynamic_cast< QAction& >( *sender() );
        auto& type = types_.Get( s.text().toStdString() );
        // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "knowledge_group_update_type" );
        std::unique_ptr< Action_ABC > action( new KnowledgeGroupMagicAction( actionType, controllers_.controller_, true ) );
        tools::Iterator< const OrderParameter& > paramIt = actionType.CreateIterator();
        action->AddParameter( *new parameters::String( paramIt.NextElement(), type.GetName() ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
        actionsModel_.Publish( *action, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::CreateSubKnowledgeGroup
// Created: FHD 2009-12-23
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::OnCreateKnowledgeGroup( const kernel::SafePointer< kernel::Entity_ABC >& entity, const std::string& type )
{
    if( entity )
        actionsModel_.PublishCreateKnowledgeGroup( entity->GetId(), type );
}
