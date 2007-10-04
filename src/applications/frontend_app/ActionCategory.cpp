// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "ActionCategory.h"

// -----------------------------------------------------------------------------
// Name: ActionCategory constructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
ActionCategory::ActionCategory( QWidget* parent )
    : QVButtonGroup( parent )
{
    setExclusive( true );
    setInsideMargin( 1 );
    setInsideSpacing( 0 );
    setPaletteBackgroundColor( Qt::white );
}

// -----------------------------------------------------------------------------
// Name: ActionCategory destructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
ActionCategory::~ActionCategory()
{
    // NOTHING
}
