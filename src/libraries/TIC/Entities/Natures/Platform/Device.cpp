// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Platform/Device.cpp $
// $Author: Sbo $
// $Modtime: 22/07/05 11:01 $
// $Revision: 8 $
// $Workfile: Device.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"

#include "Device.h"

using namespace TIC;

Device::T_DeviceMap Device::devices_;
const Device        Device::notDefined( "None", eNone, "NONE" );

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Device::Initialize
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void Device::Initialize()
{
    devices_[ "arme" ]                   = new Device( "arme"                   , eArme                  , "WEAPON"  );
    devices_[ "equipementNBC" ]          = new Device( "Equipement NBC"         , eEquipementNBC         , "EQNBC"   );
    devices_[ "aeronef" ]                = new Device( "Aeronef"                , eAeronef               , "AIR"     );
    devices_[ "equipementGenie" ]        = new Device( "Equipement du genie"    , eEquipementGenie       , "EQINGE"  );
    devices_[ "vehicule" ]               = new Device( "Vehicule"               , eVehicule              , "VEHICU"  );
    devices_[ "equipementElectronique" ] = new Device( "Equipement electronique", eEquipementElectronique, "EQELECT" );
    devices_[ "batiment" ]               = new Device( "Batiment"               , eBatiment              , "BUILD"   );
    devices_[ "autreTypeEquipement" ]    = new Device( "Autre type d'equipement", eAutreTypeEquipement   , "OTHEREQ" );
    devices_[ notDefined.GetName() ]     = &notDefined;
}

// -----------------------------------------------------------------------------
// Name: Device::Terminate
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void Device::Terminate()
{
    for( CIT_DeviceMap it = devices_.begin(); it != devices_.end(); ++it )
        if( it->second != &notDefined )
            delete it->second;
    devices_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Device constructor
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
Device::Device( const std::string& strName, E_Type nType, const std::string& strKey )
    : strName_ ( strName )
    , nType_   ( nType   )
    , strKey_  ( strKey  )
{
}

// -----------------------------------------------------------------------------
// Name: Device destructor
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
Device::~Device()
{
}