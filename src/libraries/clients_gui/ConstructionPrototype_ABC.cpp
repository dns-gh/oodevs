// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ConstructionPrototype_ABC.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
ConstructionPrototype_ABC::ConstructionPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "ConstructionPrototype_ABC", "Construction" ) )    
{
    // new QLabel( tools::translate( "ConstructionPrototype_ABC", "Construction:" ), this );
}

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype_ABC destructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
ConstructionPrototype_ABC::~ConstructionPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype_ABC::CheckValidity
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
bool ConstructionPrototype_ABC::CheckValidity() const
{
    return true;
}