// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ConstructionPrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/LoadableSpinBox.h"
#include "preparation/ConstructionAttribute.h"

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype constructor
// Created: SBO 2009-06-09
// -----------------------------------------------------------------------------
ConstructionPrototype::ConstructionPrototype( QWidget* parent, kernel::Object_ABC*& creation )
    : gui::ConstructionPrototype_ABC( parent )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype destructor
// Created: SBO 2009-06-09
// -----------------------------------------------------------------------------
ConstructionPrototype::~ConstructionPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype::Commit
// Created: SBO 2009-06-09
// -----------------------------------------------------------------------------
void ConstructionPrototype::Commit()
{
    if( creation_ )
    {
        kernel::PropertiesDictionary& dico = creation_->Get< kernel::PropertiesDictionary >();
        ConstructionAttribute* attribute = new ConstructionAttribute( dico );
        attribute->SetCompletion( completion_->value() );
        creation_->Attach( *attribute );
    }
}
