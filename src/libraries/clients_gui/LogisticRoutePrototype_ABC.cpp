// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LogisticRoutePrototype_ABC.h"
#include "clients_kernel/Units.h"
#include "Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
LogisticRoutePrototype_ABC::LogisticRoutePrototype_ABC( QWidget* parent )
    : ObjectPrototypeAttributes_ABC( parent, tools::translate( "LogisticRoutePrototype_ABC", "Logistic route" ) )
{
    new QLabel( tools::translate( "LogisticRoutePrototype_ABC", "Flow:" ), this );
    flow_ = new QSpinBox( 0, 10000, 1, this );
    flow_->setSuffix( Units::vehiclesPerHour );

    new QLabel( tools::translate( "LogisticRoutePrototype_ABC", "Width:" ), this );
    width_ = new QSpinBox( 0, 10000, 1, this );
    width_->setSuffix( Units::meters );

    new QLabel( tools::translate( "LogisticRoutePrototype_ABC", "Length:" ), this );
    length_ = new QSpinBox( 0, 10000, 1, this );
    length_->setSuffix( Units::meters );

    new QLabel( tools::translate( "LogisticRoutePrototype_ABC", "Maximum weight:" ), this );
    maxWeight_ = new QSpinBox( 0, 10000, 1, this );
    maxWeight_->setSuffix( Units::tons );

    equipped_ = new QCheckBox( tools::translate( "LogisticRoutePrototype_ABC", "Equipped:" ), this );
}

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype_ABC destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
LogisticRoutePrototype_ABC::~LogisticRoutePrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool LogisticRoutePrototype_ABC::CheckValidity() const
{
    return true;
}
