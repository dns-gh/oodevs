// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ResourceNetworks_Data.h"
#include "ADN_Resources_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_WorkspaceElement.h"
#include "clients_kernel/XmlTranslations.h"

// -----------------------------------------------------------------------------
// Name: ResourceNetworkInfos::ResourceNetworkInfos
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_Data::ResourceNetworkInfos::ResourceNetworkInfos()
    : strColor_   ( "0xffffff" )
    , nProduction_( 0 )
    , ptrCategory_( ADN_Workspace::GetWorkspace().GetResources().GetData().GetNetworkUsableResources(), 0 )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eResourceNetworks, "networks" ) );
    BindExistenceTo( &ptrCategory_ );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkInfos::~ResourceNetworkInfos
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_Data::ResourceNetworkInfos::~ResourceNetworkInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::CreateCopy
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_Data::ResourceNetworkInfos* ADN_ResourceNetworks_Data::ResourceNetworkInfos::CreateCopy()
{
    ResourceNetworkInfos* pCopy = new ResourceNetworkInfos();
    pCopy->strColor_ = strColor_.GetData();
    pCopy->nProduction_ = nProduction_.GetData();
    pCopy->ptrCategory_ = ptrCategory_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkInfos::ReadArchive
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_Data::ResourceNetworkInfos::ReadArchive( xml::xistream& input )
{
    int id;
    input >> xml::attribute( "name", *this )
          >> xml::start( "resource" )
              >> xml::attribute( "id", id )
          >> xml::end
          >> xml::start( "color" )
              >> xml::attribute( "rgb", strColor_ )
          >> xml::end
          >> xml::start( "defaults" )
              >> xml::attribute( "production", nProduction_ )
          >> xml::end;
    ADN_Resources_Data::T_ResourceInfos_Vector& dotations = ADN_Workspace::GetWorkspace().GetResources().GetData().GetResources();
    for( auto itDotation = dotations.begin(); itDotation != dotations.end(); ++itDotation )
    {
        ADN_Resources_Data::T_CategoryInfos_Vector& categories = ( *itDotation )->GetCategories();
        for( auto itCat = categories.begin(); itCat != categories.end(); ++itCat )
            if( ( *itCat )->nId_ == id && ( *itCat )->bNetworkUsable_.GetData() )
            {
                ptrCategory_ = *itCat;
                break;
            }
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkInfos::WriteArchive
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_Data::ResourceNetworkInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "resource-network" )
               << xml::attribute( "name", *this )
               << xml::start( "resource" )
                   << xml::attribute( "id", ptrCategory_.GetData() ? ptrCategory_.GetData()->nId_.GetData() : 0 )
               << xml::end
               << xml::start( "color" )
                   << xml::attribute( "rgb", strColor_ )
               << xml::end
               << xml::start( "defaults" )
                   << xml::attribute( "production", nProduction_ )
               << xml::end
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data constructor
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_Data::ADN_ResourceNetworks_Data()
    : ADN_Data_ABC( eResourceNetworks )
{
    resourceNetworks_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data destructor
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_Data::~ADN_ResourceNetworks_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::FilesNeeded
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szResourceNetworks_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::GetResourceNetworksInfos
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_Data::T_ResourceNetworkInfosVector& ADN_ResourceNetworks_Data::GetResourceNetworksInfos()
{
    return resourceNetworks_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::FindResourceNetwork
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_Data::ResourceNetworkInfos* ADN_ResourceNetworks_Data::FindResourceNetwork( const std::string& strName )
{
    auto it = std::find_if( resourceNetworks_.begin(), resourceNetworks_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == resourceNetworks_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::ReadArchive
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "resource-networks" )
            >> xml::list( "resource-network", *this, &ADN_ResourceNetworks_Data::ReadResourceNetwork )
          >> xml::end;
    resourceNetworks_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::ReadResourceNetwork
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_Data::ReadResourceNetwork( xml::xistream& input )
{
    std::unique_ptr< ResourceNetworkInfos > spNew( new ResourceNetworkInfos() );
    spNew->ReadArchive( input );
    resourceNetworks_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::WriteArchive
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_Data::WriteArchive( xml::xostream& output ) const
{
    if( resourceNetworks_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "resource-networks" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "ResourceNetworks" );
    for( auto it = resourceNetworks_.begin(); it != resourceNetworks_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::GetResourceNetworksThatUse
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_ResourceNetworks_Data::GetResourceNetworksThatUse( ADN_Resources_Data::CategoryInfo& category )
{
    QStringList result;
    for( auto it = resourceNetworks_.begin(); it != resourceNetworks_.end(); ++it )
        if( ( *it )->ptrCategory_.GetData() && ( *it )->ptrCategory_.GetData()->strName_.GetData() == category.strName_.GetData() )
            result << ( *it )->strName_.GetData().c_str();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::CheckDatabaseValidity
// Created: JSR 2013-04-18
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( auto it = resourceNetworks_.begin(); it != resourceNetworks_.end(); ++it )
        ( *it )->ptrCategory_.CheckValidity( checker, ( *it )->strName_.GetData(), eResourceNetworks, -1, tools::translate( "ADN_ResourceNetworks_Data", "Resource" ).toStdString() );
}
