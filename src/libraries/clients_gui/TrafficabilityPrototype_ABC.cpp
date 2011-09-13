// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::TrafficabilityPrototype_ABC */

#include "clients_gui_pch.h"
#include "TrafficabilityPrototype_ABC.h"
#include "Tools.h"
#include "clients_kernel/Units.h"
#include "LoadableLineEdit.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TrafficabilityPrototype_ABC constructor
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityPrototype_ABC::TrafficabilityPrototype_ABC( QWidget* parent, double maxWeight )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::TrafficabilityPrototype_ABC", "Trafficability" ) )
{
    new QLabel( tools::translate( "gui::TrafficabilityPrototype_ABC", "Max weight:" ), this );
    Q3HBox* box = new Q3HBox( this );
    maxWeight_ = new LoadableLineEdit( box );
    maxWeight_->setText( QString::number( maxWeight ) );
    maxWeight_->setValidator( new QDoubleValidator( 0, 1000000, 2, maxWeight_ ) );
    new QLabel( kernel::Units::tons.AsString(), box );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityPrototype_ABC destructor
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityPrototype_ABC::~TrafficabilityPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityPrototype_ABC::CheckValidity
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
bool TrafficabilityPrototype_ABC::CheckValidity() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityPrototype_ABC::SetLoader
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
void TrafficabilityPrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    maxWeight_->SetLoader( loader );
}
