// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FirePropagationModifierPrototype.h"
#include "clients_kernel/Units.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"

using namespace sword;

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierPrototype constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
FirePropagationModifierPrototype::FirePropagationModifierPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList )
    : FirePropagationModifierPrototype_ABC ( parent )
    , attributesList_( attributesList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierPrototype destructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
FirePropagationModifierPrototype::~FirePropagationModifierPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierPrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void FirePropagationModifierPrototype::Commit()
{
    //NOTHING
}
