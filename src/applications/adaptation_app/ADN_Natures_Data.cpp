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
#include "clients_kernel/XmlTranslations.h"

tools::IdManager ADN_Natures_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: NatureInfos::NatureInfos
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
ADN_Natures_Data::NatureInfos::NatureInfos()
    : nId_( ADN_Natures_Data::idManager_.GetNextId() )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eCategories, eNatures, "natures" ) );
}

// -----------------------------------------------------------------------------
// Name: NatureInfos::NatureInfos
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
ADN_Natures_Data::NatureInfos::NatureInfos( int id )
    : nId_( id )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eCategories, eNatures, "natures" ) );
}

// -----------------------------------------------------------------------------
// Name: NatureInfos::~NatureInfos
// Created: JSR 2013-07-30
// -----------------------------------------------------------------------------
ADN_Natures_Data::NatureInfos::~NatureInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNatureInfos::CreateCopy
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
ADN_Natures_Data::NatureInfos* ADN_Natures_Data::NatureInfos::CreateCopy()
{
    return new NatureInfos();
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data constructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Natures_Data::ADN_Natures_Data()
    : ADN_Data_ABC( eCategories, eNatures )
{
    vDotationNatures_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data destructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Natures_Data::~ADN_Natures_Data()
{
    // NOTHING
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
    int id = input.attribute< int >( "id", 0 );
    if( !id )
        id = idManager_.GetNextId();
    ADN_Natures_Data::NatureInfos* pNew = new ADN_Natures_Data::NatureInfos( id );
    input >> xml::attribute( "type", *pNew );
    vDotationNatures_.AddItem( pNew );
    idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data::WriteArchive
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Natures_Data::WriteArchive( xml::xostream& output ) const
{
    if( vDotationNatures_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "natures" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "ResourceNatures" );
    for( auto it = vDotationNatures_.begin(); it != vDotationNatures_.end(); ++it )
    {
        output << xml::start( "nature" )
                 << xml::attribute( "type", **it )
                 << xml::attribute( "id", ( *it )->nId_ )
               << xml::end;
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data::CheckDatabaseValidity
// Created: LDC 2014-03-21
// -----------------------------------------------------------------------------
void ADN_Natures_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    for( auto it = vDotationNatures_.begin(); it != vDotationNatures_.end(); ++it )
    {
        if( ( *it )->strName_.GetData().empty() )
            checker.AddError( eInvalidResourceNature, std::string(), eCategories );
    }
}
