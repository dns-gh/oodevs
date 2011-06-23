// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FirePropagationModifierPrototype.h"
#include "clients_kernel/FireClass.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "preparation/FireAttribute.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
FirePropagationModifierPrototype::FirePropagationModifierPrototype( QWidget* parent, Object_ABC*& creation )
    : FirePropagationModifierPrototype_ABC( parent )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierPrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
FirePropagationModifierPrototype::~FirePropagationModifierPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierPrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void FirePropagationModifierPrototype::Commit()
{
    //NOTHING
}
