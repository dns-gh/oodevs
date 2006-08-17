// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "CrossingSitePrototype_ABC.h"
#include "astec_kernel/Units.h"

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype_ABC constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype_ABC::CrossingSitePrototype_ABC( QWidget* parent )
    : ObjectPrototypeAttributes_ABC( parent, tr( "Site de franchissement" ) )
{
    new QLabel( tr( "Largeur:" ), this );
    width_ = new QSpinBox( 0, 10000, 10, this );
    width_->setSuffix( Units::meters );

    new QLabel( tr( "Profondeur:" ), this );
    depth_ = new QSpinBox( 0, 1000, 10, this );
    depth_->setSuffix( Units::meters );

    new QLabel( tr( "Vitesse courant:" ), this );
    speed_ = new QSpinBox( 0, 100, 1, this );
    speed_->setSuffix( Units::metersPerSecond );

    new QLabel( tr( "Berges à aménager:" ), this );
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
