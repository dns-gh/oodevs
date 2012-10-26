// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "DirectFireCommandPopulation.h"
#include <wrapper/View.h>
#include <wrapper/Event.h>
#include <wrapper/Hook.h>

using namespace sword;
using namespace sword::fire;

// -----------------------------------------------------------------------------
// Name: DirectFireCommandPopulation constructor
// Created: MCO 2012-03-19
// -----------------------------------------------------------------------------
DirectFireCommandPopulation::DirectFireCommandPopulation( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& /*model*/ )
    : action_    ( parameters[ "action" ] )
    , identifier_( parameters[ "identifier" ] )
    , role_      ( module )
{
    PostCallback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: DirectFireCommandPopulation destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
DirectFireCommandPopulation::~DirectFireCommandPopulation()
{
    PostCallback( role_.GetFinalReturnCode() );
    wrapper::Event event( "direct fire population" );
    event[ "entity/identifier" ] = identifier_;
    event[ "running" ] = false;
    event.Post();
}

// -----------------------------------------------------------------------------
// Name: DirectFireCommandPopulation::Destroy
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void DirectFireCommandPopulation::Destroy( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // $$$$ MCO 2012-10-26: move code from destructor in here and use "entity" in "direct fire pion"
}

// -----------------------------------------------------------------------------
// Name: DirectFireCommandPopulation::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void DirectFireCommandPopulation::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const wrapper::View& entity = model[ "entities" ][ identifier_ ];
    const wrapper::View& target = parameters[ "population" ];
    int nResult = role_.FirePopulation( model, entity, target, parameters );
    PostCallback( nResult );
}

// -----------------------------------------------------------------------------
// Name: DirectFireCommandPopulation::ExecutePaused
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void DirectFireCommandPopulation::ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const wrapper::View& entity = model[ "entities" ][ identifier_ ];
    const wrapper::View& target = parameters[ "population" ];
    role_.FirePopulationSuspended( entity, target );
}

// -----------------------------------------------------------------------------
// Name: DirectFireCommandPopulation::PostCallback
// Created: VPR 2012-02-02
// -----------------------------------------------------------------------------
void DirectFireCommandPopulation::PostCallback( int code ) const
{
    wrapper::Event event( "direct fire population callback" );
    event[ "entity" ] = identifier_;
    event[ "action" ] = action_;
    event[ "code" ] = code;
    event.Post();
}
