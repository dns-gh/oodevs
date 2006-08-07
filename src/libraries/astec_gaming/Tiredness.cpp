// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Tiredness.cpp $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: Tiredness.cpp $
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "Tiredness.h"
#include "xeumeuleu/xml.h"
using namespace xml;

Tiredness::T_TirednessMap Tiredness::tirednesses_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

const Tiredness Tiredness::normal_ ( "Normal" , eNormal , EnumUnitFatigue::normal  );
const Tiredness Tiredness::fatigue_( "Fatigue", eFatigue, EnumUnitFatigue::fatigue );
const Tiredness Tiredness::epuise_ ( "Epuise" , eEpuise , EnumUnitFatigue::epuise  );

// -----------------------------------------------------------------------------
// Name: Tiredness::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void Tiredness::Initialize( xistream& xis  )
{
    tirednesses_[ normal_  .GetName() ] = &normal_;
    tirednesses_[ fatigue_ .GetName() ] = &fatigue_;
    tirednesses_[ epuise_  .GetName() ] = &epuise_;

    xis >> start( "FacteursHumains" )
         >> start( "Fatigue" );

    for( CIT_TirednessMap it = tirednesses_.begin(); it != tirednesses_.end(); ++it )
    {
        Tiredness& tiredness = const_cast< Tiredness& >( *it->second );
        xis >> start( tiredness.GetName() );
            tiredness.Read( xis );
        xis >> end();
    }

    xis >> end()
        >> end();
}

// -----------------------------------------------------------------------------
// Name: Tiredness::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void Tiredness::Terminate()
{
    tirednesses_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Tiredness constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
Tiredness::Tiredness( const std::string& strName, E_TirednessType nType, ASN1T_EnumUnitFatigue nAsnID )
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
// Name: Tiredness destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
Tiredness::~Tiredness()
{
}

// -----------------------------------------------------------------------------
// Name: Tiredness::Read
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void Tiredness::Read( xistream& xis  )
{
    xis >> content( "CoefModificationVitesseMax"        , rCoefMaxSpeedModificator_      );
    xis >> content( "CoefModificationTempsRechargement" , rCoefReloadingTimeModificator_ );
    xis >> content( "CoefModificationPH"                , rCoefPhModificator_            );
    xis >> content( "CoefModificationTempsMiseEnPosture", rCoefPostureTimeModificator_   );
    xis >> content( "CoefModificationDistanceCapteurs"  , rCoefSensorDistanceModificator_);
}
