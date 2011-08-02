// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::MinePrototype_ABC */

#include "clients_gui_pch.h"
#include "MinePrototype_ABC.h"
#include "clients_kernel/Units.h"
#include "Tools.h"
#include "LoadableLineEdit.h"


using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MinePrototype_ABC constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MinePrototype_ABC::MinePrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::MinePrototype_ABC", "Mine parameters" ) )
{
    densityLabel_ = new QLabel( tools::translate( "gui::MinePrototype_ABC", "Density:" ), this );
    densityBox_ = new Q3HBox( this );
    density_ = new LoadableLineEdit( densityBox_ );
    density_->setText( QString::number( 0. ) );
    density_->setValidator( new QDoubleValidator( 0, 5, 3, density_ ) );
    new QLabel( Units::minesPerMeter.AsString(), densityBox_ );
}

// -----------------------------------------------------------------------------
// Name: MinePrototype_ABC destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MinePrototype_ABC::~MinePrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MinePrototype_ABC::CheckValidity
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
bool MinePrototype_ABC::CheckValidity() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MinePrototype_ABC::SetMineField
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MinePrototype_ABC::SetMineField( bool minefield )
{
    densityLabel_->setShown( minefield );
    densityBox_->setShown( minefield );
}

// -----------------------------------------------------------------------------
// Name: MinePrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void MinePrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    density_->SetLoader( loader );
}
