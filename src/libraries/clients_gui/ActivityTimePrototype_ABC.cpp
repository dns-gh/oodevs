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
#include "clients_kernel/Units.h"
#include "Tools.h"
#include <qvalidator.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
ActivityTimePrototype_ABC::ActivityTimePrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "ActivityTimePrototype_ABC", "Activity time" ) )    
{    
    new QLabel( tools::translate( "ActivityTimePrototype_ABC", "Activity time:" ), this );
    activityTime_ = new QTimeEdit( this );
}

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype_ABC destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
ActivityTimePrototype_ABC::~ActivityTimePrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool ActivityTimePrototype_ABC::CheckValidity() const
{
    return true;
}
