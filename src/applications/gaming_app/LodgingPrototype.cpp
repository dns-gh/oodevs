// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LodgingPrototype.h"
#include "clients_kernel/Automat_ABC.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LodgingPrototype::LodgingPrototype
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
LodgingPrototype::LodgingPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList )
: LodgingPrototype_ABC( parent )
, attributesList_( attributesList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingPrototype::~LodgingPrototype
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
LodgingPrototype::~LodgingPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingPrototype::Commit
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
void LodgingPrototype::Commit()
{
    if( CheckValidity() )
    {
        actions::parameters::ParameterList& list = attributesList_->AddList( "Lodging" );
        list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_lodging );
        list.AddQuantity( "LodgingCapacity", lodgingCapacity_->value() );
    }
}
