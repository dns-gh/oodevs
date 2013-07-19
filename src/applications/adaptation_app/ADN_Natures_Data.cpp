// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Natures_Data.h"
#include "ADN_Project_Data.h"

tools::IdManager ADN_Natures_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: NatureInfos::NatureInfos
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
ADN_Natures_Data::NatureInfos::NatureInfos()
    : nId_( ADN_Natures_Data::idManager_.GetNextId() )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: NatureInfos::NatureInfos
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
ADN_Natures_Data::NatureInfos::NatureInfos( const std::string& name, int id )
    : ADN_RefWithName( name )
    , nId_( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNatureInfos::CreateCopy
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
ADN_Natures_Data::NatureInfos* ADN_Natures_Data::NatureInfos::CreateCopy()
{
    NatureInfos* result = new NatureInfos();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data constructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Natures_Data::ADN_Natures_Data()
    : ADN_Data_ABC( eCategories, eNatures )
{
    vDotationNatures_.AddUniquenessChecker( eError, duplicateName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data destructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Natures_Data::~ADN_Natures_Data()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data::FilesNeeded
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Natures_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szDotationNatures_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data::Reset
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Natures_Data::Reset()
{
    idManager_.Reset();
    vDotationNatures_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data::ReadArchive
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Natures_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "natures" )
              >> xml::list( "nature", *this, &ADN_Natures_Data::ReadNature )
          >> xml::end;
    vDotationNatures_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data::ReadNature
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Natures_Data::ReadNature( xml::xistream& input )
{
    std::string strName;
    int id ( 0 );
    input >> xml::attribute( "type", strName );
    input >> xml::optional >> xml::attribute( "id", id );
    ADN_Natures_Data::T_NatureInfos_Vector::iterator found = std::find_if( vDotationNatures_.begin(), vDotationNatures_.end(), ADN_Tools::NameCmp( strName ) );
    if( found != vDotationNatures_.end() )
        throw MASA_EXCEPTION( tools::translate( "Categories_Data", "Categories - Duplicated resource nature type name '%1'" ).arg( strName.c_str() ).toStdString() );
    if( !id )
        id = idManager_.GetNextId();
    ADN_Natures_Data::NatureInfos* pNew = new ADN_Natures_Data::NatureInfos( strName, id );
    vDotationNatures_.AddItem( pNew );
    idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data::WriteArchive
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Natures_Data::WriteArchive( xml::xostream& output )
{
    if( vDotationNatures_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "natures" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "ResourceNatures" );
    for( ADN_Natures_Data::T_NatureInfos_Vector::const_iterator it = vDotationNatures_.begin(); it != vDotationNatures_.end(); ++it )
    {
        if( ( *it )->strName_.GetData().empty() )
            throw MASA_EXCEPTION( tools::translate( "Categories_Data","Categories - Invalid resource nature" ).toStdString() );
        std::string strData( ( *it )->strName_.GetData() );
        output << xml::start( "nature" )
            << xml::attribute( "type", trim( strData ) )
            << xml::attribute( "id", ( *it )->nId_ )
            << xml::end;
    }
    output << xml::end;
}
