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
#include "MIL_Random.h"
#include "protocol/Protocol.h"
#include "Tools/MIL_Tools.h"
#include "tools/Codec.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

PHY_HumanWound::T_HumanWoundMap PHY_HumanWound::humanWounds_;

unsigned int PHY_HumanWound::nDiagnosticTime_ = 0;
unsigned int PHY_HumanWound::nSortingTime_ = 0;
double       PHY_HumanWound::diagnosisLifeExpectancyFactor_ = 2.0;
unsigned int PHY_HumanWound::nContaminatedHealingTime_ = 0;
unsigned int PHY_HumanWound::nContaminatedRestingTime_ = 0;
unsigned int PHY_HumanWound::nMentalDiseaseHealingTime_ = 0;
unsigned int PHY_HumanWound::nMentalDiseaseRestingTime_ = 0;
double PHY_HumanWound::rMentalDiseaseFactor_ = 0;

const PHY_HumanWound PHY_HumanWound::notWounded_( "healthy", eNotWounded, sword::unwounded             );
const PHY_HumanWound PHY_HumanWound::woundedU3_ ( "u3"     , eWoundedU3 , sword::wounded_urgency_3       );
const PHY_HumanWound PHY_HumanWound::woundedU2_ ( "u2"     , eWoundedU2 , sword::wounded_urgency_2       );
const PHY_HumanWound PHY_HumanWound::woundedU1_ ( "u1"     , eWoundedU1 , sword::wounded_urgency_1       );
const PHY_HumanWound PHY_HumanWound::woundedUE_ ( "ue"     , eWoundedUE , sword::wounded_extreme_urgency );
const PHY_HumanWound PHY_HumanWound::killed_    ( "dead"   , eKilled    , sword::dead                   );

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
    double rTimeVal = 0.;
    xis >> xml::start( "times" );
    tools::ReadTimeAttribute( xis, "diagnosis-time", rTimeVal );
    if( rTimeVal < 0 )
        throw MASA_EXCEPTION( xis.context() + "times: diagnosis-time < 0" );
    nDiagnosticTime_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( rTimeVal ) );

    tools::ReadTimeAttribute( xis, "sorting-time", rTimeVal );
    if( rTimeVal < 0 )
        throw MASA_EXCEPTION( xis.context() + "times: sorting-time < 0" );
    nSortingTime_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( rTimeVal ) );

    xis >> xml::optional >> xml::attribute( "diagnosis-life-expectancy-factor", diagnosisLifeExpectancyFactor_ );
    if ( diagnosisLifeExpectancyFactor_ < 0.0 )
        throw MASA_EXCEPTION( xis.context() + "times: diagnosis-life-expectancy-factor < 0" );

    xis >> xml::end;

    double rFactorSum = 0.;
    xis >> xml::start( "injuries" )
            >> xml::list( "injury", boost::bind( &PHY_HumanWound::ReadInjury, _1, boost::ref( rFactorSum ) ) )
        >> xml::end;

    if( std::fabs( 1. - rFactorSum ) > 0.01 )
        throw MASA_EXCEPTION( xis.context() + "Total pourcentage is not 100%" );

    const_cast< PHY_HumanWound& >( notWounded_ ).nLifeExpectancy_ = std::numeric_limits< unsigned int >::max();
    const_cast< PHY_HumanWound& >( killed_ ).nLifeExpectancy_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::ReadInjury
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void PHY_HumanWound::ReadInjury( xml::xistream& xis, double& rFactorSum )
{
    std::string injuryType = xis.attribute< std::string >( "category" );
    auto it = humanWounds_.find( injuryType );
    if( it != humanWounds_.end() )
    {
        const PHY_HumanWound& wound = *it->second;
        if( wound == killed_ || wound == notWounded_ )
            return;
        double rValue = xis.attribute< double >( "percentage" );
        if( rValue < 0 || rValue > 100 )
            throw MASA_EXCEPTION( xis.context() + "injury: percentage not in [0..100]" );
        rValue /= 100.;
        rFactorSum += rValue;
        const_cast< PHY_HumanWound& >( wound ).rWoundedFactor_ = rValue;
        if( tools::ReadTimeAttribute( xis, "life-expectancy", rValue ) )
        {
            if( rValue <= 0 )
                throw MASA_EXCEPTION( xis.context() + "injury: life-expectancy <= 0" );
            const_cast< PHY_HumanWound& >( wound ).nLifeExpectancy_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( rValue ) );
        }
        tools::ReadTimeAttribute( xis, "caring-time", rValue );
        if( rValue < 0 )
            throw MASA_EXCEPTION( xis.context() + "injury: caring-time < 0" );
        const_cast< PHY_HumanWound& >( wound ).nHealingTime_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( rValue ) );
        tools::ReadTimeAttribute( xis, "resting-time", rValue );
        if( rValue < 0 )
            throw MASA_EXCEPTION( xis.context() + "injury: resting-time < 0" );
        const_cast< PHY_HumanWound& >( wound ).nRestingTime_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( rValue ) );
    }
    else if( injuryType == "mental" )
    {
        double rValue = 0.;
        tools::ReadTimeAttribute( xis, "caring-time", rValue );
        if( rValue < 0 )
            throw MASA_EXCEPTION( xis.context() + "injury: caring-time < 0" );
        nMentalDiseaseHealingTime_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( rValue ) );
        tools::ReadTimeAttribute( xis, "resting-time", rValue );
        if( rValue < 0 )
            throw MASA_EXCEPTION( xis.context() + "injury: resting-time < 0" );
        nMentalDiseaseRestingTime_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( rValue ) );
        rValue = xis.attribute< double >( "percentage" );
        if( rValue < 0 || rValue > 100 )
            throw MASA_EXCEPTION( xis.context() + "injury: percentage not in [0..100]" );
        rMentalDiseaseFactor_ = rValue / 100.;
    }
    else if( injuryType == "contaminated" )
    {
        double rValue = 0;
        tools::ReadTimeAttribute( xis, "caring-time", rValue );
        if( rValue < 0 )
            throw MASA_EXCEPTION( xis.context() + "injury: caring-time < 0" );
        nContaminatedHealingTime_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( rValue ) );
        tools::ReadTimeAttribute( xis, "resting-time", rValue );
        if( rValue < 0 )
            throw MASA_EXCEPTION( xis.context() + "injury: resting-time < 0" );
        nContaminatedRestingTime_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( rValue ) );
    }
    else
        throw MASA_EXCEPTION( xis.context() + "injury: unknown category" );
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
PHY_HumanWound::PHY_HumanWound( const std::string& strName, E_Wound nWound, const sword::EnumHumanWound& nAsnID )
    : strName_        ( strName )
    , nWound_         ( nWound )
    , nAsnID_         ( nAsnID )
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
    const double rRand = MIL_Random::rand_oi( MIL_Random::eWounds );
    double rSumFactors = 0;
    for( auto it = humanWounds_.begin(); it != humanWounds_.end(); ++it )
    {
        const PHY_HumanWound& state = *it->second;
        rSumFactors += state.GetWoundedFactor();
        if( rSumFactors >= rRand )
            return state;
    }
    assert( false ); // $$$$ MCO : throw instead, or even better rewrite algorithm
    return killed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetRandomWoundSeriousness
// Created: ABR 2011-08-11
// -----------------------------------------------------------------------------
const PHY_HumanWound* PHY_HumanWound::GetRandomWoundSeriousness()
{
    unsigned long index = MIL_Random::rand32_io( 0, 4 ); // $$$$ ABR 2011-08-11: Hard coded, number of injury seriousness
    switch( index )
    {
    case 0:  return &woundedU1_;
    case 1:  return &woundedU2_;
    case 2:  return &woundedU3_;
    default: return &woundedUE_;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetDiagnosingTime
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
unsigned int PHY_HumanWound::GetDiagnosticTime()
{
    return nDiagnosticTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetSortingTime
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
unsigned int PHY_HumanWound::GetSortingTime()
{
    return nSortingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetDiagnosisLifeExpectancyFactor
// Created: AHC 2011-01-26
// -----------------------------------------------------------------------------
double PHY_HumanWound::GetDiagnosisLifeExpectancyFactor()
{
    return diagnosisLifeExpectancyFactor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetContaminatedHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
unsigned int PHY_HumanWound::GetContaminatedHealingTime()
{
    return nContaminatedHealingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetContaminatedRestingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
unsigned int PHY_HumanWound::GetContaminatedRestingTime()
{
    return nContaminatedRestingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetMentalDiseasedHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
unsigned int PHY_HumanWound::GetMentalDiseaseHealingTime()
{
    return nMentalDiseaseHealingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetMentalDiseasedRestingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
unsigned int PHY_HumanWound::GetMentalDiseaseRestingTime()
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
    auto it = humanWounds_.find( strName );
    return it == humanWounds_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_HumanWound* PHY_HumanWound::Find( sword::EnumHumanWound nAsnID )
{
    for( auto it = humanWounds_.begin(); it != humanWounds_.end(); ++it )
        if( it->second->GetAsnID() == nAsnID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_HumanWound* PHY_HumanWound::Find( unsigned int nID )
{
    for( auto it = humanWounds_.begin(); it != humanWounds_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
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
unsigned int PHY_HumanWound::GetID() const
{
    return static_cast< unsigned int >( nWound_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
sword::EnumHumanWound PHY_HumanWound::GetAsnID() const
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
double PHY_HumanWound::GetWoundedFactor() const
{
    return rWoundedFactor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetLifeExpectancy
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
unsigned int PHY_HumanWound::GetLifeExpectancy() const
{
    return nLifeExpectancy_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
unsigned int PHY_HumanWound::GetHealingTime() const
{
    return nHealingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetHealingTime
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
unsigned int PHY_HumanWound::GetRestingTime() const
{
    return nRestingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::ChooseMentalDisease
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
bool PHY_HumanWound::ChooseMentalDisease()
{
    return MIL_Random::rand_oi( 0., 1. ) <= rMentalDiseaseFactor_;
}
