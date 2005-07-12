// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Experience.cpp $
// $Author: Nld $
// $Modtime: 30/11/04 18:32 $
// $Revision: 1 $
// $Workfile: MOS_Experience.cpp $
//
// *****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_Experience.h"

MOS_Experience::T_ExperienceMap MOS_Experience::experiences_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

const MOS_Experience MOS_Experience::conscrit_   ( "Conscrit"   , eConscrit   , EnumUnitExperience::conscrit    );
const MOS_Experience MOS_Experience::experimente_( "Experimente", eExperimente, EnumUnitExperience::experimente );
const MOS_Experience MOS_Experience::veteran_    ( "Veteran"    , eVeteran    , EnumUnitExperience::veteran     );

// -----------------------------------------------------------------------------
// Name: MOS_Experience::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MOS_Experience::Initialize( MT_InputArchive_ABC& archive )
{
    MT_LOG_INFO_MSG( "Initializing experiences" );
    experiences_[ conscrit_    .GetName() ] = &conscrit_;
    experiences_[ experimente_ .GetName() ] = &experimente_;
    experiences_[ veteran_     .GetName() ] = &veteran_;

    archive.Section( "FacteursHumains" );
    archive.Section( "Experience" );

    for( CIT_ExperienceMap it = experiences_.begin(); it != experiences_.end(); ++it )
    {
        MOS_Experience& experience = const_cast< MOS_Experience& >( *it->second );
        archive.Section( experience.GetName() );
        experience.Read( archive );
        archive.EndSection(); // experience.GetName()
    }

    archive.EndSection(); // Experience
    archive.EndSection(); // FacteursHumains
}

// -----------------------------------------------------------------------------
// Name: MOS_Experience::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MOS_Experience::Terminate()
{
    experiences_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_Experience constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MOS_Experience::MOS_Experience( const std::string& strName, E_ExperienceType nType, ASN1T_EnumUnitExperience nAsnID )
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
// Name: MOS_Experience destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MOS_Experience::~MOS_Experience()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_Experience::Read
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void MOS_Experience::Read( MT_InputArchive_ABC& archive )
{
    archive.ReadField( "CoefModificationVitesseMax"        , rCoefMaxSpeedModificator_       );
    archive.ReadField( "CoefModificationTempsRechargement" , rCoefReloadingTimeModificator_  );
    archive.ReadField( "CoefModificationPH"                , rCoefPhModificator_             );
    archive.ReadField( "CoefModificationTempsMiseEnPosture", rCoefPostureTimeModificator_    );
    archive.ReadField( "CoefModificationDistanceCapteurs"  , rCoefSensorDistanceModificator_ );
}
