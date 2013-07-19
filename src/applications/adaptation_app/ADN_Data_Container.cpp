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
ADN_Data_Container::ADN_Data_Container( E_WorkspaceElements currentTab )
    : ADN_Data_ABC( currentTab )
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
void ADN_Data_Container::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->FilesNeeded( vFiles );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::Reset
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::Reset()
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::Load
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::Load( const tools::Loader_ABC& fileLoader )
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Load( fileLoader );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::Initialize
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::Initialize()
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Initialize();
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::Save
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::Save()
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Save();
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::CheckDatabaseValidity
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->CheckDatabaseValidity( checker );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::ReadArchive
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::ReadArchive( xml::xistream& input )
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::WriteArchive
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Data_Container::WriteArchive( xml::xostream& output )
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->WriteArchive( output );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::OnLanguageChanged
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
void ADN_Data_Container::OnLanguageChanged( const std::string& language )
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->OnLanguageChanged( language );
}
