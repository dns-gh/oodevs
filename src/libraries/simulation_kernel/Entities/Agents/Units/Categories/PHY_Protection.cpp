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
#include "tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include "xeumeuleu/xml.h"



PHY_Protection::T_ProtectionMap PHY_Protection::protections_;
uint                            PHY_Protection::nNextID_;
MT_Random                       PHY_Protection::random_;

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

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

struct PHY_Protection::LoadingWrapper
{
    void ReadProtection( xml::xistream& xis )
    {
        PHY_Protection::ReadProtection( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_Protection::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_Protection::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing protections" );

    LoadingWrapper loader;
    xis >> xml::start( "protections" )
            >> xml::list( "protection", loader, &LoadingWrapper::ReadProtection )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::ReadProtection
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_Protection::ReadProtection( xml::xistream& xis )
{
    std::string strProtection;
    xis >> xml::attribute( "name", strProtection );
    
    const PHY_Protection*& pProtection = protections_[ strProtection ];
    if( pProtection )
        xis.error( "Protection " + strProtection + " already defined" );

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
}

// =============================================================================
// INITIALIZATION
// =============================================================================

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
    std::string type;
    xis >> xml::attribute( "type", type );
    nType_ = sCaseInsensitiveEqual()( type, "humain" ) ? eHuman : eMaterial;

    std::string timeString, varianceString;
    xis >> xml::start( "neutralization" )
            >> xml::attribute( "average-time", timeString )
            >> xml::attribute( "variance", varianceString )
        >> xml::end();

    MT_Float timeVal, variance;
    if( ! tools::DecodeTime( timeString, timeVal ) || timeVal < 0 )
        xis.error( "average-time not defined or < 0" );
    if( ! tools::DecodeTime( varianceString, variance ) )
        xis.error( "variance is not defined" );
    neutralizationTime_ = MT_GaussianRandom( timeVal, fabs( variance ) );

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
            >> xml::end();

        if( rBreakdownProbabilityEva_ < 0 || rBreakdownProbabilityEva_ > 100 )
            xis.error( "random-breakdown-probability eva not in [0..100]" );
        if( rBreakdownProbabilityNeva_ < 0 || rBreakdownProbabilityNeva_ > 100 )
            xis.error( "random-breakdown-probability neva not in [0..100]" );
        rBreakdownProbabilityEva_  /= 100.;
        rBreakdownProbabilityNeva_ /= 100.;

        xis >> xml::start( "attrition-effects" )
                >> xml::list( "attrition-effect", *this, &PHY_Protection::ReadAttrition )
            >> xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::ReadAttrition
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_Protection::ReadAttrition( xml::xistream& xis )
{
    std::string state;
    xis >> xml::attribute( "equipment-state", state );
    const PHY_ComposanteState* pComposanteState = PHY_ComposanteState::Find( state );
    if( !pComposanteState )
        xis.error( "Unknown composante state" );

    assert( attritionEffectsOnHumans_.size() > pComposanteState->GetID() );

    T_HumanEffect& data = attritionEffectsOnHumans_[ pComposanteState->GetID() ];

    MT_Float rTmp;
    xis >> xml::attribute( "injured-percentage", rTmp );
    if( rTmp < 0 || rTmp > 100 )
        xis.error( "injured-percentage not in [0..100]" );
    data.rWoundedRatio_ = rTmp / 100.;
        
    xis >> xml::attribute( "dead-percentage", rTmp );
    if( rTmp < 0 || rTmp > 100 )
        xis.error( "dead-percentage not in [0..100]" );
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

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Protection::CanRandomlyBreaksDownEva
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
bool PHY_Protection::CanRandomlyBreaksDownEva() const
{
    return ( random_.rand_oi( 0., 1. ) <= rBreakdownProbabilityEva_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::CanRandomlyBreaksDownNeva
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
bool PHY_Protection::CanRandomlyBreaksDownNeva() const
{
    return ( random_.rand_oi( 0., 1. ) <= rBreakdownProbabilityNeva_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::GetHumanDeadRatio
// Created: NLD 2006-07-26
// -----------------------------------------------------------------------------
MT_Float PHY_Protection::GetHumanDeadRatio( const PHY_ComposanteState& composanteState ) const
{
    assert( attritionEffectsOnHumans_.size() > composanteState.GetID() );
    return attritionEffectsOnHumans_[ composanteState.GetID() ].rDeadRatio_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection::GetHumanWoundedRatio
// Created: NLD 2006-07-26
// -----------------------------------------------------------------------------
MT_Float PHY_Protection::GetHumanWoundedRatio( const PHY_ComposanteState& composanteState ) const
{
    assert( attritionEffectsOnHumans_.size() > composanteState.GetID() );
    return attritionEffectsOnHumans_[ composanteState.GetID() ].rWoundedRatio_;
}
