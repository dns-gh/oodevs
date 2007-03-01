// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "ExercisesTab.h"
#include "ExerciseList.h"
#include "ExerciseInfo.h"

// -----------------------------------------------------------------------------
// Name: ExercisesTab constructor
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
ExercisesTab::ExercisesTab( QWidget* parent, kernel::Controllers& controllers )
    : QHBox( parent )
{
    setMargin( 5 );
    setSpacing( 5 );
    setStretchFactor( new ExerciseList( this, controllers ), 1 );
    setStretchFactor( new ExerciseInfo( this, controllers ), 3 ); // $$$$ SBO 2007-02-01: dummy
}

// -----------------------------------------------------------------------------
// Name: ExercisesTab destructor
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
ExercisesTab::~ExercisesTab()
{
    // NOTHING
}
