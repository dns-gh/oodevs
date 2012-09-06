// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::BypassPrototype_ABC */

#include "clients_gui_pch.h"
#include "BypassPrototype_ABC.h"
#include "LoadableSpinBox.h"
#include "Tools.h"
#include "clients_kernel/Units.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: BypassPrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
BypassPrototype_ABC::BypassPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::BypassPrototype_ABC", "Breaching" ) )
{
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->addWidget( new QLabel( tools::translate( "gui::ByPassPrototype_ABC", "Value:" ) ) );
    bypassConstructionPercentage_ = new LoadableSpinBox( 1, 100, 1, 0 );
    bypassConstructionPercentage_->setSuffix( kernel::Units::percentage.AsString() );
    bypassConstructionPercentage_->setValue( 100 );
    layout->addWidget( bypassConstructionPercentage_ );
}

// -----------------------------------------------------------------------------
// Name: BypassPrototype_ABC destructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
BypassPrototype_ABC::~BypassPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassPrototype_ABC::CheckValidity
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
bool BypassPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: BypassPrototype_ABC::SetLoader
// Created: NPT 2012-09-05
// -----------------------------------------------------------------------------
void BypassPrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    bypassConstructionPercentage_->SetLoader( loader );
}

