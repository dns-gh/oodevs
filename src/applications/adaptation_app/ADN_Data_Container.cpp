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
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        delete it->second;
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
// Name: ADN_Data_Container::GetElementABC
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
ADN_Data_ABC& ADN_Data_Container::GetElementABC( int index )
{
    auto it = elements_.find( index );
    assert( it != elements_.end() );
    return *it->second;
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_Container::ApplyOnTranslations
// Created: ABR 2013-10-08
// -----------------------------------------------------------------------------
bool ADN_Data_Container::ApplyOnTranslations( const boost::function< bool ( kernel::LocalizedString& ) >& functor )
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->ApplyOnTranslations( functor ) )
            return true;
    return false;
}
