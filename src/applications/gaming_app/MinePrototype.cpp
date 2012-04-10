// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MinePrototype.h"
#include "clients_gui/LoadableSpinBox.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: MinePrototype constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MinePrototype::MinePrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList )
    : MinePrototype_ABC( parent )
    , attributesList_( attributesList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MinePrototype destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MinePrototype::~MinePrototype()
{
}

// -----------------------------------------------------------------------------
// Name: MinePrototype::Commit
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MinePrototype::Commit()
{
    actions::parameters::ParameterList& list = attributesList_->AddList( "mine" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_mine );
    list.AddQuantity( "density", density_->value() );
}
