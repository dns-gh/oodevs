// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "DirectoryExerciseLister.h"
#include "frontend/commands.h"

// -----------------------------------------------------------------------------
// Name: DirectoryExerciseLister constructor
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
DirectoryExerciseLister::DirectoryExerciseLister( const tools::GeneralConfig& config, const std::string& subDir )
    : subDir_( subDir )
    , config_( config )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectoryExerciseLister destructor
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
DirectoryExerciseLister::~DirectoryExerciseLister()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectoryExerciseLister::ListExercises
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void DirectoryExerciseLister::ListExercises( QStringList& list ) const
{
    list = frontend::commands::ListExercises( config_, subDir_ );
}

// -----------------------------------------------------------------------------
// Name: DirectoryExerciseLister::GetPort
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
unsigned short DirectoryExerciseLister::GetPort( const QString& /*exercise*/ ) const
{
    return 10001;
}
