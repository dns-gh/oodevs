// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FloodPrototype.h"
#include "actions/ParameterList.h"
#include "protocol/Protocol.h"
#include "clients_gui/LoadableSpinBox.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: FloodPrototype constructor
// Created: JSR 2011-01-03
// -----------------------------------------------------------------------------
FloodPrototype::FloodPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList )
    : FloodPrototype_ABC( parent )
    , attributesList_   ( attributesList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodPrototype destructor
// Created: JSR 2011-01-03
// -----------------------------------------------------------------------------
FloodPrototype::~FloodPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodPrototype::Commit
// Created: JSR 2011-01-03
// -----------------------------------------------------------------------------
void FloodPrototype::Commit()
{
    actions::parameters::ParameterList& list = attributesList_->AddList( "Flood" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::flood );
    list.AddQuantity( "Depth", depth_->value() );
    list.AddQuantity( "RefDist", refDist_->value() );
}
