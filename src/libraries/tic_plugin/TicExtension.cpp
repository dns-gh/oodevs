// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_plugin_pch.h"
#include "TicExtension.h"
#include "Platform.h"
#include "dispatcher/Agent.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ComponentType.h"
#include "pathfind/TerrainData.h"
#include <boost/bind.hpp>

using namespace tic;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TicExtension constructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
TicExtension::TicExtension( dispatcher::Agent& holder )
    : holder_( holder )
    , onRoad_( false )
{
    CreatePlatforms();
}

// -----------------------------------------------------------------------------
// Name: TicExtension destructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
TicExtension::~TicExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TicExtension::DoUpdate
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void TicExtension::DoUpdate( const ASN1T_MsgUnitAttributes& asnMsg )
{
    if( asnMsg.m.dotation_eff_materielPresent )
        UpdatePlatforms( asnMsg );
    
    if( asnMsg.m.vitessePresent || asnMsg.m.directionPresent || asnMsg.m.hauteurPresent )
        std::for_each( platforms_.begin(), platforms_.end(), boost::bind( &Platform::Update, _1, asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: TicExtension::DoUpdate
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void TicExtension::DoUpdate( const ASN1T_MsgUnitEnvironmentType& message )
{
    const unsigned int mask = TerrainData::motorway_  | TerrainData::largeroad_  | TerrainData::mediumroad_
                            | TerrainData::smallroad_ | TerrainData::bridge_;
    onRoad_ = ( message.linear & mask ) != 0;
}

// -----------------------------------------------------------------------------
// Name: TicExtension::CreatePlatforms
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void TicExtension::CreatePlatforms()
{
    const AgentType& type = holder_.type_;
    Iterator< const ComponentType& > it = type.CreateIterator();
    while( it.HasMoreElements() )
    {
        const ComponentType& component = it.NextElement();
        unsigned count = type.GetComponentCount( component );
        while( count-- )
            platforms_.push_back( new Platform( component ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TicExtension::UpdatePlatforms
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void TicExtension::UpdatePlatforms( const ASN1T_MsgUnitAttributes& asnMsg )
{
    for( unsigned i = 0; i < asnMsg.dotation_eff_materiel.n; ++i )
        UpdatePlatforms( asnMsg.dotation_eff_materiel.elem[i] );
}

// -----------------------------------------------------------------------------
// Name: TicExtension::UpdatePlatforms
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void TicExtension::UpdatePlatforms( const ASN1T_EquipmentDotations& asnMsg )
{
    ASN1T_EquipmentDotations copy( asnMsg );
    std::for_each( platforms_.begin(), platforms_.end(), boost::bind( &Platform::Spread, _1, boost::ref( copy ) ) );
}
