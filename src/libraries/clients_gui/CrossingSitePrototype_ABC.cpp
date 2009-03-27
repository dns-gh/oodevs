// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CrossingSitePrototype_ABC.h"
#include "clients_kernel/Units.h"
#include "Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype_ABC constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype_ABC::CrossingSitePrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "CrossingSitePrototype_ABC", "Crossing site" ) )
{
    new QLabel( tools::translate( "CrossingSitePrototype_ABC", "Width:" ), this );
    width_ = new QSpinBox( 0, 10000, 10, this );
    width_->setSuffix( Units::meters );

    new QLabel( tools::translate( "CrossingSitePrototype_ABC", "Depth:" ), this );
    depth_ = new QSpinBox( 0, 1000, 10, this );
    depth_->setSuffix( Units::meters );

    new QLabel( tools::translate( "CrossingSitePrototype_ABC", "Stream speed:" ), this );
    speed_ = new QSpinBox( 0, 100, 1, this );
    speed_->setSuffix( Units::metersPerSecond );

    new QLabel( tools::translate( "CrossingSitePrototype_ABC", "Build river banks:" ), this );
    needsConstruction_ = new QCheckBox( this );
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
