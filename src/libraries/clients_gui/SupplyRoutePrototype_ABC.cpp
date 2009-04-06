// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SupplyRoutePrototype_ABC.h"
#include "clients_kernel/Units.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
SupplyRoutePrototype_ABC::SupplyRoutePrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "SupplyRoutePrototype_ABC", "Logistic route" ) )
{
    new QLabel( this, tools::translate( "SupplyRoutePrototype_ABC", "Flow:" ) );
    flow_ = new QSpinBox( 0, 10000, 1, this );
    flow_->setSuffix( kernel::Units::vehiclesPerHour );

    new QLabel( tools::translate( "SupplyRoutePrototype_ABC", "Width:" ), this );
    width_ = new QSpinBox( 0, 10000, 1, this );
    width_->setSuffix( kernel::Units::meters );

    new QLabel( tools::translate( "SupplyRoutePrototype_ABC", "Length:" ), this );
    length_ = new QSpinBox( 0, 10000, 1, this );
    length_->setSuffix( kernel::Units::meters );

    new QLabel( tools::translate( "SupplyRoutePrototype_ABC", "Maximum weight:" ), this );
    maxWeight_ = new QSpinBox( 0, 10000, 1, this );
    maxWeight_->setSuffix( kernel::Units::tons );

    equipped_ = new QCheckBox( tools::translate( "SupplyRoutePrototype_ABC", "Equipped:" ), this );
}

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype_ABC destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
SupplyRoutePrototype_ABC::~SupplyRoutePrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool SupplyRoutePrototype_ABC::CheckValidity() const
{
    return true;
}
