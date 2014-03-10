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
#include "Entities/MIL_Entity_ABC.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"

namespace
{
    const PHY_DotationCategory& GetCategory( const PHY_DotationCategory* category )
    {
        if( !category )
            throw MASA_EXCEPTION( "invalid dotation category" );
        return *category;
    }

    int GetSteps( double duration, unsigned tickDuration )
    {
        if( duration == 0 )
            throw MASA_EXCEPTION( "invalid null duration" );
        return static_cast< int >( floor( duration / tickDuration + 0.5 ) );
    }
}

PHY_ActionConsumeResources::PHY_ActionConsumeResources( MIL_Entity_ABC& unit,
                                                        const PHY_DotationCategory* category,
                                                        double value, double duration,
                                                        unsigned tickDuration )
    : dotations_( unit.GetRole< dotation::PHY_RoleInterface_Dotations >() )
    , category_ ( GetCategory( category ) )
    , steps_    ( GetSteps( duration, tickDuration ) )
    , fraction_ ( value / steps_ / 100 )
{
    // NOTHING
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
    dotations_.ChangeDotation( category_, fraction_ );
}

void PHY_ActionConsumeResources::ExecuteSuspended()
{
    // NOTHING
}

void PHY_ActionConsumeResources::Stop()
{
    // NOTHING
}
