// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "SupplyRoutePrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/LoadableSpinBox.h"
#include "clients_gui/LoadableCheckBox.h"
#include "preparation/SupplyRouteAttribute.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
SupplyRoutePrototype::SupplyRoutePrototype( QWidget* parent, Object_ABC*& creation )
    : SupplyRoutePrototype_ABC( parent )
    , creation_( creation )
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
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        SupplyRouteAttribute* attribute = new SupplyRouteAttribute( dico );
        {
            attribute->SetFlow( flow_->value() );
            attribute->SetWidth( width_->value() );
            attribute->SetLength( length_->value() );
            attribute->SetMaxWeight( maxWeight_->value() );
            attribute->SetEquipped( equipped_->isChecked() );
        }
        creation_->Attach( *attribute );
    }
}
