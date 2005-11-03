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

#include "MIL_pch.h"

#include "PHY_Protection.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteState.h"
#include "Tools/MIL_Tools.h"

PHY_Protection::T_ProtectionMap PHY_Protection::protections_;
uint                            PHY_Protection::nNextID_;
MT_Random                       PHY_Protection::random_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Protection::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_Protection::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing protections" );

    // Initialisation des composantes
    archive.BeginList( "Protections" );

    while ( archive.NextListElement() )
    {
        archive.Section( "Protection" );

        std::string strProtection;
        archive.ReadAttribute( "nom", strProtection );

        const PHY_Protection*& pProtection = protections_[ strProtection ];
        if( pProtection )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Protection already defined", archive.GetContext() );

        pProtection = new PHY_Protection( strProtection, archive );

        archive.EndSection(); // Protection
    }
    archive.EndList(); // Protections
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
PHY_Protection::PHY_Protection( const std::string& strName, MIL_InputArchive& archive )
    : strName_                  ( strName )
    , nID_                      ( nNextID_++ )
    , neutralizationTime_       ( )
    , rBreakdownProbabilityEva_ ( 0. )
    , rBreakdownProbabilityNeva_( 0. )
    , humanWoundFactors_        ( PHY_ComposanteState::GetNbrStates(), 0. )
{
    std::string strType;
    archive.ReadAttribute( "type", strType );
    if( sCaseInsensitiveEqual()( strType, "humain" ) )
        nType_ = eHuman;
    else
        nType_ = eMaterial;

    archive.Section( "Neutralisation" );
    MT_Float rTimeVal;
    MT_Float rVariance;   
    archive.ReadTimeAttribute( "tempsMoyen", rTimeVal, CheckValueGreaterOrEqual( 0. ) );
    archive.ReadTimeAttribute( "variance", rVariance );
    rTimeVal            = MIL_Tools::ConvertSecondsToSim( rTimeVal );
    rVariance           = fabs( MIL_Tools::ConvertSecondsToSim( rVariance ) );
    neutralizationTime_ = MT_GaussianRandom( rTimeVal, rVariance );
    archive.EndSection(); // Neutralisation

    //$$$ Ne pas lire quand type == eHuman
    archive.Section( "ProbabilitePanneAleatoire" );
    archive.ReadAttribute( "EVA" , rBreakdownProbabilityEva_ , CheckValueBound( 0., 100. ) );
    archive.ReadAttribute( "NEVA", rBreakdownProbabilityNeva_, CheckValueBound( 0., 100. ) );
    rBreakdownProbabilityEva_  /= 100.;
    rBreakdownProbabilityNeva_ /= 100.;
    archive.EndSection(); // ProbabilitePanneAleatoire

    archive.Section( "BlessureHumainsDansMateriel" );
    
    humanWoundFactors_[ PHY_ComposanteState::undamaged_.GetID() ] = 0.;
    humanWoundFactors_[ PHY_ComposanteState::dead_     .GetID() ] = 1.;
    
    MT_Float rTmp;
    archive.ReadField( "MaterielReparableAvecEvacuation", rTmp, CheckValueBound( 0., 100. ) );
    humanWoundFactors_[ PHY_ComposanteState::repairableWithEvacuation_.GetID() ] = rTmp / 100.;

    archive.ReadField( "MaterielReparableSansEvacuation", rTmp, CheckValueBound( 0., 100. ) );
    humanWoundFactors_[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ] = rTmp / 100.;

    archive.EndSection(); // BlessureHumainsDansMateriel
}

// -----------------------------------------------------------------------------
// Name: PHY_Protection destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_Protection::~PHY_Protection()
{

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
// Name: PHY_Protection::GetHumanWoundFactor
// Created: NLD 2005-03-11
// -----------------------------------------------------------------------------
MT_Float PHY_Protection::GetHumanWoundFactor( const PHY_ComposanteState& composanteState ) const
{
    assert( humanWoundFactors_.size() > composanteState.GetID() );
    return humanWoundFactors_[ composanteState.GetID() ];
}
