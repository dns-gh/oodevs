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
#include "ADN_Equipement_Data.h"
#include "ADN_Project_Data.h"

// -----------------------------------------------------------------------------
// Name: ResourceNetworkInfos::ResourceNetworkInfos
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_Data::ResourceNetworkInfos::ResourceNetworkInfos()
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , strName_    ( "" )
    , strColor_   ( "0xffffff" )
    , nProduction_( 0 )
    , ptrCategory_( ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Energy ).categories_, 0, "" )
{
    strColor_.SetParentNode( *this );
    nProduction_.SetParentNode( *this );
    ptrCategory_.SetParentNode( *this );
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
// Name: ResourceNetworkInfos::GetNodeName
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
std::string ADN_ResourceNetworks_Data::ResourceNetworkInfos::GetNodeName()
{
    std::string strResult( "de la ressource " );
    return strResult + strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkInfos::GetItemName
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
std::string ADN_ResourceNetworks_Data::ResourceNetworkInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::CreateCopy
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_Data::ResourceNetworkInfos* ADN_ResourceNetworks_Data::ResourceNetworkInfos::CreateCopy()
{
    ResourceNetworkInfos* pCopy = new ResourceNetworkInfos();
    pCopy->strName_ = strName_.GetData();
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
    input >> xml::attribute( "name", strName_ )
          >> xml::start( "resource" )
              >> xml::attribute( "id", id )
          >> xml::end
          >> xml::start( "color" )
              >> xml::attribute( "rgb", strColor_ )
          >> xml::end
          >> xml::start( "defaults" )
              >> xml::attribute( "production", nProduction_ )
          >> xml::end;
    ADN_Equipement_Data::T_ResourceInfos_Vector& dotations = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotations();
    for( ADN_Equipement_Data::IT_ResourceInfos_Vector itDotation = dotations.begin(); itDotation != dotations.end(); ++itDotation )
    {
        ADN_Equipement_Data::T_CategoryInfos_Vector& categories = ( *itDotation )->GetCategories();
        for( ADN_Equipement_Data::IT_CategoryInfos_Vector itCat = categories.begin(); itCat != categories.end(); ++itCat )
            if( ( *itCat )->nMosId_ == id )
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
void ADN_ResourceNetworks_Data::ResourceNetworkInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "resource-network" )
               << xml::attribute( "name", strName_ )
               << xml::start( "resource" )
                   << xml::attribute( "id", ptrCategory_.GetData()->nMosId_ )
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
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data destructor
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
ADN_ResourceNetworks_Data::~ADN_ResourceNetworks_Data()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::FilesNeeded
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_Data::FilesNeeded( T_StringList& files ) const
{
    files.push_back(ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szResourceNetworks_.GetData());
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::Reset
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_Data::Reset()
{
    resourceNetworks_.Reset();
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
    IT_ResourceNetworkInfosVector it = std::find_if( resourceNetworks_.begin(), resourceNetworks_.end(), ADN_Tools::NameCmp< ResourceNetworkInfos >( strName ) );
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
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::ReadResourceNetwork
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_Data::ReadResourceNetwork( xml::xistream& input )
{
    std::auto_ptr< ResourceNetworkInfos > spNew( new ResourceNetworkInfos() );
    spNew->ReadArchive( input );
    resourceNetworks_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceNetworks_Data::WriteArchive
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
void ADN_ResourceNetworks_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "resource-networks" );
    ADN_Tools::AddSchema( output, "ResourceNetworks" );
    for( IT_ResourceNetworkInfosVector it = resourceNetworks_.begin(); it != resourceNetworks_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}
