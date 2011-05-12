// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::CrossingSitePrototype_ABC */

#include "clients_gui_pch.h"
#include "CrossingSitePrototype_ABC.h"
#include "clients_kernel/Units.h"
#include "Tools.h"
#include "LoadableSpinBox.h"
#include "LoadableCheckBox.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype_ABC constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype_ABC::CrossingSitePrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::CrossingSitePrototype_ABC", "Crossing site" ) )
{
    new QLabel( tools::translate( "gui::CrossingSitePrototype_ABC", "Width:" ), this );
    width_ = new LoadableSpinBox( 1, 10000, 10, this );
    width_->setSuffix( kernel::Units::meters.AsString() );

    new QLabel( tools::translate( "gui::CrossingSitePrototype_ABC", "Depth:" ), this );
    depth_ = new LoadableSpinBox( 1, 1000, 10, this );
    depth_->setSuffix( kernel::Units::meters.AsString() );

    new QLabel( tools::translate( "gui::CrossingSitePrototype_ABC", "Stream speed:" ), this );
    speed_ = new LoadableSpinBox( 0, 100, 1, this );
    speed_->setSuffix( kernel::Units::metersPerSecond.AsString() );

    needsConstruction_ = new LoadableCheckBox( tools::translate( "gui::CrossingSitePrototype_ABC", "Build river banks:" ), this );
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype_ABC destructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype_ABC::~CrossingSitePrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype_ABC::CheckValidity
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
bool CrossingSitePrototype_ABC::CheckValidity() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void CrossingSitePrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    width_->SetLoader( loader );
    depth_->SetLoader( loader );
    speed_->SetLoader( loader );
    needsConstruction_->SetLoader( loader );
}
