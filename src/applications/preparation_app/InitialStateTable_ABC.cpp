// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "InitialStateTable_ABC.h"

// -----------------------------------------------------------------------------
// Name: InitialStateTable_ABC constructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
InitialStateTable_ABC::InitialStateTable_ABC( int numRows, int numCols, QWidget* parent, const char* name /* = 0*/ )
    : Q3Table( numRows, numCols, parent, name )
{
    setShowGrid( false );
    setLeftMargin( 0 );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTable_ABC destructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
InitialStateTable_ABC::~InitialStateTable_ABC()
{
    // NOTHING
}
