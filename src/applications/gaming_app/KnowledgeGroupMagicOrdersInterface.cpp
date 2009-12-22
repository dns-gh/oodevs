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
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "gaming/KnowledgeGroup.h"
#include "game_asn/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface constructor
// Created: SLG 2009-12-16
// -----------------------------------------------------------------------------
KnowledgeGroupMagicOrdersInterface::KnowledgeGroupMagicOrdersInterface( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const Profile_ABC& profile )
: QObject( parent )
, controllers_( controllers )
, publisher_( publisher )
, profile_( profile )
, selectedEntity_( controllers )
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
        AddMagic( tr( "Desactivate KnowledgeGroup" ), SLOT( DesactivateKnowledgeGroup() ), magicMenu );  
    else
        AddMagic( tr( "Activate KnowledgeGroup" ), SLOT( ActivateKnowledgeGroup() ), magicMenu );
    AddMagic( tr( "Delete KnowledgeGroup" ), SLOT( DeleteKnowledgeGroup() ), magicMenu );
    QPopupMenu* typeMenu = menu.SubMenu( "Type", tr( "Change Type" ) );
    AddMagic( tr( "toto" ), SLOT( ChangeTypeKnowledgeGroup() ), typeMenu );
    AddMagic( tr( "tutu" ), SLOT( ChangeTypeKnowledgeGroup() ), typeMenu );
    AddMagic( tr( "titi" ), SLOT( ChangeTypeKnowledgeGroup() ), typeMenu );
}



// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::ActivateKnowledgeGroup
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::ActivateKnowledgeGroup()
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
// Name: KnowledgeGroupMagicOrdersInterface::DesactivateKnowledgeGroup
// Created: SLG 2009-12-17
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::DesactivateKnowledgeGroup()
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
// Name: KnowledgeGroupMagicOrdersInterface::DeleteKnowledgeGroup
// Created: FHD 2009-12-22
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::DeleteKnowledgeGroup()
{
    simulation::KnowledgeGroupDelete asnMsg;
    if( selectedEntity_ )
    {
        asnMsg().oid = selectedEntity_->GetId();
        asnMsg.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicOrdersInterface::ChangeTypeKnowledgeGroup
// Created: FHD 2009-12-22
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicOrdersInterface::ChangeTypeKnowledgeGroup()
{
    simulation::KnowledgeGroupSetType asnMsg;
    if( selectedEntity_ )
    {
        asnMsg().oid = selectedEntity_->GetId();
        asnMsg().type = 0;
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
