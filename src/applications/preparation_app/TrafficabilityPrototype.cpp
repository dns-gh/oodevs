// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "TrafficabilityPrototype.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_gui/LoadableLineEdit.h"
#include "preparation/TrafficabilityAttribute.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: TrafficabilityPrototype constructor
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
TrafficabilityPrototype::TrafficabilityPrototype( QWidget* parent, Object_ABC*& creation, double maxWeight )
    : TrafficabilityPrototype_ABC( parent, maxWeight )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityPrototype destructor
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
TrafficabilityPrototype::~TrafficabilityPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityPrototype::Commit
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
void TrafficabilityPrototype::Commit()
{
    if( creation_ )
    {
        PropertiesDictionary& dictionary = creation_->Get< PropertiesDictionary >();
        TrafficabilityAttribute* attribute = new TrafficabilityAttribute( dictionary, maxWeight_->text().toDouble() );
        creation_->Attach( *attribute );
    }
}
