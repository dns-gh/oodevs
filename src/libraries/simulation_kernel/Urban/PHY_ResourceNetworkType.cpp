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
#include <boost/ptr_container/ptr_map.hpp>

namespace
{
    boost::ptr_map< std::string, PHY_ResourceNetworkType > resourceNetworks;
    unsigned int nNextId;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::Initialize
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
void PHY_ResourceNetworkType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing resource networks" );
    xis >> xml::start( "resource-networks" )
            >> xml::list( "resource-network", &PHY_ResourceNetworkType::ReadResourceNetwork )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::Terminate
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
void PHY_ResourceNetworkType::Terminate()
{
    resourceNetworks.clear();
    nNextId = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::Find
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
const PHY_ResourceNetworkType* PHY_ResourceNetworkType::Find( const std::string& strName )
{
    auto it = resourceNetworks.find( strName );
    if( it == resourceNetworks.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::Find
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
const PHY_ResourceNetworkType* PHY_ResourceNetworkType::Find( unsigned int id )
{
    for( auto it = resourceNetworks.begin(); it != resourceNetworks.end(); ++it )
        if( it->second->GetId() == id )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ResourceNetworkType::FindByDotation
// Created: BCI 2011-02-21
// -----------------------------------------------------------------------------
const PHY_ResourceNetworkType* PHY_ResourceNetworkType::FindByDotation( const PHY_DotationCategory& dotation )
{
    for( auto it = resourceNetworks.begin(); it != resourceNetworks.end(); ++it )
        if( &it->second->GetDotationCategory() == &dotation )
            return it->second;
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
    , nId_             ( nNextId++ )
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
    std::string strResourceNetwork = xis.attribute< std::string >( "name" );
    if( resourceNetworks.count( strResourceNetwork ) )
        throw MASA_EXCEPTION( xis.context() + "Resource network " + strResourceNetwork + " already defined" );
    unsigned int resourceId;
    xis >> xml::start( "resource" )
        >> xml::attribute( "id", resourceId )
        >> xml::end;
    const PHY_DotationCategory* dotationCategory = PHY_DotationType::FindDotationCategory( resourceId );
    if( !dotationCategory )
        throw MASA_EXCEPTION( xis.context() + "Unknown dotation category Id: " + boost::lexical_cast< std::string >( resourceId ) );
    auto next = std::auto_ptr< PHY_ResourceNetworkType >( new PHY_ResourceNetworkType( strResourceNetwork, *dotationCategory ) );
    resourceNetworks.insert( strResourceNetwork, next );
}
