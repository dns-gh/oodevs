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
    : rDestroyed_                 ( 0. )
    , rReparableWithEvacuation_   ( 0. )
    , rReparableWithoutEvacuation_( 0. )
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
    xis >> xml::attribute( "destruction", rDestroyed_ )
        >> xml::attribute( "repairable-with-evacuation", rReparableWithEvacuation_ )
        >> xml::attribute( "repairable-without-evacuation", rReparableWithoutEvacuation_ );

    if( rDestroyed_ < 0. || rDestroyed_ > 1. )
        xis.error( "rDestroyed not in [0..1]" );
    if( rReparableWithEvacuation_ < 0. || rReparableWithEvacuation_ > 1. )
        xis.error( "rReparableWithEvacuation not in [0..1]" );
    if( rReparableWithoutEvacuation_ < 0. || rReparableWithoutEvacuation_ > 1. )
        xis.error( "rReparableWithoutEvacuation not in [0..1]" );
    if( rDestroyed_ + rReparableWithEvacuation_ + rReparableWithoutEvacuation_ > 1. )
        xis.error( "Sum of attrition percentages is out of bound" );

    // Score
    rScore_ = rDestroyed_ + ( rReparableWithEvacuation_ / 2. ) + ( rReparableWithoutEvacuation_ / 4. );
    assert( rScore_ <= 1. );
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_AttritionData::PHY_AttritionData( const PHY_AttritionData& rhs )
    : rDestroyed_                 ( rhs.rDestroyed_                     )
    , rReparableWithEvacuation_   ( rhs.rReparableWithEvacuation_       )
    , rReparableWithoutEvacuation_( rhs.rReparableWithoutEvacuation_    )
    , rScore_                     ( rhs.rScore_                         )
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
const PHY_ComposanteState& PHY_AttritionData::ComputeComposanteState( MT_Float urbanProtection ) const
{
    assert( urbanProtection < 1.f );
    // Tirage de l'état opérationnel
    MT_Float rRand = randomGenerator_.rand_ii();

    MT_Float rReduction = rDestroyed_ * urbanProtection;
    MT_Float rDestroyedBound = rDestroyed_ - rReduction;
    rReduction = ( rReparableWithEvacuation_ + rReduction ) * urbanProtection;
    MT_Float rReparableWithEvacuationBound = rDestroyedBound + rReparableWithEvacuation_ - rReduction;
    rReduction = ( rReparableWithoutEvacuation_ + rReduction ) * urbanProtection;
    MT_Float rReparableWithoutEvacuationBound  = rReparableWithEvacuationBound + rReparableWithoutEvacuation_ - rReduction;

    return rRand <= rDestroyedBound                  ? PHY_ComposanteState::dead_:
           rRand <= rReparableWithEvacuationBound    ? PHY_ComposanteState::repairableWithEvacuation_   :
           rRand <= rReparableWithoutEvacuationBound ? PHY_ComposanteState::repairableWithoutEvacuation_:
                                                        PHY_ComposanteState::undamaged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData::operator=
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_AttritionData& PHY_AttritionData::operator=( const PHY_AttritionData& rhs )
{
    rScore_                      = rhs.rScore_;
    rDestroyed_                  = rhs.rDestroyed_;
    rReparableWithEvacuation_    = rhs.rReparableWithEvacuation_;
    rReparableWithoutEvacuation_ = rhs.rReparableWithoutEvacuation_;
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
