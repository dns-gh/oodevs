// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ResourceNetworkType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.hpp>

PHY_ResourceNetworkType::T_ResourceNetworkMap PHY_ResourceNetworkType::resourceNetworks_;
unsigned int PHY_ResourceNetworkType::nNextId_ = 0;

struct PHY_ResourceNetworkType::LoadingWrapper
{
    void ReadResourceNetwork( xml::xistream& xis )
    {
        PHY_ResourceNetworkType::ReadResourceNetwork( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::Initialize
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
void PHY_ResourceNetworkType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing resource networks" );
    LoadingWrapper loader;
    xis >> xml::start( "resource-networks" )
            >> xml::list( "resource-network", loader, &LoadingWrapper::ReadResourceNetwork )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::Terminate
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
void PHY_ResourceNetworkType::Terminate()
{
    for( CIT_ResourceNetworkMap itResource = resourceNetworks_.begin(); itResource != resourceNetworks_.end(); ++itResource )
        delete itResource->second;
    resourceNetworks_.clear();
    nNextId_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::GetResourceNetworks
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
const PHY_ResourceNetworkType::T_ResourceNetworkMap& PHY_ResourceNetworkType::GetResourceNetworks()
{
    return resourceNetworks_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::Find
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
const PHY_ResourceNetworkType* PHY_ResourceNetworkType::Find( const std::string& strName )
{
    CIT_ResourceNetworkMap itResource = resourceNetworks_.find( strName );
    if( itResource == resourceNetworks_.end() )
        return 0;
    return itResource->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::Find
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
const PHY_ResourceNetworkType* PHY_ResourceNetworkType::Find( unsigned int id )
{
    for( CIT_ResourceNetworkMap it = resourceNetworks_.begin(); it != resourceNetworks_.end(); ++it )
    {
        if( it->second->GetId() == id )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::GetName
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
const std::string& PHY_ResourceNetworkType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::GetId
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
unsigned int PHY_ResourceNetworkType::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::GetDotationCategory
// Created: JSR 2011-02-02
// -----------------------------------------------------------------------------
const PHY_DotationCategory& PHY_ResourceNetworkType::GetDotationCategory() const
{
    return dotationCategory_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType constructor
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
PHY_ResourceNetworkType::PHY_ResourceNetworkType( const std::string& strName, const PHY_DotationCategory& dotationCategory )
    : strName_         ( strName )
    , dotationCategory_( dotationCategory )
    , nId_             ( nNextId_++ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType destructor
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
PHY_ResourceNetworkType::~PHY_ResourceNetworkType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::ReadResourceNetwork
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
void PHY_ResourceNetworkType::ReadResourceNetwork( xml::xistream& xis )
{
    std::string strResourceNetwork;
    xis >> xml::attribute( "name", strResourceNetwork );
    const PHY_ResourceNetworkType*& pResourceNetwork = resourceNetworks_[ strResourceNetwork ];
    if( pResourceNetwork )
        xis.error( "Resource network " + strResourceNetwork + " already defined" );
    unsigned int resourceId;
    xis >> xml::start( "resource" )
        >> xml::attribute( "id", resourceId )
        >> xml::end;
    const PHY_DotationCategory* dotationCategory = PHY_DotationType::FindDotationCategory( resourceId );
    if( !dotationCategory )
        xis.error( "Unknown dotation category Id: " + boost::lexical_cast< std::string >( resourceId ) );
    pResourceNetwork = new PHY_ResourceNetworkType( strResourceNetwork, *dotationCategory );
}
