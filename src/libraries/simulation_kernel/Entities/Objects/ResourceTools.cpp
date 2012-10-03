// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ResourceTools.h"
#include "MIL_AgentServer.h"
#include "PHY_ResourceNetworkType.h"
#include "UrbanObjectWrapper.h"
#include "Entities/MIL_EntityManager.h"
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: ResourceTools constructor
// Created: JSR 2010-09-03
// -----------------------------------------------------------------------------
ResourceTools::ResourceTools()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceTools destructor
// Created: JSR 2010-09-03
// -----------------------------------------------------------------------------
ResourceTools::~ResourceTools()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceTools::GetResourceName
// Created: JSR 2010-09-03
// -----------------------------------------------------------------------------
const std::string& ResourceTools::GetResourceName( unsigned long id ) const
{
    const PHY_ResourceNetworkType* resourceNetwork = PHY_ResourceNetworkType::Find( id );
    if( !resourceNetwork )
        throw std::runtime_error( "Bad resource network Id :" + boost::lexical_cast< std::string >( id ) );
    return resourceNetwork->GetName();
}

// -----------------------------------------------------------------------------
// Name: ResourceTools::GetResourceId
// Created: JSR 2010-09-03
// -----------------------------------------------------------------------------
unsigned long ResourceTools::GetResourceId( const std::string& name ) const
{
    const PHY_ResourceNetworkType* resourceNetwork = PHY_ResourceNetworkType::Find( name );
    if( !resourceNetwork )
        throw std::runtime_error( "Bad resource name :" + name );
    return resourceNetwork->GetId();
}

// -----------------------------------------------------------------------------
// Name: ResourceTools::GetIdFromSimulation
// Created: JSR 2011-01-17
// -----------------------------------------------------------------------------
unsigned int ResourceTools::GetIdFromSimulation( unsigned int urbanId ) const
{
    return MIL_AgentServer::GetWorkspace().GetEntityManager().ConvertUrbanIdToSimId( urbanId );
}

// -----------------------------------------------------------------------------
// Name: ResourceTools::ConvertIdToUrbanId
// Created: NPT 2012-09-12
// -----------------------------------------------------------------------------
bool ResourceTools::ConvertIdToUrbanId( unsigned int& id ) const
{
    return MIL_AgentServer::GetWorkspace().GetEntityManager().ConvertIdToUrbanId( id );
}
