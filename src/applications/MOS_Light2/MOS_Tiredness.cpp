// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Tiredness.cpp $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: MOS_Tiredness.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_Tiredness.h"

MOS_Tiredness::T_TirednessMap MOS_Tiredness::tirednesses_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

const MOS_Tiredness MOS_Tiredness::normal_ ( "Normal" , eNormal , EnumUnitFatigue::normal  );
const MOS_Tiredness MOS_Tiredness::fatigue_( "Fatigue", eFatigue, EnumUnitFatigue::fatigue );
const MOS_Tiredness MOS_Tiredness::epuise_ ( "Epuise" , eEpuise , EnumUnitFatigue::epuise  );

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MOS_Tiredness::Initialize( MT_InputArchive_ABC& archive )
{
    MT_LOG_INFO_MSG( "Initializing tirenesses" );
    tirednesses_[ normal_  .GetName() ] = &normal_;
    tirednesses_[ fatigue_ .GetName() ] = &fatigue_;
    tirednesses_[ epuise_  .GetName() ] = &epuise_;

    archive.Section( "FacteursHumains" );
    archive.Section( "Fatigue" );

    for( CIT_TirednessMap it = tirednesses_.begin(); it != tirednesses_.end(); ++it )
    {
        MOS_Tiredness& tiredness = const_cast< MOS_Tiredness& >( *it->second );
        archive.Section( tiredness.GetName() );
        tiredness.Read( archive );
        archive.EndSection(); // tiredness.GetName()
    }

    archive.EndSection(); // Fatigue
    archive.EndSection(); // FacteursHumains
}

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MOS_Tiredness::Terminate()
{
    tirednesses_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MOS_Tiredness::MOS_Tiredness( const std::string& strName, E_TirednessType nType, ASN1T_EnumUnitFatigue nAsnID )
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
// Name: MOS_Tiredness destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MOS_Tiredness::~MOS_Tiredness()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_Tiredness::Read
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void MOS_Tiredness::Read( MT_InputArchive_ABC& archive )
{
    archive.ReadField( "CoefModificationVitesseMax"        , rCoefMaxSpeedModificator_      );
    archive.ReadField( "CoefModificationTempsRechargement" , rCoefReloadingTimeModificator_ );
    archive.ReadField( "CoefModificationPH"                , rCoefPhModificator_            );
    archive.ReadField( "CoefModificationTempsMiseEnPosture", rCoefPostureTimeModificator_   );
    archive.ReadField( "CoefModificationDistanceCapteurs"  , rCoefSensorDistanceModificator_);
}
