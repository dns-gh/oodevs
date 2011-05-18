// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::AltitudeModifierPrototype_ABC */

#include "clients_gui_pch.h"
#include "AltitudeModifierPrototype_ABC.h"
#include "Tools.h"
#include "clients_kernel/Units.h"
#include "LoadableSpinBox.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: AltitudeModifierPrototype_ABC constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierPrototype_ABC::AltitudeModifierPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::AltitudeModifierPrototype_ABC", "Altitude modifier" ) )
{
    new QLabel( tools::translate( "gui::AltitudeModifierPrototype_ABC", "Height:" ), this );
    height_ = new LoadableSpinBox( 0, std::numeric_limits< int >::max(), 1, this );
    height_->setSuffix( kernel::Units::meters.AsString() );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierPrototype_ABC destructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierPrototype_ABC::~AltitudeModifierPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierPrototype_ABC::CheckValidity
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
bool AltitudeModifierPrototype_ABC::CheckValidity() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierPrototype_ABC::SetLoader
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierPrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    height_->SetLoader( loader );
}
