// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Categories/PHY_Protection.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 14:04 $
// $Revision: 7 $
// $Workfile: PHY_Protection.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Protection.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteState.h"
#include "tools/Codec.h"
#include "Tools/MIL_Tools.h"
#include "MIL_Random.h"
#include "MT_Tools/MT_Logger.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

PHY_Protection::T_ProtectionMap PHY_Protection::protections_;
unsigned int PHY_Protection::nNextID_ = 0;

// -----------------------------------------------------------------------------
// Name: PHY_Protection::T_HumanEffect constructor
// Created: NLD 2006-07-25
// -----------------------------------------------------------------------------
PHY_Protection::T_HumanEffect::T_HumanEffect()
    : rDeadRatio_   ( 0. )
    , rWoundedRatio_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_Protection::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing protections" );
    xis >> xml::start( "protections" )
            >> xml::list( "protection", &PHY_Protection::ReadProtection )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::ReadProtection
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_Protection::ReadProtection( xml::xistream& xis )
{
    std::string strProtection = xis.attribute< std::string >( "name" );
    const PHY_Protection*& pProtection = protections_[ strProtection ];
    if( pProtection )
        throw MASA_EXCEPTION( xis.context() + "Protection " + strProtection + " already defined" );
     pProtection = new PHY_Protection( strProtection, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::Terminate
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_Protection::Terminate()
{
    for( CIT_ProtectionMap itProtection = protections_.begin(); itProtection != protections_.end(); ++itProtection )
        delete itProtection->second;
    protections_.clear();
    nNextID_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_Protection::PHY_Protection( const std::string& strName, xml::xistream& xis )
    : strName_                  ( strName )
    , nID_                      ( nNextID_++ )
    , neutralizationTime_       ( )
    , rBreakdownProbabilityEva_ ( 0. )
    , rBreakdownProbabilityNeva_( 0. )
    , attritionEffectsOnHumans_ ( PHY_ComposanteState::GetNbrStates(), T_HumanEffect() )
{
    nType_ = boost::iequals( xis.attribute< std::string >( "type" ), "humain" ) ? eHuman : eMaterial;

    std::string timeString, varianceString;
    xis >> xml::start( "neutralization" )
            >> xml::attribute( "average-time", timeString )
            >> xml::attribute( "variance", varianceString )
        >> xml::end;

    double timeVal, variance;
    if( ! tools::DecodeTime( timeString, timeVal ) || timeVal < 0 )
        throw MASA_EXCEPTION( xis.context() + "average-time not defined or < 0" );
    timeVal = MIL_Tools::ConvertSecondsToSim( timeVal );
    if( ! tools::DecodeTime( varianceString, variance ) )
        throw MASA_EXCEPTION( xis.context() + "variance is not defined" );
    neutralizationTime_ = MT_GaussianRandom( timeVal, fabs( MIL_Tools::ConvertSecondsToSim( variance ) ) );

    if( nType_ == eHuman )
    {
        rBreakdownProbabilityEva_  = 0.;
        rBreakdownProbabilityNeva_ = 0.;

        attritionEffectsOnHumans_[ PHY_ComposanteState::dead_                       .GetID() ].rWoundedRatio_ = 0.;
        attritionEffectsOnHumans_[ PHY_ComposanteState::dead_                       .GetID() ].rDeadRatio_    = 1.;
        attritionEffectsOnHumans_[ PHY_ComposanteState::repairableWithEvacuation_   .GetID() ].rWoundedRatio_ = 1.;
        attritionEffectsOnHumans_[ PHY_ComposanteState::repairableWithEvacuation_   .GetID() ].rDeadRatio_    = 0.;
        attritionEffectsOnHumans_[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ].rWoundedRatio_ = 1.;
        attritionEffectsOnHumans_[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ].rDeadRatio_    = 0.;
    }
    else
    {
        xis >> xml::start( "random-breakdown-probability" )
                >> xml::attribute( "eva", rBreakdownProbabilityEva_ )
                >> xml::attribute( "neva", rBreakdownProbabilityNeva_ )
            >> xml::end;

        if( rBreakdownProbabilityEva_ < 0 || rBreakdownProbabilityEva_ > 100 )
            throw MASA_EXCEPTION( xis.context() + "random-breakdown-probability eva not in [0..100]" );
        if( rBreakdownProbabilityNeva_ < 0 || rBreakdownProbabilityNeva_ > 100 )
            throw MASA_EXCEPTION( xis.context() + "random-breakdown-probability neva not in [0..100]" );
        rBreakdownProbabilityEva_  /= 100.;
        rBreakdownProbabilityNeva_ /= 100.;

        xis >> xml::start( "attrition-effects" )
                >> xml::list( "attrition-effect", *this, &PHY_Protection::ReadAttrition )
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::ReadAttrition
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_Protection::ReadAttrition( xml::xistream& xis )
{
    std::string state = xis.attribute< std::string >( "equipment-state" );
    const PHY_ComposanteState* pComposanteState = PHY_ComposanteState::Find( state );
    if( !pComposanteState )
        throw MASA_EXCEPTION( xis.context() + "Unknown composante state" );

    assert( attritionEffectsOnHumans_.size() > pComposanteState->GetID() );

    T_HumanEffect& data = attritionEffectsOnHumans_[ pComposanteState->GetID() ];

    double rTmp = xis.attribute< double >( "injured-percentage" );
    if( rTmp < 0 || rTmp > 100 )
        throw MASA_EXCEPTION( xis.context() + "injured-percentage not in [0..100]" );
    data.rWoundedRatio_ = rTmp / 100.;

    rTmp = xis.attribute< double >( "dead-percentage" );
    if( rTmp < 0 || rTmp > 100 )
        throw MASA_EXCEPTION( xis.context() + "dead-percentage not in [0..100]" );
    data.rDeadRatio_ = rTmp / 100.;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_Protection::~PHY_Protection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::TriggerRandomEvacuationBreakdown
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
bool PHY_Protection::TriggerRandomEvacuationBreakdown() const
{
    return ( 1. - MIL_Random::rand_oi( 0., 1., MIL_Random::eBreakdowns ) <= rBreakdownProbabilityEva_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::TriggerRandomNonEvacuationBreakdown
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
bool PHY_Protection::TriggerRandomNonEvacuationBreakdown() const
{
    return ( 1. - MIL_Random::rand_oi( 0., 1., MIL_Random::eBreakdowns ) <= rBreakdownProbabilityNeva_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::GetHumanDeadRatio
// Created: NLD 2006-07-26
// -----------------------------------------------------------------------------
double PHY_Protection::GetHumanDeadRatio( const PHY_ComposanteState& composanteState ) const
{
    assert( attritionEffectsOnHumans_.size() > composanteState.GetID() );
    return attritionEffectsOnHumans_[ composanteState.GetID() ].rDeadRatio_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::GetHumanWoundedRatio
// Created: NLD 2006-07-26
// -----------------------------------------------------------------------------
double PHY_Protection::GetHumanWoundedRatio( const PHY_ComposanteState& composanteState ) const
{
    assert( attritionEffectsOnHumans_.size() > composanteState.GetID() );
    return attritionEffectsOnHumans_[ composanteState.GetID() ].rWoundedRatio_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::Find
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
const PHY_Protection* PHY_Protection::Find( const std::string& strName )
{
    CIT_ProtectionMap itProtection = protections_.find( strName );
    if( itProtection == protections_.end() )
        return 0;
    return itProtection->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_Protection::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::GetID
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
unsigned int PHY_Protection::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::GetNeutralizationTime
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
unsigned int PHY_Protection::GetNeutralizationTime() const
{
    return static_cast< unsigned int >( neutralizationTime_.rand() );
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::GetProtections
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_Protection::T_ProtectionMap& PHY_Protection::GetProtections()
{
    return protections_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::IsHuman
// Created: NLD 2005-03-11
// -----------------------------------------------------------------------------
bool PHY_Protection::IsHuman() const
{
    return nType_ == eHuman;
}
