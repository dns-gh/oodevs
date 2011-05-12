// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::ConstructionPrototype_ABC */

#include "clients_gui_pch.h"
#include "ConstructionPrototype_ABC.h"
#include "Tools.h"
#include "LoadableSpinBox.h"
#include "clients_kernel/Units.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
ConstructionPrototype_ABC::ConstructionPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::ConstructionPrototype_ABC", "Construction" ) )
{
    new QLabel( tools::translate( "gui::ConstructionPrototype_ABC", "Construction:" ), this );
    completion_ = new LoadableSpinBox( 1, 100, 1, this );
    completion_->setSuffix( kernel::Units::percentage.AsString() );
    completion_->setValue( 100 );
}

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype_ABC destructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
ConstructionPrototype_ABC::~ConstructionPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype_ABC::CheckValidity
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
bool ConstructionPrototype_ABC::CheckValidity() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void ConstructionPrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    completion_->SetLoader( loader );
}
