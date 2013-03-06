// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SinkRoleExtender.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "MT_Tools/MT_Logger.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: SinkRoleExtender constructor
// Created: AHC 2013-02-07
// -----------------------------------------------------------------------------
SinkRoleExtender::SinkRoleExtender( RoleExtender_ABC* chain )
    : chain_( chain )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SinkRoleExtender:::RegisterRoles
// Created: AHC 2013-02-07
// -----------------------------------------------------------------------------
void SinkRoleExtender::RegisterRoles( MIL_AgentPion& pion )
{
    if( chain_ )
        chain_->RegisterRoles( pion );
    for( auto it = indexes_.begin(); indexes_.end() != it; ++it )
        RegisterRole( pion, *it );
    indexes_.clear();
    roleFactories_.clear();
}

// -----------------------------------------------------------------------------
// Name: SinkRoleExtender::RegisterRole
// Created: MCO 2013-03-05
// -----------------------------------------------------------------------------
void SinkRoleExtender::RegisterRole( MIL_AgentPion& pion, std::size_t idx )
{
    auto it = roleFactories_.find( idx );
    if( it != roleFactories_.end() )
    {
        try
        {
            it->second->RegisterRole( pion );
        }
        catch( const tools::Exception& e )
        {
            MT_LOG_ERROR_MSG( e.CreateLoggerMsg() );
        }
        catch( const std::exception& e )
        {
            MT_LOG_ERROR_MSG( "Can't configure sink ( '" << tools::GetExceptionMsg( e ) << "' )" );
        }
    }
}
