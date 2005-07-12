// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/HumanFactors/PHY_Experience.cpp $
// $Author: Nld $
// $Modtime: 1/12/04 10:46 $
// $Revision: 2 $
// $Workfile: PHY_Experience.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_Experience.h"

PHY_Experience::T_ExperienceMap PHY_Experience::experiences_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

const PHY_Experience PHY_Experience::conscrit_   ( "Conscrit"   , eConscrit   , EnumUnitExperience::conscrit   , 1.);
const PHY_Experience PHY_Experience::experimente_( "Experimente", eExperimente, EnumUnitExperience::experimente, 1.);
const PHY_Experience PHY_Experience::veteran_    ( "Veteran"    , eVeteran    , EnumUnitExperience::veteran    , 1. );

// -----------------------------------------------------------------------------
// Name: PHY_Experience::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Experience::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing experiences" );
    experiences_[ conscrit_    .GetName() ] = &conscrit_;
    experiences_[ experimente_ .GetName() ] = &experimente_;
    experiences_[ veteran_     .GetName() ] = &veteran_;

    archive.Section( "FacteursHumains" );
    archive.Section( "Experience" );

    for( CIT_ExperienceMap it = experiences_.begin(); it != experiences_.end(); ++it )
    {
        PHY_Experience& experience = const_cast< PHY_Experience& >( *it->second );
        archive.Section( experience.GetName() );
        experience.Read( archive );
        archive.EndSection(); // experience.GetName()
    }

    archive.EndSection(); // Experience
    archive.EndSection(); // FacteursHumains
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Experience::Terminate()
{
    experiences_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Experience constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Experience::PHY_Experience( const std::string& strName, E_ExperienceType nType, ASN1T_EnumUnitExperience nAsnID, MT_Float rDIAWeight )
    : strName_                       ( strName )
    , nType_                         ( nType   )
    , nAsnID_                        ( nAsnID  )
    , rDIAWeight_                    ( rDIAWeight )
    , rCoefMaxSpeedModificator_      ( 1. )
    , rCoefReloadingTimeModificator_ ( 1. )
    , rCoefPhModificator_            ( 1. )
    , rCoefPostureTimeModificator_   ( 1. )
    , rCoefSensorDistanceModificator_( 1. )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Experience::~PHY_Experience()
{
}


// -----------------------------------------------------------------------------
// Name: PHY_Experience::Read
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void PHY_Experience::Read( MIL_InputArchive& archive )
{
    archive.ReadField( "CoefModificationVitesseMax"        , rCoefMaxSpeedModificator_      , CheckValueGreater( 0. ) );
    archive.ReadField( "CoefModificationTempsRechargement" , rCoefReloadingTimeModificator_ , CheckValueGreater( 0. ) );
    archive.ReadField( "CoefModificationPH"                , rCoefPhModificator_            , CheckValueGreater( 0. ) );
    archive.ReadField( "CoefModificationTempsMiseEnPosture", rCoefPostureTimeModificator_   , CheckValueGreater( 0. ) );
    archive.ReadField( "CoefModificationDistanceCapteurs"  , rCoefSensorDistanceModificator_, CheckValueGreater( 0. ) );
}
