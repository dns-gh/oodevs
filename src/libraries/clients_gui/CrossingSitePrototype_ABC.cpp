// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->addWidget( new QLabel( tools::translate( "gui::CrossingSitePrototype_ABC", "Width:" ) ) );
    width_ = new LoadableSpinBox( 1, 10000, 10, this );
    width_->setSuffix( kernel::Units::meters.AsString() );
    layout->addWidget( width_ );

    layout->addWidget( new QLabel( tools::translate( "gui::CrossingSitePrototype_ABC", "Depth:" ) ) );
    depth_ = new LoadableSpinBox( 1, 1000, 10, this );
    depth_->setSuffix( kernel::Units::meters.AsString() );
    layout->addWidget( depth_ );

    layout->addWidget( new QLabel( tools::translate( "gui::CrossingSitePrototype_ABC", "Stream speed:" ) ) );
    speed_ = new LoadableSpinBox( 0, 100, 1, this );
    speed_->setSuffix( kernel::Units::metersPerSecond.AsString() );
    layout->addWidget( speed_ );

    needsConstruction_ = new LoadableCheckBox( tools::translate( "gui::CrossingSitePrototype_ABC", "Build river banks:" ), 0 );
    layout->addWidget( needsConstruction_ );
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
bool CrossingSitePrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
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
