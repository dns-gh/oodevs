// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Platform/DeviceCategory.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:05 $
// $Revision: 3 $
// $Workfile: DeviceCategory.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"

#include "DeviceCategory.h"

using namespace TIC;

DeviceCategory::T_DeviceCategoryMap DeviceCategory::deviceCategories_;
const DeviceCategory                DeviceCategory::notDefined( "None", eNone, "NONE" );

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DeviceCategory::Initialize
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void DeviceCategory::Initialize()
{
    deviceCategories_[ "FXD"  ]   = new DeviceCategory( "Fixed wing aircraft" , eFixedWingAircraft , "FXD"  );
    deviceCategories_[ "RTRY" ]   = new DeviceCategory( "Rotary wing aircraft", eRotaryWingAircraft, "RTRY" );
    deviceCategories_[ "AMBUL" ]  = new DeviceCategory( "Ambulance", eAmbulance, "AMBUL" );
    deviceCategories_[ "APC" ]    = new DeviceCategory( "Armoured personnel carrier", eArmouredPersonnelCarrier, "APC" );
    deviceCategories_[ "APCREC" ] = new DeviceCategory( "Armoured personnel carrier, recovery", eArmouredPersonnelCarrierRecovery, "APCREC" );
    deviceCategories_[ "ARMRCC" ] = new DeviceCategory( "Armoured reconnaissance carrier", eArmouredReconnaissanceCarrier, "ARMRCC" );
    deviceCategories_[ "ARVELT" ] = new DeviceCategory( "Armoured vehicle, light", eArmouredVehicleLight, "ARVELT" );
    deviceCategories_[ "ASSV" ]   = new DeviceCategory( "Armoured service support", eArmouredServiceSupport, "ASSV" );
    deviceCategories_[ "AUTOMO" ] = new DeviceCategory( "Automobile", eAutomobile, "AUTOMO" );
    deviceCategories_[ "BICYCL" ] = new DeviceCategory( "Bicycle", eBicycle, "BICYCL" );
    deviceCategories_[ "BTHVRE" ] = new DeviceCategory( "Battle tank, heavy, recovery", eBattleTankHeavyRecovery, "BTHVRE" );
    deviceCategories_[ "BTMDRE" ] = new DeviceCategory( "Battle tank, medium, recovery", eBattleTankMediumRecovery, "BTMDRE" );
    deviceCategories_[ "BUS" ]    = new DeviceCategory( "Bus", eBus, "BUS" );
    deviceCategories_[ "CCTRCK" ] = new DeviceCategory( "Cross country truck", eCrossCountryTruck, "CCTRCK" );
    deviceCategories_[ "FFVEH" ]  = new DeviceCategory( "Fire fighting", eFireFighting, "FFVEH" );
    deviceCategories_[ "HETVEH" ] = new DeviceCategory( "Heavy equipment transport", eHeavyEquipmentTransport, "HETVEH" );
    deviceCategories_[ "MAIVEH" ] = new DeviceCategory( "Maintenance", eMaintenance, "MAIVEH" );
    deviceCategories_[ "MHVEH" ]  = new DeviceCategory( "Materiel handling", eMaterielHandling, "MHVEH" );
    deviceCategories_[ "MILUV" ]  = new DeviceCategory( "Military Utility", eMilitaryUtility, "MILUV" );
    deviceCategories_[ "MOTCYC" ] = new DeviceCategory( "Motorcycle", eMotorcycle, "MOTCYC" );
    deviceCategories_[ "NKN" ]    = new DeviceCategory( "Not known", eNotKnown, "NKN" );
    deviceCategories_[ "NOS" ]    = new DeviceCategory( "Not otherwise specified", eNotOtherwiseSpecified, "NOS" );
    deviceCategories_[ "SEMI" ]   = new DeviceCategory( "Semi", eSemi, "SEMI" );
    deviceCategories_[ "TRACTR" ] = new DeviceCategory( "Tractor", eTractor, "TRACTR" );
    deviceCategories_[ "TRAILR" ] = new DeviceCategory( "Trailer", eTrailer, "TRAILR" );
    deviceCategories_[ "TRLBUS" ] = new DeviceCategory( "Trolley bus", eTrolleyBus, "TRLBUS" );
    deviceCategories_[ "TRUCK" ]  = new DeviceCategory( "Truck", eTruck, "TRUCK" );
    deviceCategories_[ "NONE" ]   = &notDefined;
}

// -----------------------------------------------------------------------------
// Name: DeviceCategory::Terminate
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void DeviceCategory::Terminate()
{
    for( CIT_DeviceCategoryMap it = deviceCategories_.begin(); it != deviceCategories_.end(); ++it )
        if( it->second != &notDefined )
            delete it->second;
    deviceCategories_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DeviceCategory constructor
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
DeviceCategory::DeviceCategory( const std::string& strName, E_Type nType, const std::string& strKey )
    : strName_ ( strName )
    , nType_   ( nType   )
    , strKey_  ( strKey  )
{
}

// -----------------------------------------------------------------------------
// Name: DeviceCategory destructor
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
DeviceCategory::~DeviceCategory()
{
}