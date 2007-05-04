// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectMagicOrdersInterface.h"
#include "moc_ObjectMagicOrdersInterface.cpp"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "gaming/ASN_Messages.h"
#include "gaming/Object.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
ObjectMagicOrdersInterface::ObjectMagicOrdersInterface( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const Profile_ABC& profile )
    : QObject( parent )
    , controllers_( controllers )
    , publisher_( publisher )
    , profile_( profile )
    , selectedEntity_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
ObjectMagicOrdersInterface::~ObjectMagicOrdersInterface()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::NotifyContextMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::NotifyContextMenu( const Object_ABC& entity, ContextMenu& menu )
{
    if( !profile_.CanDoMagic( entity ) )
        return;
    selectedEntity_ = &entity;
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    AddMagic( tr( "Build" ), SLOT( BuildObject() ), magicMenu );
    AddMagic( tr( "Destroy" ), SLOT( DestroyObject() ), magicMenu );
    if( entity.GetType().CanBeValorized() )
    {
        AddMagic( tr( "Mine" ), SLOT( MineObject() ), magicMenu );
        AddMagic( tr( "Sweep mines" ), SLOT( SweepMineObject() ), magicMenu );
    }
    if( entity.GetType().CanBePrepared() )
    {
        const Object& obj = static_cast< const Object& >( entity );
        if( obj.IsPrepared() )
            AddMagic( tr( "Set demolition reserved" ), SLOT( SetDemolitionReservedObject() ), magicMenu );
        else
            AddMagic( tr( "Set preliminary" ), SLOT( SetPreliminaryObject() ), magicMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::SendObjectMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::SendObjectMagic( ASN1T_MagicActionUpdateObject& asn )
{
    asn.oid_objet = selectedEntity_->GetId();
    ASN_MsgObjectMagicAction asnMsg;
    asnMsg().action.t               = T_MsgObjectMagicAction_action_update_object;
    asnMsg().action.u.update_object = &asn;
    asnMsg.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::BuildObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::BuildObject()
{
    ASN1T_MagicActionUpdateObject asn;
    asn.m.pourcentage_constructionPresent = 1;
    asn.pourcentage_construction          = 100;
    SendObjectMagic( asn );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::DestroyObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::DestroyObject()
{
    ASN1T_MagicActionUpdateObject asn;
    asn.m.pourcentage_constructionPresent  = 1;
    asn.pourcentage_construction           = 0;
    SendObjectMagic( asn );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::MineObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::MineObject()
{
    ASN1T_MagicActionUpdateObject asn;
    asn.m.pourcentage_valorisationPresent  = 1;
    asn.pourcentage_valorisation           = 100;
    SendObjectMagic( asn );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::SweepMineObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::SweepMineObject()
{
    ASN1T_MagicActionUpdateObject asn;
    asn.m.pourcentage_valorisationPresent  = 1;
    asn.pourcentage_valorisation           = 0;
    SendObjectMagic( asn );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::SetDemolitionReservedObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::SetDemolitionReservedObject()
{
    ASN1T_MagicActionUpdateObject asn;
    asn.m.en_preparationPresent = 1;
    asn.en_preparation          = 0;
    SendObjectMagic( asn );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::SetPreliminaryObject
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ObjectMagicOrdersInterface::SetPreliminaryObject()
{
    ASN1T_MagicActionUpdateObject asn;
    asn.m.en_preparationPresent = 1;
    asn.en_preparation          = 1;
    SendObjectMagic( asn );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicOrdersInterface::AddMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
int ObjectMagicOrdersInterface::AddMagic( const QString& label, const char* slot, QPopupMenu* menu )
{
    return menu->insertItem( label, this, slot );
}
