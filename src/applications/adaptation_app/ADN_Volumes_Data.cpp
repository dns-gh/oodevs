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
    // NOTHING
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
    Reset();
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
// Name: ADN_Volumes_Data::Reset
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Volumes_Data::Reset()
{
    vSizes_.Reset();
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
    std::string strName = input.attribute< std::string >( "name" );
    VolumeInfos* pNew = new VolumeInfos();
    pNew->strName_.SetTranslation( strName, translations_->GetTranslation( "volumes", strName ) );
    vSizes_.AddItem( pNew );
}

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_Data::WriteArchive
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Volumes_Data::WriteArchive( xml::xostream& output )
{
    if( vSizes_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "volumes" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Volumes" );
    for( T_VolumeInfos_Vector::const_iterator itSize = vSizes_.begin(); itSize != vSizes_.end(); ++itSize )
    {
        if( ( *itSize )->strName_.GetData().empty() )
            throw MASA_EXCEPTION( tools::translate( "Categories_Data", "Categories - Invalid volume type name" ).toStdString() );
        output << xml::start( "volume" )
            << xml::attribute( "name", ( *itSize )->strName_ )
            << xml::end;
    }
    output << xml::end;
}
