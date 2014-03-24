// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Volumes_Data.h"
#include "ADN_Project_Data.h"
#include "clients_kernel/XmlTranslations.h"

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_Data::VolumeInfos
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
ADN_Volumes_Data::VolumeInfos::VolumeInfos()
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eCategories, eVolumes, "volumes" ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_Data::CreateCopy
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
ADN_Volumes_Data::VolumeInfos* ADN_Volumes_Data::VolumeInfos::CreateCopy()
{
    return new VolumeInfos();
}

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_Data constructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Volumes_Data::ADN_Volumes_Data()
    : ADN_Data_ABC( eCategories, eVolumes )
{
    vSizes_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_Data destructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Volumes_Data::~ADN_Volumes_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_Data::FilesNeeded
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Volumes_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szSizes_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_Data::ReadArchive
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Volumes_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "volumes" )
              >> xml::list( "volume", *this, &ADN_Volumes_Data::ReadVolume )
          >> xml::end;
    vSizes_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_Data::ReadVolume
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Volumes_Data::ReadVolume( xml::xistream& input )
{
    VolumeInfos* pNew = new VolumeInfos();
    input >> xml::attribute( "name", *pNew );
    vSizes_.AddItem( pNew );
}

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_Data::WriteArchive
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Volumes_Data::WriteArchive( xml::xostream& output ) const
{
    if( vSizes_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "volumes" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Volumes" );
    for( auto itSize = vSizes_.begin(); itSize != vSizes_.end(); ++itSize )
    {
        output << xml::start( "volume" )
            << xml::attribute( "name", **itSize )
            << xml::end;
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_Data::CheckDatabaseValidity
// Created: LDC 2014-03-21
// -----------------------------------------------------------------------------
void ADN_Volumes_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    for( auto it = vSizes_.begin(); it != vSizes_.end(); ++it )
        if( ( *it )->strName_.GetData().empty() )
            checker.AddError( eInvalidVolume, "", eCategories );
}
