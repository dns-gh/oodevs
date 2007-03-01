// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "ExerciseInfo.h"

// -----------------------------------------------------------------------------
// Name: ExerciseInfo constructor
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
ExerciseInfo::ExerciseInfo( QWidget* parent, kernel::Controllers& controllers )
    : QVBox( parent )
{
    setFrameStyle( QFrame::Panel | QFrame::Plain );
}

// -----------------------------------------------------------------------------
// Name: ExerciseInfo destructor
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
ExerciseInfo::~ExerciseInfo()
{
    // NOTHING
}
