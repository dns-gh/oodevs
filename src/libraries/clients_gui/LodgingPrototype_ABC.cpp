// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LodgingPrototype_ABC.h"
#include "RichSpinBox.h"
#include "SubObjectName.h"

#include "clients_kernel/Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LodgingPrototype_ABC constructor
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
LodgingPrototype_ABC::LodgingPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, "LodgingPrototype_ABC", tools::translate( "gui::LodgingPrototype_ABC", "Lodging" ) )
{
    SubObjectName subObject( "LodgingPrototype_ABC" );
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->addWidget( new QLabel( tools::translate( "gui::LodgingPrototype_ABC", "Capacity:" ) ) );
    lodgingCapacity_ = new RichSpinBox( "lodgingCapacity", 0, 0, std::numeric_limits< int >::max(), 1 );
    layout->addWidget( lodgingCapacity_ );
}

// -----------------------------------------------------------------------------
// Name: LodgingPrototype_ABC destructor
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
LodgingPrototype_ABC::~LodgingPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingPrototype_ABC::CheckValidity
// Created: MMC 2011-05-02
// -----------------------------------------------------------------------------
bool LodgingPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return true;
}
