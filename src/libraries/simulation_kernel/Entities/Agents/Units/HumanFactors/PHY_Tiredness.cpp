// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/HumanFactors/PHY_Tiredness.cpp $
// $Author: Nld $
// $Modtime: 1/12/04 10:48 $
// $Revision: 2 $
// $Workfile: PHY_Tiredness.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_Tiredness.h"

PHY_Tiredness::T_TirednessMap PHY_Tiredness::tirednesses_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

const PHY_Tiredness PHY_Tiredness::normal_ ( "Normal" , eNormal , EnumUnitFatigue::normal , 1. );
const PHY_Tiredness PHY_Tiredness::fatigue_( "Fatigue", eFatigue, EnumUnitFatigue::fatigue, 1. );
const PHY_Tiredness PHY_Tiredness::epuise_ ( "Epuise" , eEpuise , EnumUnitFatigue::epuise , 1. );

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Tiredness::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing tirenesses" );
    tirednesses_[ normal_  .GetName() ] = &normal_;
    tirednesses_[ fatigue_ .GetName() ] = &fatigue_;
    tirednesses_[ epuise_  .GetName() ] = &epuise_;

    archive.Section( "FacteursHumains" );
    archive.Section( "Fatigue" );

    for( CIT_TirednessMap it = tirednesses_.begin(); it != tirednesses_.end(); ++it )
    {
        PHY_Tiredness& tiredness = const_cast< PHY_Tiredness& >( *it->second );
        archive.Section( tiredness.GetName() );
        tiredness.Read( archive );
        archive.EndSection(); // tiredness.GetName()
    }

    archive.EndSection(); // Fatigue
    archive.EndSection(); // FacteursHumains
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Tiredness::Terminate()
{
    tirednesses_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Tiredness::PHY_Tiredness( const std::string& strName, E_TirednessType nType, ASN1T_EnumUnitFatigue nAsnID, MT_Float rDIAWeight )
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
// Name: PHY_Tiredness destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Tiredness::~PHY_Tiredness()
{
}


// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Read
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void PHY_Tiredness::Read( MIL_InputArchive& archive )
{
    archive.ReadField( "CoefModificationVitesseMax"        , rCoefMaxSpeedModificator_      , CheckValueGreater( 0. ) );
    archive.ReadField( "CoefModificationTempsRechargement" , rCoefReloadingTimeModificator_ , CheckValueGreater( 0. ) );
    archive.ReadField( "CoefModificationPH"                , rCoefPhModificator_            , CheckValueGreater( 0. ) );
    archive.ReadField( "CoefModificationTempsMiseEnPosture", rCoefPostureTimeModificator_   , CheckValueGreater( 0. ) );
    archive.ReadField( "CoefModificationDistanceCapteurs"  , rCoefSensorDistanceModificator_, CheckValueGreater( 0. ) );
}
