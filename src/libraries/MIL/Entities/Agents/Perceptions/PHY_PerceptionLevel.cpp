// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionLevel.cpp $
// $Author: Jvt $
// $Modtime: 16/03/05 19:04 $
// $Revision: 2 $
// $Workfile: PHY_PerceptionLevel.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_PerceptionLevel.h"

const PHY_PerceptionLevel PHY_PerceptionLevel::identified_( "Identified", eIdentified, EnumUnitIdentificationLevel::identifiee );
const PHY_PerceptionLevel PHY_PerceptionLevel::recognized_( "Recognized", eRecognized, EnumUnitIdentificationLevel::reconnue   );
const PHY_PerceptionLevel PHY_PerceptionLevel::detected_  ( "Detected"  , eDetected  , EnumUnitIdentificationLevel::detectee   );
const PHY_PerceptionLevel PHY_PerceptionLevel::notSeen_   ( "NotSeen"   , eNotSeen   , EnumUnitIdentificationLevel::signale    );

PHY_PerceptionLevel::T_LevelMap PHY_PerceptionLevel::levels_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::Initialize
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_PerceptionLevel::Initialize()
{
    levels_[ identified_.GetID() ] = &identified_;
    levels_[ recognized_.GetID() ] = &recognized_;
    levels_[ detected_  .GetID() ] = &detected_  ;
    levels_[ notSeen_   .GetID() ] = &notSeen_   ;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel::Terminate
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_PerceptionLevel::Terminate()
{
    levels_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_PerceptionLevel::PHY_PerceptionLevel( const std::string& strName, E_Level nLevel, const ASN1T_EnumUnitIdentificationLevel& nAsnID )
    : strName_( strName )
    , nLevel_ ( nLevel  )
    , nAsnID_ ( nAsnID  )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionLevel destructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_PerceptionLevel::~PHY_PerceptionLevel()
{

}
