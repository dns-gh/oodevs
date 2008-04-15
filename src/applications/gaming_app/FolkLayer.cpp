// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FolkLayer.h"
#include "gaming/FolkModel.h"

// -----------------------------------------------------------------------------
// Name: FolkLayer constructor
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
FolkLayer::FolkLayer( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, FolkModel& model )
    : gui::FolkLayer( controller, converter )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FolkLayer destructor
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
FolkLayer::~FolkLayer()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: FolkLayer::Paint
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( !ShouldDrawPass() )
        return;
    SetOccupation( model_.GetCurrentOccupation() );
    gui::FolkLayer::Paint( viewport );
}
