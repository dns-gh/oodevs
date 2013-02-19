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

SinkRoleExtender::SinkRoleExtender( RoleExtender_ABC* chain )
    : chain_( chain )
{
    // NOTHING
}

void SinkRoleExtender::RegisterRoles( MIL_AgentPion& pion )
{
    if( chain_ )
        chain_->RegisterRoles( pion );
    for( auto it=indexes_.begin(); indexes_.end() != it; ++it )
    {
        std::size_t idx = *it;
        T_RoleFactories::iterator itFact( roleFactories_.find( idx ) );
        if( roleFactories_.end() != itFact )
        {
            try
            {
                itFact->second->RegisterRole( pion );
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
    indexes_.clear();
    roleFactories_.clear();
}
