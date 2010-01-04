// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "KnowledgeGroupMagicOrdersInterface.h"
#include "moc_KnowledgeGroupMagicOrdersInterface.cpp"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "gaming/KnowledgeGroup.h"
#include "gaming/StaticModel.h"
#include "game_asn/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface constructor
// Created: SLG 2009-12-16
// -----------------------------------------------------------------------------
KnowledgeGroupMagicOrdersInterface::KnowledgeGroupMagicOrdersInterface( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const Profile_ABC& profile, const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types )
    : QObject( parent )
    , controllers_( controllers )
    , publisher_( publisher )
    , profile_( profile )
    , selectedEntity_( controllers )
    , knowledgeGroupTypes_( types )
{    
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
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );

    const KnowledgeGroup& knowledgeGroup = static_cast< const KnowledgeGroup& >( entity );
    AddMagic( tr( "Delete" ), SLOT( OnDeleteKnowledgeGroup() ), magicMenu );

    if( knowledgeGroup.IsActivated() ) 
        AddMagic( tr( "Desactivate" ), SLOT( OnDesactivateKnowledgeGroup() ), magicMenu );  
    else
        AddMagic( tr( "Activate KnowledgeGroup" ), SLOT( OnActivateKnowledgeGroup() ), magicMenu );
    AddMagic( tr( "Create child KnowledgeGroup" ), SLOT( OnCreateSubKnowledgeGroup() ), magicMenu );
    
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
// Name: KnowledgeGroupMagicOrdersInterface::OnActivateKnowledgeGroup
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::OnActivateKnowledgeGroup()
{
    simulation::KnowledgeGroupEnable asnMsg;
    if( selectedEntity_ )
    {
        asnMsg().oid = selectedEntity_->GetId();
        asnMsg().enabled = true;
        asnMsg.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::OnDesactivateKnowledgeGroup
// Created: SLG 2009-12-17
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::OnDesactivateKnowledgeGroup()
{
    simulation::KnowledgeGroupEnable asnMsg;
    if( selectedEntity_ )
    {
        asnMsg().oid = selectedEntity_->GetId();
        asnMsg().enabled = false;
        asnMsg.Send( publisher_ );
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
            simulation::KnowledgeGroupSetType asnMsg;
            asnMsg().oid = selectedEntity_->GetId();
            asnMsg().type = it->second->GetName().c_str();
            asnMsg.Send( publisher_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::OnDeleteKnowledgeGroup
// Created: SLG 2009-12-22
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::OnDeleteKnowledgeGroup()
{
    simulation::KnowledgeGroupDelete asnMsg;
    if( selectedEntity_ )
    {
        asnMsg().oid = selectedEntity_->GetId();
        asnMsg.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::CreateSubKnowledgeGroup
// Created: FHD 2009-12-23
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::OnCreateSubKnowledgeGroup()
{
    simulation::KnowledgeGroupCreation asnMsg;
    if( selectedEntity_ )
    {
        asnMsg().type = "Standard";
        if( const kernel::CommunicationHierarchies* hierarchies = selectedEntity_->Retrieve< kernel::CommunicationHierarchies >() )
            asnMsg().oid_camp = hierarchies->GetTop().GetId();
        asnMsg().oid_knowledgegroup_parent = selectedEntity_->GetId();
        asnMsg.Send( publisher_ );
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
