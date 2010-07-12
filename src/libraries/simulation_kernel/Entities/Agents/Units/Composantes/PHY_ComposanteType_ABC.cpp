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
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

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
    int equipmenttype_val;
    xis >> xml::attribute( "id", equipmenttype_val )
        >> xml::attribute( "protection", strVal );
    nMosID_.set_equipment( equipmenttype_val );
    pProtection_ = PHY_Protection::Find( strVal );
    if( !pProtection_ )
        xis.error( "Unknown protection '" + strVal + "'" );

    xis >> xml::attribute( "size", strVal );
    pVolume_ = PHY_Volume::FindVolume( strVal );
    if( !pVolume_ )
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

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteType_ABC::GetMosID
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
Common::MsgEquipmentType PHY_ComposanteType_ABC::GetMosID() const
{
    return nMosID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteType_ABC::GetProtection
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const PHY_Protection& PHY_ComposanteType_ABC::GetProtection() const
{
    assert( pProtection_ );
    return *pProtection_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteType_ABC::GetVolume
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const PHY_Volume& PHY_ComposanteType_ABC::GetVolume() const
{
    assert( pVolume_ );
    return *pVolume_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteType_ABC::GetName
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
const std::string& PHY_ComposanteType_ABC::GetName() const
{
    return strName_;
}
