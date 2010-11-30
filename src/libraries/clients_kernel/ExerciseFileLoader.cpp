// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ExerciseFileLoader.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ExerciseFileLoader constructor
// Created: LDC 2010-11-29
// -----------------------------------------------------------------------------
ExerciseFileLoader::ExerciseFileLoader( const tools::ExerciseConfig& config )
    : FileLoader( config )
{
}

// -----------------------------------------------------------------------------
// Name: ExerciseFileLoader destructor
// Created: LDC 2010-11-29
// -----------------------------------------------------------------------------
ExerciseFileLoader::~ExerciseFileLoader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseFileLoader::LoadAndUpdate
// Created: LDC 2010-11-29
// -----------------------------------------------------------------------------
FileLoader& ExerciseFileLoader::LoadAndUpdate( const std::string& rootTag, T_Loader loader, const std::string& xslTransform )
{
    xml::xifstream xis( config_.GetExerciseFile() );
    std::string file;
    GetFile( rootTag, xis, file );
    Check( config_.BuildExerciseChildFile( file ), loader, xslTransform );
    return *this;
}