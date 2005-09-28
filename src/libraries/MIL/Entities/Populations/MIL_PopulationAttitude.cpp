// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Humans/MIL_PopulationAttitude.cpp $
// $Author: Nld $
// $Modtime: 29/12/04 14:01 $
// $Revision: 2 $
// $Workfile: MIL_PopulationAttitude.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_PopulationAttitude.h"

MIL_PopulationAttitude::T_AttitudeMap MIL_PopulationAttitude::attitudes_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::Initialize
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void MIL_PopulationAttitude::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing population attitudes" );
    attitudes_[ "calme"     ] = new MIL_PopulationAttitude( "calme"    , eCalme    , EnumPopulationAttitude::calme     );
    attitudes_[ "agitee"    ] = new MIL_PopulationAttitude( "agitee"   , eAgitee   , EnumPopulationAttitude::agitee    );
    attitudes_[ "excitee"   ] = new MIL_PopulationAttitude( "excitee"  , eExcitee  , EnumPopulationAttitude::excitee   );
    attitudes_[ "agressive" ] = new MIL_PopulationAttitude( "agressive", eAgressive, EnumPopulationAttitude::agressive );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::Terminate
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void MIL_PopulationAttitude::Terminate()
{
    for( CIT_AttitudeMap it = attitudes_.begin(); it != attitudes_.end(); ++it )
        delete it->second;
    attitudes_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationAttitude::MIL_PopulationAttitude( const std::string& strName, E_Attitude nAttitude, ASN1T_EnumPopulationAttitude nAsnID )
    : strName_  ( strName   )
    , nAttitude_( nAttitude )
    , nAsnID_   ( nAsnID    )
{

}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude destructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
MIL_PopulationAttitude::~MIL_PopulationAttitude()
{

}
