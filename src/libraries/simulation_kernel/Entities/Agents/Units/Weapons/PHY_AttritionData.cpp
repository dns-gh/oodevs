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
#include "MIL_Random.h"

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_AttritionData::PHY_AttritionData()
    : rDestroyed_( 0. )
    , rReparableWithEvacuation_( 0. )
    , rReparableWithoutEvacuation_( 0. )
    , rScore_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_AttritionData::PHY_AttritionData( xml::xistream& xis )
    : rDestroyed_( 0. )
    , rReparableWithEvacuation_( 0. )
    , rReparableWithoutEvacuation_( 0. )
    , rScore_( 0. )
{
    xis >> xml::attribute( "destruction", rDestroyed_ )
        >> xml::attribute( "repairable-with-evacuation", rReparableWithEvacuation_ )
        >> xml::attribute( "repairable-without-evacuation", rReparableWithoutEvacuation_ );

    if( rDestroyed_ < 0. || rDestroyed_ > 1. )
        throw MASA_EXCEPTION( xis.context() + "rDestroyed not in [0..1]" );
    if( rReparableWithEvacuation_ < 0. || rReparableWithEvacuation_ > 1. )
        throw MASA_EXCEPTION( xis.context() + "rReparableWithEvacuation not in [0..1]" );
    if( rReparableWithoutEvacuation_ < 0. || rReparableWithoutEvacuation_ > 1. )
        throw MASA_EXCEPTION( xis.context() + "rReparableWithoutEvacuation not in [0..1]" );
    if( rDestroyed_ + rReparableWithEvacuation_ + rReparableWithoutEvacuation_ > 1. )
        throw MASA_EXCEPTION( xis.context() + "Sum of attrition percentages is out of bound" );

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
// Name: PHY_AttritionData constructor
// Created: MMC 2011-03-28
// -----------------------------------------------------------------------------
PHY_AttritionData::PHY_AttritionData( const PHY_AttritionData& armedAttritionData, const PHY_AttritionData& unarmedAttritionData, double armedIndividuals )
{
    rDestroyed_ = unarmedAttritionData.rDestroyed_ + ( armedAttritionData.rDestroyed_ - unarmedAttritionData.rDestroyed_ ) * armedIndividuals;
    rReparableWithEvacuation_ = unarmedAttritionData.rReparableWithEvacuation_ + ( armedAttritionData.rReparableWithEvacuation_ - unarmedAttritionData.rReparableWithEvacuation_ ) * armedIndividuals;
    rReparableWithoutEvacuation_ = unarmedAttritionData.rReparableWithoutEvacuation_ + ( armedAttritionData.rReparableWithoutEvacuation_ - unarmedAttritionData.rReparableWithoutEvacuation_ ) * armedIndividuals;

    // Score
    rScore_ = rDestroyed_ + ( rReparableWithEvacuation_ / 2. ) + ( rReparableWithoutEvacuation_ / 4. );
    assert( rScore_ <= 1. );
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
const PHY_ComposanteState& PHY_AttritionData::ComputeComposanteState( double urbanProtection ) const
{
    assert( urbanProtection < 1.f );
    // Tirage de l'état opérationnel
    double rRand = 1. - MIL_Random::rand_ii( MIL_Random::eFire );

    double rReductionDestroyed = rDestroyed_ * urbanProtection;
    double rNewDestroyed = rDestroyed_ - rReductionDestroyed;

    double rReductionReparableWithEvacuation = ( rReparableWithEvacuation_ + rReductionDestroyed ) * urbanProtection;
    double rNewReductionReparableWithEvacuation = rReparableWithEvacuation_ + rReductionDestroyed - rReductionReparableWithEvacuation;

    double rReductionReparableWithoutEvacuation = ( rReparableWithoutEvacuation_ + rReductionReparableWithEvacuation ) * urbanProtection;
    double rNewReductionReparableWithoutEvacuation = rReparableWithoutEvacuation_ + rReductionReparableWithEvacuation - rReductionReparableWithoutEvacuation;

    double rDestroyedBound = rNewDestroyed;
    double rReparableWithEvacuationBound = rDestroyedBound + rNewReductionReparableWithEvacuation;
    double rReparableWithoutEvacuationBound = rReparableWithEvacuationBound + rNewReductionReparableWithoutEvacuation;

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
double PHY_AttritionData::GetScore() const
{
    return rScore_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData::GetDestroyed
// Created: LGY 2012-02-13
// -----------------------------------------------------------------------------
double PHY_AttritionData::GetDestroyed() const
{
    return rDestroyed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData::GetReparableWithEvacuation
// Created: LGY 2012-02-13
// -----------------------------------------------------------------------------
double PHY_AttritionData::GetReparableWithEvacuation() const
{
    return rReparableWithEvacuation_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AttritionData::GetReparableWithoutEvacuation
// Created: LGY 2012-02-13
// -----------------------------------------------------------------------------
double PHY_AttritionData::GetReparableWithoutEvacuation() const
{
    return rReparableWithoutEvacuation_;
}
