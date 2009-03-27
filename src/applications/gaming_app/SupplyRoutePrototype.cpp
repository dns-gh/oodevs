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
#include "game_asn/SimulationSenders.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
SupplyRoutePrototype::SupplyRoutePrototype( QWidget* parent, ASN1T_MagicActionCreateObject& msg )
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
    msg_.attributes.m.supply_routePresent = 1;
    msg_.attributes.supply_route.flow_rate = flow_->value();
    msg_.attributes.supply_route.width = width_->value();
    msg_.attributes.supply_route.length = length_->value();
    msg_.attributes.supply_route.max_weight = maxWeight_->value();
    msg_.attributes.supply_route.equipped = equipped_->isOn();
}

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void SupplyRoutePrototype::Clean()
{
    msg_.attributes.m.supply_routePresent = 0;
}
