// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UndergroundPrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/LoadableComboBox.h"
#include "preparation/UndergroundAttribute.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UndergroundPrototype constructor
// Created: JSR 2011-07-08
// -----------------------------------------------------------------------------
UndergroundPrototype::UndergroundPrototype( QWidget* parent, Controller& controller, Object_ABC*& creation )
    : UndergroundPrototype_ABC( parent, controller )
    , controller_( controller )
    , creation_  ( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundPrototype destructor
// Created: JSR 2011-07-08
// -----------------------------------------------------------------------------
UndergroundPrototype::~UndergroundPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundPrototype::Commit
// Created: JSR 2011-07-08
// -----------------------------------------------------------------------------
void UndergroundPrototype::Commit( const kernel::Team_ABC& )
{
    if( creation_ )
    {
        PropertiesDictionary& dictionary = creation_->Get< PropertiesDictionary >();
        UndergroundAttribute* attribute = new UndergroundAttribute( dictionary, controller_, *creation_ );
        attribute->SetNetwork( network_->currentText().toStdString() );
        creation_->Attach( *attribute );
    }
}
