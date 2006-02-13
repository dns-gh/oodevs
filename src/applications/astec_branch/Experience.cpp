// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Experience.cpp $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: Experience.cpp $
//
// *****************************************************************************

#include "astec_pch.h"

#include "Experience.h"

Experience::T_ExperienceMap Experience::experiences_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

const Experience Experience::conscrit_   ( "Conscrit"   , eConscrit   , EnumUnitExperience::conscrit    );
const Experience Experience::experimente_( "Experimente", eExperimente, EnumUnitExperience::experimente );
const Experience Experience::veteran_    ( "Veteran"    , eVeteran    , EnumUnitExperience::veteran     );

// -----------------------------------------------------------------------------
// Name: Experience::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void Experience::Initialize( InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing experiences" );
    experiences_[ conscrit_    .GetName() ] = &conscrit_;
    experiences_[ experimente_ .GetName() ] = &experimente_;
    experiences_[ veteran_     .GetName() ] = &veteran_;

    archive.Section( "FacteursHumains" );
    archive.Section( "Experience" );

    for( CIT_ExperienceMap it = experiences_.begin(); it != experiences_.end(); ++it )
    {
        Experience& experience = const_cast< Experience& >( *it->second );
        archive.Section( experience.GetName() );
        experience.Read( archive );
        archive.EndSection(); // experience.GetName()
    }

    archive.EndSection(); // Experience
    archive.EndSection(); // FacteursHumains
}

// -----------------------------------------------------------------------------
// Name: Experience::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void Experience::Terminate()
{
    experiences_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Experience constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
Experience::Experience( const std::string& strName, E_ExperienceType nType, ASN1T_EnumUnitExperience nAsnID )
    : strName_                       ( strName )
    , nType_                         ( nType   )
    , nAsnID_                        ( nAsnID  )
    , rCoefMaxSpeedModificator_      ( 1. )
    , rCoefReloadingTimeModificator_ ( 1. )
    , rCoefPhModificator_            ( 1. )
    , rCoefPostureTimeModificator_   ( 1. )
    , rCoefSensorDistanceModificator_( 1. )
{
}

// -----------------------------------------------------------------------------
// Name: Experience destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
Experience::~Experience()
{
}


// -----------------------------------------------------------------------------
// Name: Experience::Read
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void Experience::Read( InputArchive& archive )
{
    archive.ReadField( "CoefModificationVitesseMax"        , rCoefMaxSpeedModificator_       );
    archive.ReadField( "CoefModificationTempsRechargement" , rCoefReloadingTimeModificator_  );
    archive.ReadField( "CoefModificationPH"                , rCoefPhModificator_             );
    archive.ReadField( "CoefModificationTempsMiseEnPosture", rCoefPostureTimeModificator_    );
    archive.ReadField( "CoefModificationDistanceCapteurs"  , rCoefSensorDistanceModificator_ );
}
