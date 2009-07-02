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
#include <xeumeuleu/xml.h>

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
    xis >> xml::start( "times" );
    tools::ReadTimeAttribute( xis, "diagnosis-time", rTimeVal );
    if( rTimeVal < 0 )
        xis.error( "times: diagnosis-time < 0" );
    nDiagnosticTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rTimeVal );

    tools::ReadTimeAttribute( xis, "sorting-time", rTimeVal );
    if( rTimeVal < 0 )
        xis.error( "times: sorting-time < 0" );
    nSortingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rTimeVal );
    xis >> xml::end();

    MT_Float rFactorSum = 0.;
    LoadingWrapper loader;
    xis >> xml::start( "injuries" )
            >> xml::list( "injury", loader, &LoadingWrapper::ReadInjury, rFactorSum )
        >> xml::end();

    if( std::fabs( 1. - rFactorSum ) > 0.01 )
        xis.error( "Total pourcentage is not 100%" );

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
    xis >> xml::attribute( "category", injuryType );

    CIT_HumanWoundMap it = humanWounds_.find( injuryType );
    if( it != humanWounds_.end() )
    {
        const PHY_HumanWound& wound = *it->second;

        if( wound == killed_ || wound == notWounded_ )
            return;

        MT_Float rValue = 0.;
        xis >> xml::attribute( "percentage", rValue );
        if( rValue < 0 || rValue > 100 )
            xis.error( "injury: percentage not in [0..100]" );

        rValue /= 100.;
        rFactorSum += rValue;
        const_cast< PHY_HumanWound& >( wound ).rWoundedFactor_ = rValue;


        if( tools::ReadTimeAttribute( xis, "life-expectancy", rValue ) )
        {
            if( rValue <= 0 )
                xis.error( "injury: life-exectancy <= 0" );
            const_cast< PHY_HumanWound& >( wound ).nLifeExpectancy_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );
        }

        tools::ReadTimeAttribute( xis, "caring-time", rValue );
        if( rValue < 0 )
            xis.error( "injury: caring-time < 0" );
        const_cast< PHY_HumanWound& >( wound ).nHealingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );

        tools::ReadTimeAttribute( xis, "resting-time", rValue );
        if( rValue < 0 )
            xis.error( "injury: resting-time < 0" );
        const_cast< PHY_HumanWound& >( wound ).nRestingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );
    }
    else if( injuryType == "mental" )
    {
        MT_Float rValue = 0.;
        tools::ReadTimeAttribute( xis, "caring-time", rValue );
        if( rValue < 0 )
            xis.error( "injury: caring-time < 0" );
        nMentalDiseaseHealingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );

        tools::ReadTimeAttribute( xis, "resting-time", rValue );
        if( rValue < 0 )
            xis.error( "injury: resting-time < 0" );
        nMentalDiseaseRestingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );

        tools::ReadTimeAttribute( xis, "percentage", rValue );
        if( rValue < 0 || rValue > 100 )
            xis.error( "injury: percentage not in [0..100]" );
        rMentalDiseaseFactor_      = rValue / 100.;
    }
    else if( injuryType == "contaminated" )
    {
        MT_Float rValue = 0;
        tools::ReadTimeAttribute( xis, "caring-time", rValue );
        if( rValue < 0 )
            xis.error( "injury: caring-time < 0" );
        nContaminatedHealingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );

        tools::ReadTimeAttribute( xis, "resting-time", rValue );
        if( rValue < 0 )
            xis.error( "injury: resting-time < 0" );
        nContaminatedRestingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );
    }
    else
        xis.error( "injury: unknow category" );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Terminate
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_HumanWound::Terminate()
{
    // NOTHING
}

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

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetDiagnosingTime
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
uint PHY_HumanWound::GetDiagnosticTime()
{
    return nDiagnosticTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetSortingTime
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
uint PHY_HumanWound::GetSortingTime()
{
    return nSortingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetContaminatedHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
uint PHY_HumanWound::GetContaminatedHealingTime()
{
    return nContaminatedHealingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetContaminatedRestingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
uint PHY_HumanWound::GetContaminatedRestingTime()
{
    return nContaminatedRestingTime_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetMentalDiseasedHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
uint PHY_HumanWound::GetMentalDiseaseHealingTime()
{
    return nMentalDiseaseHealingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetMentalDiseasedRestingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
uint PHY_HumanWound::GetMentalDiseaseRestingTime()
{
    return nMentalDiseaseRestingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetHumanWounds
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
const PHY_HumanWound::T_HumanWoundMap& PHY_HumanWound::GetHumanWounds()
{
    return humanWounds_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Find
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
const PHY_HumanWound* PHY_HumanWound::Find( const std::string& strName )
{
    CIT_HumanWoundMap it = humanWounds_.find( strName );
    return it == humanWounds_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_HumanWound* PHY_HumanWound::Find( ASN1T_EnumHumanWound nAsnID )
{
    CIT_HumanWoundMap it = std::find_if( humanWounds_.begin(), humanWounds_.end(), std::compose1( std::bind2nd( std::equal_to< ASN1T_EnumHumanWound >(), nAsnID ), std::compose1( std::mem_fun( &PHY_HumanWound::GetAsnID ), std::select2nd< T_HumanWoundMap::value_type >() ) ) );

    return it == humanWounds_.end() ? 0 : it->second;   
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_HumanWound* PHY_HumanWound::Find( uint nID )
{
    CIT_HumanWoundMap it = std::find_if( humanWounds_.begin(), humanWounds_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_HumanWound::GetID ), std::select2nd< T_HumanWoundMap::value_type >() ) ) );

    return it == humanWounds_.end() ? 0 : it->second;    
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetName
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
const std::string& PHY_HumanWound::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetID
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
uint PHY_HumanWound::GetID() const
{
    return (uint)nWound_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
ASN1T_EnumHumanWound PHY_HumanWound::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::operator==
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PHY_HumanWound::operator==( const PHY_HumanWound& rhs ) const
{
    return nWound_ == rhs.nWound_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::operator!=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PHY_HumanWound::operator!=( const PHY_HumanWound& rhs ) const
{
    return nWound_ != rhs.nWound_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::operator!=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PHY_HumanWound::operator<( const PHY_HumanWound& rhs ) const
{
    return nWound_ < rhs.nWound_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::operator!=
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
bool PHY_HumanWound::operator>( const PHY_HumanWound& rhs ) const
{
    return nWound_ > rhs.nWound_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetWoundedFactor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MT_Float PHY_HumanWound::GetWoundedFactor() const
{
    return rWoundedFactor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetLifeExpectancy
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
uint PHY_HumanWound::GetLifeExpectancy() const
{
    return nLifeExpectancy_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
uint PHY_HumanWound::GetHealingTime() const
{
    return nHealingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
uint PHY_HumanWound::GetRestingTime() const
{
    return nRestingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::ChooseMentalDisease
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
bool PHY_HumanWound::ChooseMentalDisease()
{
    return randomGenerator_.rand_oi( 0., 1. ) <= rMentalDiseaseFactor_;
}
