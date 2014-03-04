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

#include "MIL_Time_ABC.h"
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

    int GetSteps( double duration )
    {
        const auto ticks = MIL_Time_ABC::GetTime().GetTickDuration();
        return static_cast< int >( floor( duration / ticks + 0.5 ) );
    }

    double GetOffset( const dotation::PHY_RoleInterface_Dotations& dotations,
                      const PHY_DotationCategory& category,
                      double value, int steps )
    {
        const auto reference = dotations.GetDotationNumber( category );
        const double target = reference * ( 100 + value ) / 100;
        return ( reference - target ) / steps;
    }
}

PHY_ActionConsumeResources::PHY_ActionConsumeResources( MIL_Entity_ABC& unit,
                                                        const PHY_DotationCategory* category,
                                                        double value, double duration )
    : PHY_Action_ABC()
    , dotations_    ( unit.GetRole< dotation::PHY_RoleInterface_Dotations >() )
    , category_     ( GetCategory( category ) )
    , steps_        ( GetSteps( duration ) )
    , offset_       ( GetOffset( dotations_, category_, value, steps_ ) )
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
    steps_--;
    const auto next = dotations_.GetDotationNumber( category_ ) - offset_;
    dotations_.SupplyDotation( category_, next );
}

void PHY_ActionConsumeResources::ExecuteSuspended()
{
    // NOTHING
}

void PHY_ActionConsumeResources::Stop()
{
    // NOTHING
}
