// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "PhysicalFileLoader.h"
#include "ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: PhysicalFileLoader constructor
// Created: LDC 2010-11-30
// -----------------------------------------------------------------------------
PhysicalFileLoader::PhysicalFileLoader( const tools::ExerciseConfig& config )
    : FileLoader( config )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PhysicalFileLoader constructor
// Created: LDC 2010-11-29
// -----------------------------------------------------------------------------
PhysicalFileLoader::PhysicalFileLoader( const tools::ExerciseConfig& config, std::string& invalidSignatureFiles, std::string& missingSignatureFiles )
    : FileLoader( config )
{
    std::string filename = config.GetPhysicalFile();
    CheckSignatures( filename, invalidSignatureFiles, missingSignatureFiles );
}

// -----------------------------------------------------------------------------
// Name: PhysicalFileLoader destructor
// Created: LDC 2010-11-29
// -----------------------------------------------------------------------------
PhysicalFileLoader::~PhysicalFileLoader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PhysicalFileLoader::LoadAndUpdate
// Created: LDC 2010-11-29
// -----------------------------------------------------------------------------
FileLoader& PhysicalFileLoader::LoadAndUpdate( const std::string& rootTag, T_Loader loader, const std::string& xslTransform )
{
    xml::xifstream xis( config_.GetPhysicalFile() );
    std::string file;
    GetFile( rootTag, xis, file );
    Check( config_.BuildPhysicalChildFile( file ), loader, xslTransform );
    return *this;
}