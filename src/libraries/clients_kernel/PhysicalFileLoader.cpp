// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "PhysicalFileLoader.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PhysicalFileLoader constructor
// Created: LDC 2010-11-29
// -----------------------------------------------------------------------------
PhysicalFileLoader::PhysicalFileLoader( const tools::ExerciseConfig& config, std::string& invalidSignatureFiles )
    : FileLoader( config )
{
    std::string filename = config.GetPhysicalFile();
    CheckSignatures( filename, invalidSignatureFiles );
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