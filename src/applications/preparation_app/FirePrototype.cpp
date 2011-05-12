// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FirePrototype.h"
#include "clients_kernel/FireClass.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/LoadableSpinBox.h"
#include "preparation/FireAttribute.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FirePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
FirePrototype::FirePrototype( QWidget* parent, const tools::Resolver_ABC< FireClass, std::string >& resolver, Object_ABC*& creation )
    : FirePrototype_ABC( parent, resolver )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
FirePrototype::~FirePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void FirePrototype::Commit()
{
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        FireAttribute* attribute = new FireAttribute( dico );
        attribute->SetClass( *fireClass_->GetValue() );
        attribute->SetMaxCombustionEnergy( maxCombustionEnergy_->value() );
        creation_->Attach( *attribute );
    }
}
