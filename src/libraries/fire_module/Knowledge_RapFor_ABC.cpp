// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Knowledge_RapFor_ABC.h"
#include "tools/Codec.h" // $$$$ MCO 2012-05-15: refactor dependency
#include <wrapper/Hook.h>
#include <module_api/Log.h>
#include <xeumeuleu/xml.h>

using namespace sword;
using namespace sword::fire;

namespace
{
    DEFINE_HOOK( InitializeDecisional, void, ( const char* xml, double tickDuration ) )
    {
        try
        {
            // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
            xml::xistringstream xis( xml );
            xis >> xml::start( "decisional" );
            Knowledge_RapFor_ABC::Initialize( xis, tickDuration );
        }
        catch( std::exception& e )
        {
            ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
        }
        catch( ... )
        {
            ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown exception during force ratio initialization" );
        }
        if( GET_PREVIOUS_HOOK( InitializeDecisional ) )
            GET_PREVIOUS_HOOK( InitializeDecisional )( xml, tickDuration );
    }

    const double rRapForBoundMin = 0.2;
    const double rRapForBoundMax = 5;

    double ComputeRapForIncreasePerTimeStepValue( double rBaseTimeValue )
    {
        if( rBaseTimeValue <= 0. )
            return rRapForBoundMax - rRapForBoundMin;
        return ( rRapForBoundMax - rRapForBoundMin ) / rBaseTimeValue;
    }
}

double Knowledge_RapFor_ABC::rRapForIncreasePerTimeStepDefaultValue_ = 0;

// -----------------------------------------------------------------------------
// Name: Knowledge_RapFor_ABC::Initialize
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void Knowledge_RapFor_ABC::Initialize( xml::xisubstream xis, double tickDuration )
{
    xis >> xml::start( "force-ratio" );
    double rTmp = 0;
    tools::ReadTimeAttribute( xis, "default-feedback-time", rTmp );
    rRapForIncreasePerTimeStepDefaultValue_ = ComputeRapForIncreasePerTimeStepValue( rTmp / tickDuration );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapFor_ABC constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
Knowledge_RapFor_ABC::Knowledge_RapFor_ABC()
    : nLastCacheUpdateTick_( 0 )
    , rRapForValue_        ( rRapForBoundMax )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapFor_ABC destructor
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
Knowledge_RapFor_ABC::~Knowledge_RapFor_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapFor_ABC::ApplyValue
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
void Knowledge_RapFor_ABC::ApplyValue( double rTotalFightScoreFriend, double rTotalFightScoreEnemy, double rFeedbackTime )
{
    // New value calculation
    double rNewRapForValue = std::numeric_limits< double >::max();
    if( rTotalFightScoreEnemy != 0. )
        rNewRapForValue = rTotalFightScoreFriend / rTotalFightScoreEnemy;
    // Bound the value between 0.2 and 5.0
    static double rRapForMidValue_ = ( rRapForBoundMax - rRapForBoundMin ) / 2;
    if( rNewRapForValue < 1. )
        rNewRapForValue = ( (rRapForMidValue_ - rRapForBoundMin) * rNewRapForValue ) + rRapForBoundMin;
    else
        rNewRapForValue = std::min( rNewRapForValue + (rRapForMidValue_ - 1), rRapForBoundMax );
    if( rNewRapForValue <= rRapForValue_ )
        rRapForValue_ = rNewRapForValue;
    else
        rRapForValue_ += std::min( rNewRapForValue - rRapForValue_, ComputeRapForIncreasePerTimeStepValue( rFeedbackTime ) );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapFor_ABC::GetValue
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
double Knowledge_RapFor_ABC::GetValue( const wrapper::View& model, const wrapper::View& entity )
{
    Update( model, entity );
    return rRapForValue_;
}
