// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionConsumeResources.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_PathWalker.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"

#include <boost/lexical_cast.hpp>

namespace
{
    const PHY_DotationType& GetType( unsigned int value )
    {
        auto type = PHY_DotationType::FindDotationType( value );
        if( !type )
            throw MASA_EXCEPTION( "invalid dotation type " + boost::lexical_cast< std::string >( value ) );
        return *type;
    }

    int GetSteps( double duration, unsigned int tickDuration )
    {
        if( duration < 0 )
            throw MASA_EXCEPTION( "invalid negative duration" );
        return std::max( 1, static_cast< int >( std::floor( duration / tickDuration + 0.5 ) ) );
    }
}

PHY_ActionConsumeResources::PHY_ActionConsumeResources( MIL_Entity_ABC& unit,
                                                        unsigned int type,
                                                        double value, double duration,
                                                        unsigned int tickDuration )
    : PHY_DecisionCallbackAction_ABC( unit )
    , dotations_( unit.GetRole< dotation::PHY_RoleInterface_Dotations >() )
    , type_     ( GetType( type ) )
    , steps_    ( GetSteps( duration, tickDuration ) )
    , fraction_ ( value / steps_ / 100 )
{
    Callback( static_cast< int >( DEC_PathWalker::eRunning ) );
}

PHY_ActionConsumeResources::~PHY_ActionConsumeResources()
{
    // NOTHING
}

void PHY_ActionConsumeResources::Execute()
{
    if( !steps_ )
        return;
    --steps_;
    dotations_.ChangeDotation( type_, fraction_ );
    if( !steps_ )
        Callback( static_cast< int >( DEC_PathWalker::eFinished ) );
}

void PHY_ActionConsumeResources::ExecuteSuspended()
{
    // NOTHING
}

void PHY_ActionConsumeResources::StopAction()
{
    // NOTHING
}
