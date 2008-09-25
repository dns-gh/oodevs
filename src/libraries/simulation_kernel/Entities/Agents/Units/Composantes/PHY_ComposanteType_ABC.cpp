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
#include <xeumeuleu/xml.h>



// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteType_ABC constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_ComposanteType_ABC::PHY_ComposanteType_ABC( const std::string& strName, xml::xistream& xis )
    : strName_           ( strName )
    , pProtection_       ( 0 )
    , pVolume_           ( 0 )
{
    std::string strVal;
    xis >> xml::attribute( "id", nMosID_ )
        >> xml::attribute( "protection", strVal );
    pProtection_ = PHY_Protection::Find( strVal );
    if ( !pProtection_ )
        xis.error( "Unknown protection '" + strVal + "'" );

    xis >> xml::attribute( "size", strVal );
    pVolume_ = PHY_Volume::FindVolume( strVal );
    if ( !pVolume_ )
        xis.error( "Unknown volume '" + strVal + "'" );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteType_ABC destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_ComposanteType_ABC::~PHY_ComposanteType_ABC()
{
    // NOTHING
}
