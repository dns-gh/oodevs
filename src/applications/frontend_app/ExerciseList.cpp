// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "ExerciseList.h"

// -----------------------------------------------------------------------------
// Name: ExerciseList constructor
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
ExerciseList::ExerciseList( QWidget* parent, kernel::Controllers& controllers )
    : ElementListView< frontend::Exercise >( parent, controllers )
{
    addColumn( tr( "Exercises" ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList destructor
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
ExerciseList::~ExerciseList()
{
    // NOTHING
}
