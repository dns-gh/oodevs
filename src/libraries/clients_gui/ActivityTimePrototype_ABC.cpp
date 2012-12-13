// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ActivityTimePrototype_ABC.h"
#include "Tools.h"
#include "LoadableTimeEdit.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
ActivityTimePrototype_ABC::ActivityTimePrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::ActivityTimePrototype_ABC", "Life time:" ) )
{
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->addWidget( new QLabel( tools::translate( "gui::ActivityTimePrototype_ABC", "Life time:" ) ) );
    activityTime_ = new LoadableTimeEdit( 0 );
    layout->addWidget( activityTime_ );
}

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype_ABC destructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
ActivityTimePrototype_ABC::~ActivityTimePrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype_ABC::CheckValidity
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
bool ActivityTimePrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void ActivityTimePrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    activityTime_->SetLoader( loader );
}
