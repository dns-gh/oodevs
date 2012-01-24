// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Data_Container.h"

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container constructor
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
ADN_Data_Container::ADN_Data_Container()
    : ADN_Data_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container destructor
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
ADN_Data_Container::~ADN_Data_Container()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::FilesNeeded
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::FilesNeeded( T_StringList& vFiles ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->FilesNeeded( vFiles );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::Reset
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::Reset()
{
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::Load
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::Load( const tools::Loader_ABC& fileLoader )
{
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Load( fileLoader );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::Initialize
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::Initialize()
{
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Initialize();
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::Save
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::Save()
{
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Save();
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::IsValidDatabase
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
bool ADN_Data_Container::IsValidDatabase()
{
    bool result = true;
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        result = result && it->second->IsValidDatabase();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::ReadArchive
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::ReadArchive( xml::xistream& input )
{
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::WriteArchive
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::WriteArchive( xml::xostream& output )
{
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->WriteArchive( output );
}
