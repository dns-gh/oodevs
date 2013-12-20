// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ConstructionPrototype_ABC.h"
#include "LoadableSpinBox.h"
#include "SubObjectName.h"

#include "clients_kernel/Tools.h"
#include "clients_kernel/Units.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
ConstructionPrototype_ABC::ConstructionPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, "ConstructionPrototypeABC", tools::translate( "gui::ConstructionPrototype_ABC", "Construction" ) )
{
    SubObjectName subObject( "ConstructionPrototypeABC" );
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->setMargin( 5 );
    layout->addWidget( new QLabel( tools::translate( "gui::ConstructionPrototype_ABC", "Value:" ) ) );
    completion_ = new LoadableSpinBox( "completion", 1, 100, 1, 0 );
    completion_->setSuffix( kernel::Units::percentage.AsString() );
    completion_->setValue( 100 );
    layout->addWidget( completion_ );
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
bool ConstructionPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
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
