// *****************************************************************************
//
// $Created: NLD 2004-04-07 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_RapFor_ABC.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_RapFor_ABC.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Knowledge_RapFor_ABC.h"

#include "Tools/MIL_Tools.h"

const MT_Float DEC_Knowledge_RapFor_ABC::rRapForBoundMin_                        = 0.2;
const MT_Float DEC_Knowledge_RapFor_ABC::rRapForBoundMax_                        = 5.0;
      MT_Float DEC_Knowledge_RapFor_ABC::rRapForIncreasePerTimeStepDefaultValue_ = 0.;

// =============================================================================
// STATIC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapFor_ABC::Initialize
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_RapFor_ABC::Initialize( MIL_InputArchive& archive )
{
    // Rapport de force
    archive.Section( "RapportDeForce" );
    MT_Float rTmp;
    archive.ReadTimeField( "TempsDeRemonteeParDefaut", rTmp );
    rTmp                                    = MIL_Tools::ConvertSecondsToSim       ( rTmp );
    rRapForIncreasePerTimeStepDefaultValue_ = ComputeRapForIncreasePerTimeStepValue( rTmp );
    archive.EndSection();
}

// =============================================================================
// INSTANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapFor_ABC constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_RapFor_ABC::DEC_Knowledge_RapFor_ABC()
    : DEC_Knowledge_ABC    ()
    , nLastCacheUpdateTick_( 0 )
    , rRapForValue_        ( rRapForBoundMax_ )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapFor_ABC destructor
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
DEC_Knowledge_RapFor_ABC::~DEC_Knowledge_RapFor_ABC()
{   
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapFor_ABC::ApplyValue
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
void DEC_Knowledge_RapFor_ABC::ApplyValue( MT_Float rTotalFightScoreFriend, MT_Float rTotalFightScoreEnemy, const MT_Float rRapForIncreasePerTimeStepValue )
{
    // New value calculation
    MT_Float rNewRapForValue = std::numeric_limits< MT_Float >::max();
    if( rTotalFightScoreEnemy != 0. )
        rNewRapForValue = rTotalFightScoreFriend / rTotalFightScoreEnemy;
    
    // Bound the value between 0.2 and 5.0
    static MT_Float rRapForMidValue_ = ( rRapForBoundMax_ - rRapForBoundMin_ ) / 2;
    if( rNewRapForValue < 1. )
        rNewRapForValue = ( (rRapForMidValue_ - rRapForBoundMin_) * rNewRapForValue ) + rRapForBoundMin_;
    else
        rNewRapForValue = std::min( rNewRapForValue + (rRapForMidValue_ - 1), rRapForBoundMax_ );
    
    if( rNewRapForValue <= rRapForValue_ )
        rRapForValue_ = rNewRapForValue;
    else
        rRapForValue_ += std::min( rNewRapForValue - rRapForValue_, rRapForIncreasePerTimeStepValue );
}

