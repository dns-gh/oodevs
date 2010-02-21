// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SupplyRoutePrototype.h"
#include "protocol/ClientSenders.h"
#include "protocol/Protocol.h"

using namespace MsgsClientToSim;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
SupplyRoutePrototype::SupplyRoutePrototype( QWidget* parent, MsgMagicActionCreateObject& msg )
    : SupplyRoutePrototype_ABC( parent )
    , msg_  ( msg )
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
    msg_.mutable_attributes()->mutable_supply_route()->set_flow_rate( flow_->value() );
    msg_.mutable_attributes()->mutable_supply_route()->set_width( width_->value() );
    msg_.mutable_attributes()->mutable_supply_route()->set_length( length_->value() );
    msg_.mutable_attributes()->mutable_supply_route()->set_max_weight( maxWeight_->value() );
    msg_.mutable_attributes()->mutable_supply_route()->set_equipped( equipped_->isOn() );
}

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void SupplyRoutePrototype::Clean()
{
}
