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
DirectFireCommand::DirectFireCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& /*model*/, std::size_t identifier )
    : commandIdentifier_          ( identifier )
    , identifier_                 ( parameters[ "identifier" ] )
    , enemy_                      ( parameters[ "enemy" ] )
    , role_                       ( module )
    , rPercentageComposantesToUse_( std::max( 0., std::min< double >( 1., parameters[ "percentage" ] ) ) )
    , nFiringMode_                ( static_cast< DirectFireData::E_FiringMode >( static_cast< int >( parameters[ "mode" ] ) ) )
    , firingType_                 ( parameters[ "type" ] )
    , major_                      ( parameters[ "major" ] )
    , ammoDotationClass_          ( parameters[ "dotation" ] )
    , mustReport_                 ( true )
{
    PostCallback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: DirectFireCommand destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
DirectFireCommand::~DirectFireCommand()
{
    PostCallback( role_.GetFinalReturnCode() );
    wrapper::Event event( "direct fire pion" );
    event[ "entity" ] = identifier_;
    event[ "running" ] = false;
    event.Post();
}

// -----------------------------------------------------------------------------
// Name: DirectFireCommand::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void DirectFireCommand::Execute( const wrapper::View& /*parameters*/, const wrapper::View& model ) const
{
    const wrapper::View& entity = model[ "entities" ][ identifier_ ];
    const unsigned int id = entity[ "knowledges" ];
    const wrapper::View& enemy = model[ "knowledges" ][ id ][ enemy_ ];
    int nResult = role_.FirePion( model, entity, enemy, nFiringMode_, rPercentageComposantesToUse_, firingType_, major_, mustReport_, ammoDotationClass_ );
    PostCallback( nResult );
    mustReport_ = false;
}

// -----------------------------------------------------------------------------
// Name: DirectFireCommand::ExecutePaused
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void DirectFireCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& model ) const
{
    const wrapper::View& entity = model[ "entities" ][ identifier_ ];
    const unsigned int id = entity[ "knowledges" ];
    const wrapper::View& enemy = model[ "knowledges" ][ id ][ enemy_ ];
    role_.FirePionSuspended( entity, enemy, !mustReport_ );
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
    event[ "id" ] = commandIdentifier_;
    event[ "code" ] = code;
    event.Post();
}
