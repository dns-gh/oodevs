// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposanteType_ABC.cpp $
// $Author: Jvt $
// $Modtime: 19/10/04 18:00 $
// $Revision: 2 $
// $Workfile: PHY_ComposanteType_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_ComposanteType_ABC.h"

#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteType_ABC constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_ComposanteType_ABC::PHY_ComposanteType_ABC( const std::string& strName, MIL_InputArchive& archive )
    : strName_           ( strName )
    , pProtection_       ( 0 )
    , pVolume_           ( 0 )
{
    archive.ReadField( "MosID", nMosID_ );

    std::string strVal;
    archive.ReadField( "Protection", strVal );
    
    pProtection_ = PHY_Protection::Find( strVal );
    if ( !pProtection_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown protection '%s'", strVal.c_str() ), archive.GetContext() );

    archive.ReadField( "Volume", strVal );

    pVolume_ = PHY_Volume::FindVolume( strVal );
    if ( !pVolume_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown volume '%s'", strVal.c_str() ), archive.GetContext() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteType_ABC destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_ComposanteType_ABC::~PHY_ComposanteType_ABC()
{
}
