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

using namespace gui;

// -----------------------------------------------------------------------------
// Name: FloodPrototype_ABC constructor
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
FloodPrototype_ABC::FloodPrototype_ABC( QWidget* parent )
: ObjectAttributePrototype_ABC( parent, tools::translate( "gui::FloodPrototype_ABC", "Flood" ) )
{
    new QLabel( tools::translate( "gui::FloodPrototype_ABC", "Depth:" ), this );
    QHBox* box1 = new QHBox( this );
    depth_ = new QLineEdit( QString::number( 0 ), box1 );
    depth_->setValidator( new QIntValidator( 0, std::numeric_limits< int >::max(), depth_ ) );
    new QLabel( kernel::Units::meters.AsString(), box1 );
    new QLabel( tools::translate( "gui::FloodPrototype_ABC", "Reference distance:" ), this );
    QHBox* box2 = new QHBox( this );
    refDist_ = new QLineEdit( QString::number( 0 ), box2 );
    refDist_->setValidator( new QIntValidator( 0, std::numeric_limits< int >::max(), refDist_ ) );
    new QLabel( kernel::Units::meters.AsString(), box2 );
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
bool FloodPrototype_ABC::CheckValidity() const
{
    return true;
}
