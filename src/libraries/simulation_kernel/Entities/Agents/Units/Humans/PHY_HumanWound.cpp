// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Humans/PHY_HumanWound.cpp $
// $Author: Nld $
// $Modtime: 8/03/05 15:37 $
// $Revision: 2 $
// $Workfile: PHY_HumanWound.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_HumanWound.h"

#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include "xeumeuleu/xml.h"

using namespace xml;

PHY_HumanWound::T_HumanWoundMap PHY_HumanWound::humanWounds_;
MT_Random                       PHY_HumanWound::randomGenerator_;

uint     PHY_HumanWound::nDiagnosticTime_           = 0;
uint     PHY_HumanWound::nSortingTime_              = 0;
uint     PHY_HumanWound::nContaminatedHealingTime_  = 0;
uint     PHY_HumanWound::nContaminatedRestingTime_  = 0;
uint     PHY_HumanWound::nMentalDiseaseHealingTime_ = 0;
uint     PHY_HumanWound::nMentalDiseaseRestingTime_ = 0;
MT_Float PHY_HumanWound::rMentalDiseaseFactor_      = 0;

//
const PHY_HumanWound PHY_HumanWound::notWounded_( "NonBlesse", eNotWounded, EnumHumanWound::non_blesse             );
const PHY_HumanWound PHY_HumanWound::woundedU3_ ( "U3"       , eWoundedU3 , EnumHumanWound::blesse_urgence_3       );
const PHY_HumanWound PHY_HumanWound::woundedU2_ ( "U2"       , eWoundedU2 , EnumHumanWound::blesse_urgence_2       );
const PHY_HumanWound PHY_HumanWound::woundedU1_ ( "U1"       , eWoundedU1 , EnumHumanWound::blesse_urgence_1       );
const PHY_HumanWound PHY_HumanWound::woundedUE_ ( "UE"       , eWoundedUE , EnumHumanWound::blesse_urgence_extreme );
const PHY_HumanWound PHY_HumanWound::killed_    ( "Mort"     , eKilled    , EnumHumanWound::mort                   );

struct PHY_HumanWound::LoadingWrapper
{
    void ReadInjury( xml::xistream& xis, MT_Float& rFactorSum )
    {
        PHY_HumanWound::ReadInjury( xis, rFactorSum );
    }
};

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Initialize
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_HumanWound::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing human wounds" );
    humanWounds_[ notWounded_.GetName() ] = &notWounded_;
    humanWounds_[ killed_    .GetName() ] = &killed_;
    humanWounds_[ woundedU1_ .GetName() ] = &woundedU1_;
    humanWounds_[ woundedU2_ .GetName() ] = &woundedU2_;
    humanWounds_[ woundedU3_ .GetName() ] = &woundedU3_;
    humanWounds_[ woundedUE_ .GetName() ] = &woundedUE_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::InitializeMedicalData
// Created: NLD 2004-10-06
// Modified: JVT 2004-10-18
// -----------------------------------------------------------------------------
void PHY_HumanWound::InitializeMedicalData( xml::xistream& xis )
{
    MT_Float rTimeVal = 0.;
    xis >> start( "times" );
    tools::ReadTimeAttribute( xis, "diagnosis-time", rTimeVal );
    if( rTimeVal < 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "times: diagnosis-time < 0" );
    nDiagnosticTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rTimeVal );

    tools::ReadTimeAttribute( xis, "sorting-time", rTimeVal );
    if( rTimeVal < 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "times: sorting-time < 0" );
    nSortingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rTimeVal );
    xis >> end();

    MT_Float rFactorSum = 0.;
    LoadingWrapper loader;
    xis >> start( "injuries" )
            >> list( "injury", loader, &LoadingWrapper::ReadInjury, rFactorSum )
        >> end();

    if( std::fabs( 1. - rFactorSum ) > 0.01 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Total pourcentage is not 100%" ); // $$$$ ABL 2007-07-24: error context

    const_cast< PHY_HumanWound& >( notWounded_ ).nLifeExpectancy_ = std::numeric_limits< uint >::max();
    const_cast< PHY_HumanWound& >( killed_     ).nLifeExpectancy_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::ReadInjury
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void PHY_HumanWound::ReadInjury( xml::xistream& xis, MT_Float& rFactorSum )
{
    std::string injuryType;
    xis >> attribute( "category", injuryType );

    CIT_HumanWoundMap it = humanWounds_.find( injuryType );
    if( it != humanWounds_.end() )
    {
        const PHY_HumanWound& wound = *it->second;

        if( wound == killed_ || wound == notWounded_ )
            return;

        MT_Float rValue = 0.;
        xis >> attribute( "percentage", rValue );
        if( rValue < 0 || rValue > 100 )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "injury: percentage not in [0..100]" );

        rValue /= 100.;
        rFactorSum += rValue;
        const_cast< PHY_HumanWound& >( wound ).rWoundedFactor_ = rValue;


        if( tools::ReadTimeAttribute( xis, "life-expectancy", rValue ) )
        {
            if( rValue <= 0 )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "injury: life-exectancy <= 0" );
            const_cast< PHY_HumanWound& >( wound ).nLifeExpectancy_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );
        }

        tools::ReadTimeAttribute( xis, "caring-time", rValue );
        if( rValue < 0 )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "injury: caring-time < 0" );
        const_cast< PHY_HumanWound& >( wound ).nHealingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );

        tools::ReadTimeAttribute( xis, "resting-time", rValue );
        if( rValue < 0 )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "injury: resting-time < 0" );
        const_cast< PHY_HumanWound& >( wound ).nRestingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );
    }
    else if( injuryType == "mental" )
    {
        MT_Float rValue = 0.;
        tools::ReadTimeAttribute( xis, "caring-time", rValue );
        if( rValue < 0 )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "injury: caring-time < 0" );
        nMentalDiseaseHealingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );

        tools::ReadTimeAttribute( xis, "resting-time", rValue );
        if( rValue < 0 )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "injury: resting-time < 0" );
        nMentalDiseaseRestingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );

        tools::ReadTimeAttribute( xis, "percentage", rValue );
        if( rValue < 0 || rValue > 100 )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "injury: percentage not in [0..100]" );
        rMentalDiseaseFactor_      = rValue / 100.;
    }
    else if( injuryType == "contaminated" )
    {
        MT_Float rValue = 0;
        tools::ReadTimeAttribute( xis, "caring-time", rValue );
        if( rValue < 0 )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "injury: caring-time < 0" );
        nContaminatedHealingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );

        tools::ReadTimeAttribute( xis, "resting-time", rValue );
        if( rValue < 0 )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "injury: resting-time < 0" );
        nContaminatedRestingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );
    }
    else
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "injury: unknow category" );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Terminate
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_HumanWound::Terminate()
{
    // NOTHING
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_HumanWound::PHY_HumanWound( const std::string& strName, E_Wound nWound, const ASN1T_EnumHumanWound& nAsnID )
    : strName_        ( strName )
    , nWound_         ( nWound  )
    , nAsnID_         ( nAsnID  )
    , rWoundedFactor_ ( 0. )
    , nLifeExpectancy_( 0 )
    , nHealingTime_   ( 0 )
    , nRestingTime_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound destructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_HumanWound::~PHY_HumanWound()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::ChooseRandomWound
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
const PHY_HumanWound& PHY_HumanWound::ChooseRandomWound()
{
    const MT_Float rRand = randomGenerator_.rand_oi();

    MT_Float rSumFactors = 0.;
    for( CIT_HumanWoundMap itWound = humanWounds_.begin(); itWound != humanWounds_.end(); ++itWound )
    {
        const PHY_HumanWound& state = *itWound->second;
        rSumFactors += state.GetWoundedFactor();
        if( rSumFactors >= rRand )
            return state;
    }
    assert( false );
    return killed_;
}
