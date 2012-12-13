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
#include "LoadableSpinBox.h"
#include "LoadableCheckBox.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
SupplyRoutePrototype_ABC::SupplyRoutePrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::SupplyRoutePrototype_ABC", "Logistic route" ) )
{
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->addWidget( new QLabel( tools::translate( "gui::SupplyRoutePrototype_ABC", "Flow:" ) ) );
    flow_ = new LoadableSpinBox( 1, 10000, 1, 0 );
    flow_->setSuffix( kernel::Units::vehiclesPerHour.AsString() );
    layout->addWidget( flow_ );

    layout->addWidget( new QLabel( tools::translate( "gui::SupplyRoutePrototype_ABC", "Width:" ) ) );
    width_ = new LoadableSpinBox( 1, 10000, 1, 0 );
    width_->setSuffix( kernel::Units::meters.AsString() );
    layout->addWidget( width_ );

    layout->addWidget( new QLabel( tools::translate( "gui::SupplyRoutePrototype_ABC", "Length:" ) ) );
    length_ = new LoadableSpinBox( 1, 10000, 1, 0 );
    length_->setSuffix( kernel::Units::meters.AsString() );
    layout->addWidget( length_ );

    layout->addWidget( new QLabel( tools::translate( "gui::SupplyRoutePrototype_ABC", "Maximum weight:" ) ) );
    maxWeight_ = new LoadableSpinBox( 1, 10000, 1, 0 );
    maxWeight_->setSuffix( kernel::Units::tons.AsString() );
    layout->addWidget( maxWeight_ );

    equipped_ = new LoadableCheckBox( tools::translate( "gui::SupplyRoutePrototype_ABC", "Equipped:" ), 0 );
    layout->addWidget( equipped_ );
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
bool SupplyRoutePrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: SupplyRoutePrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void SupplyRoutePrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    flow_->SetLoader( loader );
    width_->SetLoader( loader );
    length_->SetLoader( loader );
    maxWeight_->SetLoader( loader );
    equipped_->SetLoader( loader );
}
