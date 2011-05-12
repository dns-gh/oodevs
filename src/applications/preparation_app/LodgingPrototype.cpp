// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LodgingPrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "preparation/LodgingAttribute.h"

// -----------------------------------------------------------------------------
// Name: LodgingPrototype constructor
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
LodgingPrototype::LodgingPrototype( QWidget* parent, kernel::Object_ABC*& creation )
    : gui::LodgingPrototype_ABC( parent )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingPrototype destructor
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
    if( creation_ )
    {
        kernel::PropertiesDictionary& dico = creation_->Get< kernel::PropertiesDictionary >();
        LodgingAttribute* attribute = new LodgingAttribute( dico );
        attribute->SetLodgingCapacity( lodgingCapacity_->value() );
        creation_->Attach( *attribute );
    }
}
