// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AltitudeModifierPrototype_ABC.h"
#include "LoadableSpinBox.h"
#include "SubObjectName.h"

#include "clients_kernel/Tools.h"
#include "clients_kernel/Units.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: AltitudeModifierPrototype_ABC constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierPrototype_ABC::AltitudeModifierPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, "AltitudeModifierPrototype_ABC", tools::translate( "gui::AltitudeModifierPrototype_ABC", "Altitude modifier" ) )
{
    SubObjectName subObject( "AltitudeModifierPrototype_ABC" );
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->setMargin( 5 );
    layout->addWidget( new QLabel( tools::translate( "gui::AltitudeModifierPrototype_ABC", "Height:" ) ) );
    height_ = new LoadableSpinBox( "height", 0, std::numeric_limits< short >::max(), 1, 0 );
    height_->setSuffix( kernel::Units::meters.AsString() );
    layout->addWidget( height_ );
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
bool AltitudeModifierPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
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
