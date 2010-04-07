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

#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "gaming/KnowledgeGroup.h"
#include "gaming/StaticModel.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/SimulationSenders.h"

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

    if( knowledgeGroup.IsActivated() ) 
        AddMagic( tr( "Desactivate" ), SLOT( OnToggleKnowledgeGroupActivation() ), magicMenu );  
    else
        AddMagic( tr( "Activate" ), SLOT( OnToggleKnowledgeGroupActivation() ), magicMenu );
    //AddMagic( tr( "Create child KnowledgeGroup" ), SLOT( OnCreateSubKnowledgeGroup() ), magicMenu );  // $$$$ _RC_ SBO 2010-03-05: Not implemented
    
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
        simulation::KnowledgeGroupUpdateRequest message;
        message().set_oid( selectedEntity_->GetId() );
        message().set_enabled( ! selectedEntity_->IsActivated() );
        message.Send( publisher_ );
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
            simulation::KnowledgeGroupUpdateRequest message;
            message().set_oid( selectedEntity_->GetId() );
            message().set_type( it->second->GetName() );
            message.Send( publisher_ );
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
            simulation::KnowledgeGroupCreationRequest message;
            message().set_type( "Standard" ); // $$$$ _RC_ SBO 2010-03-04: used kernel::KnowledgeGroupTypes::GetDefault() or something
            message().set_oid_camp( hierarchies->GetTop().GetId() );
            message().set_oid_parent( selectedEntity_->GetId() );
            message.Send( publisher_ );
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
