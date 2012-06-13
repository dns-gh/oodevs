// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::FloodPrototype_ABC */

#include "clients_gui_pch.h"
#include "FloodPrototype_ABC.h"
#include "Tools.h"
#include "clients_kernel/Units.h"
#include "LoadableSpinBox.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: FloodPrototype_ABC constructor
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
FloodPrototype_ABC::FloodPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::FloodPrototype_ABC", "Flood" ) )
{
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->addWidget( new QLabel( tools::translate( "gui::FloodPrototype_ABC", "Depth:" ) ) );
    depth_ = new LoadableSpinBox( 0, std::numeric_limits< int >::max(), 1, 0 );
    depth_->setSuffix( kernel::Units::meters.AsString() );
    layout->addWidget( depth_ );

    layout->addWidget( new QLabel( tools::translate( "gui::FloodPrototype_ABC", "Reference distance:" ) ) );
    refDist_ = new LoadableSpinBox( 0, std::numeric_limits< int >::max(), 1, 0 );
    refDist_->setSuffix( kernel::Units::meters.AsString() );
    layout->addWidget( refDist_ );
}

// -----------------------------------------------------------------------------
// Name: FloodPrototype_ABC destructor
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
FloodPrototype_ABC::~FloodPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodPrototype_ABC::CheckValidity
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
bool FloodPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: FloodPrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void FloodPrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    depth_->SetLoader( loader );
    refDist_->SetLoader( loader );
}
