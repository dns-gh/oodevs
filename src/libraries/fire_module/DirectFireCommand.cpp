// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "DirectFireCommand.h"
#include <wrapper/View.h>
#include <wrapper/Event.h>
#include <wrapper/Hook.h>

using namespace sword;
using namespace sword::fire;

// -----------------------------------------------------------------------------
// Name: DirectFireCommand constructor
// Created: MCO 2012-03-19
// -----------------------------------------------------------------------------
DirectFireCommand::DirectFireCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& /*model*/ )
    : action_    ( parameters[ "action" ] )
    , identifier_( parameters[ "identifier" ] )
    , role_      ( module )
    , mustReport_( true )
{
    PostCallback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: DirectFireCommand::Destroy
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void DirectFireCommand::Destroy( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    PostCallback( role_.GetFinalReturnCode() );
    wrapper::Event event( "direct fire pion" );
    event[ "entity/identifier" ] = identifier_;
    event[ "running" ] = false;
    event.Post();
}

// -----------------------------------------------------------------------------
// Name: DirectFireCommand::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void DirectFireCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const wrapper::View& entity = model[ "entities" ][ identifier_ ];
    const unsigned int id = entity[ "knowledges" ];
    const unsigned int enemy = parameters[ "enemy" ];
    const wrapper::View& target = model[ "knowledges" ][ id ][ enemy ];
    int nResult = role_.FirePion( model, entity, target, parameters, mustReport_ );
    PostCallback( nResult );
    mustReport_ = false;
}

// -----------------------------------------------------------------------------
// Name: DirectFireCommand::ExecutePaused
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void DirectFireCommand::ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const wrapper::View& entity = model[ "entities" ][ identifier_ ];
    const unsigned int id = entity[ "knowledges" ];
    const unsigned int enemy = parameters[ "enemy" ];
    const wrapper::View& target = model[ "knowledges" ][ id ][ enemy ];
    role_.FirePionSuspended( entity, target, !mustReport_ );
    mustReport_ = true;
}

// -----------------------------------------------------------------------------
// Name: DirectFireCommand::PostCallback
// Created: VPR 2012-02-02
// -----------------------------------------------------------------------------
void DirectFireCommand::PostCallback( int code ) const
{
    wrapper::Event event( "direct fire pion callback" );
    event[ "entity" ] = identifier_;
    event[ "action" ] = action_;
    event[ "code" ] = code;
    event.Post();
}
