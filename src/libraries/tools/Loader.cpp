// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"

#include "Loader.h"
#include "PhysicalFileLoader.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: Loader constructor
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
Loader::Loader( const ExerciseConfig& config )
    : config_( config )
{   
}

// -----------------------------------------------------------------------------
// Name: Loader destructor
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
Loader::~Loader()
{   
}

// -----------------------------------------------------------------------------
// Name: Loader destructor
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void Loader::LoadPhysicalFile( const std::string& rootTag, T_Loader loader, std::string& invalidSignatureFiles, std::string& missingSignatureFiles ) const
{
    PhysicalFileLoader( config_, invalidSignatureFiles, missingSignatureFiles )
        .Load( rootTag, loader );
}