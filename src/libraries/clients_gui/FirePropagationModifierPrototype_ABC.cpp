// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "FirePropagationModifierPrototype_ABC.h"

#include "clients_kernel/Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierPrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
FirePropagationModifierPrototype_ABC::FirePropagationModifierPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, "FirePropagationModifierPrototype_ABC", tools::translate( "gui::FirePropagationModifierPrototype_ABC", "Fire propagation modifier" ) )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierPrototype_ABC destructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
FirePropagationModifierPrototype_ABC::~FirePropagationModifierPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierPrototype_ABC::CheckValidity
// Created: BCI 2011-01-11
// -----------------------------------------------------------------------------
bool FirePropagationModifierPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return true;
}
