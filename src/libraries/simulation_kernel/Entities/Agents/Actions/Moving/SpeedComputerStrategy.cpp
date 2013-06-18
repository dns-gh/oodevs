// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SpeedComputerStrategy.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Knowledge/DEC_Knowledge_PopulationCollision.h"
#include "PHY_RoleAction_InterfaceMoving.h"
#include <boost/bind.hpp>

using namespace moving;

namespace
{
    double MaxSpeedEnvObj( const PHY_ComposantePion& comp, const TerrainData& environment, const MIL_Object_ABC& object )
    {
        return std::min( comp.GetMaxSpeed( environment ), comp.GetMaxSpeed( object ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SpeedComputerStrategy constructor
// Created: LDC 2009-12-16
// -----------------------------------------------------------------------------
SpeedComputerStrategy::SpeedComputerStrategy( bool isMaxSpeed, bool withReinforcement, const MIL_Object_ABC& obj, const TerrainData* env /* =0*/ )
    : withReinforcement_( withReinforcement )
    , isMax_            ( isMaxSpeed )
{
    if( env )
    {
        compFunctor_ = boost::bind( &MaxSpeedEnvObj, _1, boost::cref( *env ), boost::cref( obj ) );
        if( isMaxSpeed )
            pionFunctor_ = boost::mem_fn( &PHY_RoleAction_InterfaceMoving::GetMaxSpeedWithReinforcement );
        else
            pionFunctor_ = boost::bind( &PHY_RoleAction_InterfaceMoving::GetSpeedWithReinforcement, _1, boost::cref( *env ), boost::cref( obj ) );
    }
    else
    {
        compFunctor_ = boost::bind< double, PHY_ComposantePion, const MIL_Object_ABC& >( &PHY_ComposantePion::GetMaxSpeed, _1, boost::cref( obj ) );
        pionFunctor_ = boost::mem_fn( &PHY_RoleAction_InterfaceMoving::GetMaxSpeedWithReinforcement );
    }
}

// -----------------------------------------------------------------------------
// Name: SpeedComputerStrategy constructor
// Created: LDC 2009-12-16
// -----------------------------------------------------------------------------
SpeedComputerStrategy::SpeedComputerStrategy( bool isMaxSpeed, bool withReinforcement, const TerrainData* env /* =0*/ )
    : withReinforcement_( withReinforcement )
    , isMax_            ( isMaxSpeed )
{
    if( env )
    {
        compFunctor_ = boost::bind< double, PHY_ComposantePion, const TerrainData& >( &PHY_ComposantePion::GetMaxSpeed, _1, boost::cref( *env ) );
        if( isMaxSpeed )
            pionFunctor_ = boost::mem_fn( &PHY_RoleAction_InterfaceMoving::GetMaxSpeedWithReinforcement );
        else
            pionFunctor_ = boost::bind( &PHY_RoleAction_InterfaceMoving::GetSpeedWithReinforcement, _1, boost::cref( *env ) );
    }
    else
    {
        compFunctor_ = boost::mem_fn( &PHY_ComposantePion::GetMaxSpeed );
        pionFunctor_ = boost::mem_fn( &PHY_RoleAction_InterfaceMoving::GetMaxSpeedWithReinforcement );
    }
}

// -----------------------------------------------------------------------------
// Name: SpeedComputerStrategy destructor
// Created: LDC 2009-12-16
// -----------------------------------------------------------------------------
SpeedComputerStrategy::~SpeedComputerStrategy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpeedComputerStrategy::ApplyOnComponent
// Created: LDC 2009-12-16
// -----------------------------------------------------------------------------
double SpeedComputerStrategy::ApplyOnComponent( const PHY_ComposantePion& comp ) const
{
    return compFunctor_( comp );
}

// -----------------------------------------------------------------------------
// Name: SpeedComputerStrategy::ApplyOnReinforcement
// Created: LDC 2009-12-16
// -----------------------------------------------------------------------------
double SpeedComputerStrategy::ApplyOnReinforcement( const MIL_Agent_ABC& pion ) const
{
    return withReinforcement_ ?
        pionFunctor_( pion.GetRole< PHY_RoleAction_InterfaceMoving >() ) :
            std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: SpeedComputerStrategy::ApplyOnPopulation
// Created: LDC 2009-12-16
// -----------------------------------------------------------------------------
double SpeedComputerStrategy::ApplyOnPopulation(const DEC_Knowledge_PopulationCollision& population ) const
{
    return isMax_ ? population.GetPionMaxSpeed() : std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: SpeedComputerStrategy::AddModifier
// Created: LDC 2009-12-16
// -----------------------------------------------------------------------------
double SpeedComputerStrategy::AddModifier( double ratio, bool isMax /*=true*/ ) const
{
    return isMax == isMax_ ? ratio : 1.0;
}
