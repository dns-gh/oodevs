// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SupplyRoutePrototype.h"
#include "protocol/ClientSenders.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"

using namespace sword;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
SupplyRoutePrototype::SupplyRoutePrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList )
    : SupplyRoutePrototype_ABC( parent )
    , attributesList_( attributesList )
    , attr_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
SupplyRoutePrototype::~SupplyRoutePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void SupplyRoutePrototype::Commit()
{
    actions::parameters::ParameterList& list = attributesList_->AddList( "SupplyRoute" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_supply_route );
    list.AddBool( "Equipped", equipped_->isOn() );
    list.AddQuantity( "MaxWeight", maxWeight_->value() );
    list.AddQuantity( "Width", width_->value() );
    list.AddQuantity( "Length", length_->value() );
    list.AddQuantity( "FlowRate", flow_->value() );
}
