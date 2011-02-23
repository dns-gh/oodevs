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
#include "ExerciseFileLoader.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: Loader constructor
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
Loader::Loader( const ExerciseConfig& config )
    : config_( config )
{   
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loader destructor
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
Loader::~Loader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadPhysicalFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void Loader::LoadPhysicalFile( const std::string& rootTag, T_Loader loader, std::string& invalidSignatureFiles, std::string& missingSignatureFiles ) const
{
    PhysicalFileLoader( config_, invalidSignatureFiles, missingSignatureFiles )
        .Load( rootTag, loader );
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadPhysicalFileAndCRC
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void Loader::LoadPhysicalFileAndCRC( const std::string& rootTag, T_Loader loader, std::string& invalidSignatureFiles, std::string& missingSignatureFiles ) const
{
    PhysicalFileLoader fileLoader( config_, invalidSignatureFiles, missingSignatureFiles );
    fileLoader.AddToCRC();
    fileLoader.Load( rootTag, loader );
}

// -----------------------------------------------------------------------------
// Name: Loader::CheckFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void Loader::CheckFile( const std::string& file ) const
{
    FileLoader( config_, file );
}

// -----------------------------------------------------------------------------
// Name: Loader::CheckFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void Loader::LoadAndUpdateExerciseFile( const std::string& rootTag, T_Loader loader, const std::string& xslFile ) const
{
    ExerciseFileLoader fileLoader( config_ );
    fileLoader.LoadAndUpdate( rootTag, loader, xslFile );
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadExerciseFileAndCRC
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void Loader::LoadExerciseFileAndCRC( const std::string& rootTag, T_Loader loader, std::string& invalidSignatureFiles, std::string& missingSignatureFiles ) const
{
    ExerciseFileLoader fileLoader( config_, invalidSignatureFiles, missingSignatureFiles );
    fileLoader.AddToCRC();
    fileLoader.Load( rootTag, loader );
}
