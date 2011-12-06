// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::LodgingPrototype_ABC */

#include "clients_gui_pch.h"
#include "LodgingPrototype_ABC.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LodgingPrototype_ABC constructor
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
LodgingPrototype_ABC::LodgingPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::LodgingPrototype_ABC", "Lodging" ) )
{
    new QLabel( tools::translate( "gui::LodgingPrototype_ABC", "Capacity:" ), this );
    lodgingCapacity_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, this );
}

// -----------------------------------------------------------------------------
// Name: LodgingPrototype_ABC destructor
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
LodgingPrototype_ABC::~LodgingPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingPrototype_ABC::CheckValidity
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
bool LodgingPrototype_ABC::CheckValidity() const
{
    return true;
}
