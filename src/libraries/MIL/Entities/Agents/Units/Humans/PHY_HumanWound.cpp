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

#include "MIL_pch.h"

#include "PHY_HumanWound.h"

#include "Tools/MIL_Tools.h"

PHY_HumanWound::T_HumanWoundMap PHY_HumanWound::humanWounds_;
MT_Random                       PHY_HumanWound::randomGenerator_;

      uint           PHY_HumanWound::nDiagnosticTime_           = 0;
      uint           PHY_HumanWound::nSortingTime_              = 0;      
      uint           PHY_HumanWound::nContaminatedHealingTime_  = 0;
      uint           PHY_HumanWound::nMentalDiseaseHealingTime_ = 0;
      MT_Float       PHY_HumanWound::rMentalDiseaseFactor_      = 0;

const PHY_HumanWound PHY_HumanWound::notWounded_( "NonBlesse", eNotWounded, 0           , &woundedU3_, EnumHumanWound::non_blesse             );
const PHY_HumanWound PHY_HumanWound::woundedU3_ ( "U3"       , eWoundedU3 , &notWounded_, &woundedU2_, EnumHumanWound::blesse_urgence_3       );
const PHY_HumanWound PHY_HumanWound::woundedU2_ ( "U2"       , eWoundedU2 , &woundedU3_ , &woundedU1_, EnumHumanWound::blesse_urgence_2       );
const PHY_HumanWound PHY_HumanWound::woundedU1_ ( "U1"       , eWoundedU1 , &woundedU2_ , &woundedUE_, EnumHumanWound::blesse_urgence_1       );
const PHY_HumanWound PHY_HumanWound::woundedUE_ ( "UE"       , eWoundedUE , &woundedU1_ , &killed_   , EnumHumanWound::blesse_urgence_extreme );
const PHY_HumanWound PHY_HumanWound::killed_    ( "Mort"     , eKilled    , &woundedUE_ , 0          , EnumHumanWound::mort                   );

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
void PHY_HumanWound::InitializeMedicalData( MIL_InputArchive& archive )
{
    MT_Float rTimeVal = 0.;
    archive.ReadTimeField( "TempsDiagnostique", rTimeVal, CheckValueGreaterOrEqual( 0. ) );
    nDiagnosticTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rTimeVal );

    archive.ReadTimeField( "TempsTri", rTimeVal, CheckValueGreaterOrEqual( 0. ) );
    nSortingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rTimeVal );

    archive.Section( "CategoriesBlesse" );

    MT_Float rFactorSum = 0.;
    for( CIT_HumanWoundMap itWound = humanWounds_.begin(); itWound != humanWounds_.end(); ++itWound )
    {
        const PHY_HumanWound& wound = *itWound->second;

        if( wound == killed_ || wound == notWounded_ )
            continue;
        
        archive.Section( wound.GetName() );

        MT_Float rValue = 0.;
        archive.ReadAttribute( "pourcentage", rValue, CheckValueBound( 0., 100. ) );        
        rValue /= 100.;
        rFactorSum += rValue;
        const_cast< PHY_HumanWound& >( wound ).rWoundedFactor_ = rValue;

        if( archive.ReadTimeAttribute( "esperanceVie", rValue, CheckValueGreater( 0. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) )
            const_cast< PHY_HumanWound& >( wound ).nLifeExpectancy_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );

        archive.ReadTimeAttribute( "tempsSoin", rValue, CheckValueGreater( 0. ) );
        const_cast< PHY_HumanWound& >( wound ).nHealingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );
    
        archive.EndSection(); // 
    }
    if( rFactorSum != 1. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Total pourcentage is not 100%", archive.GetContext() );

    archive.EndSection(); // CategoriesBlesse
    
    MT_Float rValue = 0.;
    archive.Section( "ReacMental" );
    archive.ReadTimeAttribute( "tempsSoin", rValue, CheckValueGreater( 0. ) );
    nMentalDiseaseHealingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );
    archive.ReadAttribute( "pourcentage", rValue, CheckValueBound( 0., 100. ) );
    rMentalDiseaseFactor_      = rValue / 100.;
    archive.EndSection(); // ReacMental

    archive.Section( "Contamines" );
    archive.ReadTimeAttribute( "tempsSoin", rValue, CheckValueGreater( 0. ) );
    nContaminatedHealingTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rValue );
    archive.EndSection(); // Contamines

    const_cast< PHY_HumanWound& >( notWounded_ ).nLifeExpectancy_ = std::numeric_limits< uint >::max();
    const_cast< PHY_HumanWound& >( killed_     ).nLifeExpectancy_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Terminate
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_HumanWound::Terminate()
{
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_HumanWound::PHY_HumanWound( const std::string& strName, E_Wound nWound, const PHY_HumanWound* pPrevious, const PHY_HumanWound* pNext, const ASN1T_EnumHumanWound& nAsnID )
    : strName_        ( strName )
    , nWound_         ( nWound  )
    , nAsnID_         ( nAsnID  )
    , pPrevious_      ( pPrevious )
    , pNext_          ( pNext )
    , rWoundedFactor_ ( 0. )
    , nLifeExpectancy_( 0 )
    , nHealingTime_   ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound destructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_HumanWound::~PHY_HumanWound()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::Degrade
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
const PHY_HumanWound& PHY_HumanWound::Degrade( const PHY_HumanWound& degradation ) const
{
    if( !pNext_ || !degradation.pPrevious_ )
        return *this;
    return pNext_->Degrade( *degradation.pPrevious_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanWound::GetRandomWound
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
const PHY_HumanWound& PHY_HumanWound::GetRandomWound() 
{
    const MT_Float rRand = randomGenerator_.rand_oi( 0., 1. );

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
