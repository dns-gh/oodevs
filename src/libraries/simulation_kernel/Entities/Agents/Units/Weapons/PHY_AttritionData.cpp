// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_AttritionData.cpp $
// $Author: Jvt $
// $Modtime: 19/10/04 18:30 $
// $Revision: 2 $
// $Workfile: PHY_AttritionData.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_AttritionData.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteState.h"
#include <xeumeuleu/xml.h>

MT_Random PHY_AttritionData::randomGenerator_;

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_AttritionData::PHY_AttritionData()
    : rDestroyedBound_                 ( 0. )
    , rReparableWithEvacuationBound_   ( 0. )
    , rReparableWithoutEvacuationBound_( 0. )
    , rScore_                          ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_AttritionData::PHY_AttritionData( xml::xistream& xis )
{
    MT_Float rDestroyed;
    MT_Float rReparableWithEvacuation;
    MT_Float rReparableWithoutEvacuation;

    xis >> xml::attribute( "destruction", rDestroyed )
        >> xml::attribute( "repairable-with-evacuation", rReparableWithEvacuation )
        >> xml::attribute( "repairable-without-evacuation", rReparableWithoutEvacuation );

    if( rDestroyed < 0. || rDestroyed > 1. )
        xis.error( "rDestroyed not in [0..1]" );
    if( rReparableWithEvacuation < 0. || rReparableWithEvacuation > 1. )
        xis.error( "rReparableWithEvacuation not in [0..1]" );
    if( rReparableWithoutEvacuation < 0. || rReparableWithoutEvacuation > 1. )
        xis.error( "rReparableWithoutEvacuation not in [0..1]" );

    rDestroyedBound_                    = rDestroyed;
    rReparableWithEvacuationBound_      = rDestroyedBound_ + rReparableWithEvacuation;
    rReparableWithoutEvacuationBound_   = rReparableWithEvacuationBound_ + rReparableWithoutEvacuation;

    if( rReparableWithoutEvacuationBound_ > 1. )
        xis.error( "Sum of attrition percentages is out of bound" );

    // Score
    rScore_ = rDestroyed + ( rReparableWithEvacuation / 2. ) + ( rReparableWithoutEvacuation / 4. );
    assert( rScore_ <= 1. );
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_AttritionData::PHY_AttritionData( const PHY_AttritionData& rhs )
    : rDestroyedBound_                 ( rhs.rDestroyedBound_                  )
    , rReparableWithEvacuationBound_   ( rhs.rReparableWithEvacuationBound_    )
    , rReparableWithoutEvacuationBound_( rhs.rReparableWithoutEvacuationBound_ )
    , rScore_                          ( rhs.rScore_                           )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_AttritionData::~PHY_AttritionData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData::ComputeComposanteState
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
const PHY_ComposanteState& PHY_AttritionData::ComputeComposanteState() const
{
    // Tirage de l'état opérationnel
    MT_Float rRand = randomGenerator_.rand_ii();
    
    return rRand <= rDestroyedBound_                  ? PHY_ComposanteState::dead_:
           rRand <= rReparableWithEvacuationBound_    ? PHY_ComposanteState::repairableWithEvacuation_   :
           rRand <= rReparableWithoutEvacuationBound_ ? PHY_ComposanteState::repairableWithoutEvacuation_:
                                                        PHY_ComposanteState::undamaged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData::operator=
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_AttritionData& PHY_AttritionData::operator=( const PHY_AttritionData& rhs )
{
    rScore_                           = rhs.rScore_;
    rDestroyedBound_                  = rhs.rDestroyedBound_;
    rReparableWithEvacuationBound_    = rhs.rReparableWithEvacuationBound_;
    rReparableWithoutEvacuationBound_ = rhs.rReparableWithoutEvacuationBound_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData::GetScore
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
MT_Float PHY_AttritionData::GetScore() const
{
    return rScore_;
}
