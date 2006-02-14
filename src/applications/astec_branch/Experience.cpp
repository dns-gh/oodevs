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
#include "xeumeuleu/xml.h"
using namespace xml;

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
void Experience::Initialize( xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing experiences" );
    experiences_[ conscrit_    .GetName() ] = &conscrit_;
    experiences_[ experimente_ .GetName() ] = &experimente_;
    experiences_[ veteran_     .GetName() ] = &veteran_;

    xis >> start( "FacteursHumains" )
            >>start( "Experience" );

    for( CIT_ExperienceMap it = experiences_.begin(); it != experiences_.end(); ++it )
    {
        Experience& experience = const_cast< Experience& >( *it->second );
        xis >> start( experience.GetName() );
            experience.Read( xis );
        xis >> end();
    }
    xis >> end()
        >> end();
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
void Experience::Read( xistream& xis )
{
    xis >> content( "CoefModificationVitesseMax"        , rCoefMaxSpeedModificator_       )
        >> content( "CoefModificationTempsRechargement" , rCoefReloadingTimeModificator_  )
        >> content( "CoefModificationPH"                , rCoefPhModificator_             )
        >> content( "CoefModificationTempsMiseEnPosture", rCoefPostureTimeModificator_    )
        >> content( "CoefModificationDistanceCapteurs"  , rCoefSensorDistanceModificator_ );
}
