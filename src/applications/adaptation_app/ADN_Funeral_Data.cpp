// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Funeral_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Enums.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Funeral_Data::ADN_Funeral_Data()
    : ADN_Data_ABC( eLogistic, eFuneral )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data destructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Funeral_Data::~ADN_Funeral_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Funeral_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szFuneral_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data::ReadFuneralPackagingResource
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Funeral_Data::ReadFuneralPackagingResource( xml::xistream& input )
{
    std::unique_ptr< ADN_FuneralPackagingResource > pNew( new ADN_FuneralPackagingResource() );
    pNew->ReadArchive( input );
    if( pNew->GetCrossedElement() )
        funeralPackagingResources_.AddItem( pNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, "Funeral", eFuneral, -1, tools::translate( "ADN_Funeral_Data", "Packagings" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Funeral_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "funeral" )
            >> xml::start( "fake-transporter" )
                >> xml::attribute( "speed", fakeTransporterSpeed_ )
            >> xml::end
            >> xml::start( "packagings" )
                >> xml::list( "packaging", *this, &ADN_Funeral_Data::ReadFuneralPackagingResource )
            >> xml::end
          >> xml::end;
    funeralPackagingResources_.AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Funeral_Data::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "funeral" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Funeral" );
    output  << xml::start( "fake-transporter" )
                << xml::attribute( "speed", fakeTransporterSpeed_ )
            << xml::end
            << xml::start( "packagings" );
    for( auto it = funeralPackagingResources_.begin(); it != funeralPackagingResources_.end(); ++it )
        (*it)->WriteArchive( output );
    output  << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data::CheckDatabaseValidity
// Created: JSR 2013-04-11
// -----------------------------------------------------------------------------
void ADN_Funeral_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( auto it = funeralPackagingResources_.begin(); it != funeralPackagingResources_.end(); ++it )
        ( *it )->CheckValidity( checker, ( *it )->strName_.GetData(), eLogistic, eFuneral, tools::translate( "ADN_Funeral_Data", "Packaging" ).toStdString() );
}
