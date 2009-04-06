// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "BypassPrototype_ABC.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: BypassPrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
BypassPrototype_ABC::BypassPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "BypassPrototype_ABC", "Bypass parameters" ) )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: BypassPrototype_ABC destructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
BypassPrototype_ABC::~BypassPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassPrototype_ABC::CheckValidity
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
bool BypassPrototype_ABC::CheckValidity() const
{
    return true;
}
