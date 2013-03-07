// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FloodPrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/LoadableSpinBox.h"
#include "preparation/FloodAttribute.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FloodPrototype constructor
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
FloodPrototype::FloodPrototype( QWidget* parent, Object_ABC*& creation, kernel::Controllers& controllers, const kernel::DetectionMap& detection )
    : FloodPrototype_ABC( parent )
    , creation_         ( creation )
    , controllers_      ( controllers )
    , detection_        ( detection )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodPrototype destructor
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
FloodPrototype::~FloodPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodPrototype::Commit
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
void FloodPrototype::Commit( const kernel::Team_ABC& )
{
    if( creation_ )
    {
        PropertiesDictionary& dictionary = creation_->Get< PropertiesDictionary >();
        FloodAttribute* attribute = new FloodAttribute( dictionary, detection_, creation_->Get< Positions>(), controllers_, *creation_ );
        attribute->SetValues( depth_->value(), refDist_->value() );
        creation_->Attach( *attribute );
    }
}
