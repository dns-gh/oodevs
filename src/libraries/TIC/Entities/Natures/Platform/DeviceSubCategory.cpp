// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Platform/DeviceSubCategory.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:06 $
// $Revision: 2 $
// $Workfile: DeviceSubCategory.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"

#include "DeviceSubCategory.h"

using namespace TIC;

DeviceSubCategory::T_DeviceSubCategoryMap DeviceSubCategory::deviceSubCategories_;
const DeviceSubCategory                DeviceSubCategory::notDefined( "None", eNone, "NONE" );

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DeviceSubCategory::Initialize
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void DeviceSubCategory::Initialize()
{
    deviceSubCategories_[ "AEWCON" ] = new DeviceSubCategory( "Airborne early warning and control", eAirborneEarlyWarningAndControl, "AEWCON" );
    deviceSubCategories_[ "AIRTRI" ] = new DeviceSubCategory( "Air Transport Medium Range", eAirTransportMediumRange, "AIRTRI" );
    deviceSubCategories_[ "AIRTRL" ] = new DeviceSubCategory( "Air Transport Long Range", eAirTransportLongRange, "AIRTRL" );
    deviceSubCategories_[ "AIRTRS" ] = new DeviceSubCategory( "Air Transport Short Range", eAirTransportShortRange, "AIRTRS" );
    deviceSubCategories_[ "ANTARM" ] = new DeviceSubCategory( "Anti armour", eAntiArmour, "ANTARM" );
    deviceSubCategories_[ "ASWAIR" ] = new DeviceSubCategory( "Anti submarine", eAntiSubmarine, "ASWAIR" );
    deviceSubCategories_[ "ATTAIR" ] = new DeviceSubCategory( "Attack aircraft", eAttackAircraft, "ATTAIR" );
    deviceSubCategories_[ "ATTHEL" ] = new DeviceSubCategory( "Attack helicopter", eAttackHelicopter, "ATTHEL" );
    deviceSubCategories_[ "AUTOGY" ] = new DeviceSubCategory( "Autogyro", eAutogyro, "AUTOGY" );
    deviceSubCategories_[ "BOMBIR" ] = new DeviceSubCategory( "Bomber medium range", eBomberMediumRange, "BOMBIR" );
    deviceSubCategories_[ "BOMBLR" ] = new DeviceSubCategory( "Bomber long range", eBomberLongRange, "BOMBLR" );
    deviceSubCategories_[ "BOMBSR" ] = new DeviceSubCategory( "Bomber short range", eBomberShortRange, "BOMBSR" );
    deviceSubCategories_[ "CMDCTR" ] = new DeviceSubCategory( "Command and control", eCommandAndControl, "CMDCTR" );
    deviceSubCategories_[ "DRONE" ]  = new DeviceSubCategory( "Drone", eDrone, "DRONE" );
    deviceSubCategories_[ "ELCCNM" ] = new DeviceSubCategory( "Electronic counter measures", eElectronicCounterMeasures, "ELCCNM" );
    deviceSubCategories_[ "EWAIR" ]  = new DeviceSubCategory( "Electronic warfare", eElectronicWarfare, "EWAIR" );
    deviceSubCategories_[ "FIGBOM" ] = new DeviceSubCategory( "Fighter bomber", eFighterBomber, "FIGBOM" );
    deviceSubCategories_[ "FIGHTR" ] = new DeviceSubCategory( "Fighter", eFighter, "FIGHTR" );
    deviceSubCategories_[ "HELHVL" ] = new DeviceSubCategory( "Helicopter heavy lift", eHelicopterHeavyLift, "HELHVL" );
    deviceSubCategories_[ "HELLGL" ] = new DeviceSubCategory( "Helicopter light lift", eHelicopterLightLift, "HELLGL" );
    deviceSubCategories_[ "HELMDL" ] = new DeviceSubCategory( "Helicopter medium lift", eHelicopterMediumLift, "HELMDL" );
    deviceSubCategories_[ "HELREC" ] = new DeviceSubCategory( "Helicopter reconnaissance", eHelicopterReconnaissance, "HELREC" );
    deviceSubCategories_[ "IMINGT" ] = new DeviceSubCategory( "Imagery intelligence gathering", eImageryIntelligenceGathering, "IMINGT" );
    deviceSubCategories_[ "MEDEVC" ] = new DeviceSubCategory( "Medical evacuation", eMedicalEvacuation, "MEDEVC" );
    deviceSubCategories_[ "NKN" ]    = new DeviceSubCategory( "Not known", eNotKnown, "NKN" );
    deviceSubCategories_[ "NOS" ]    = new DeviceSubCategory( "Not otherwise specified", eNotOtherwiseSpecified, "NOS" );
    deviceSubCategories_[ "RECAIR" ] = new DeviceSubCategory( "Reconnaissance", eReconnaissance, "RECAIR" );
    deviceSubCategories_[ "SAR" ]    = new DeviceSubCategory( "Search and rescue", eSearchAndRescue, "SAR" );
    deviceSubCategories_[ "SIINGA" ] = new DeviceSubCategory( "Signals intelligence gathering", eSignalsIntelligenceGathering, "SIINGA" );
    deviceSubCategories_[ "TANAIR" ] = new DeviceSubCategory( "Tanker", eTanker, "TANAIR" );
    deviceSubCategories_[ "TRAAIR" ] = new DeviceSubCategory( "Trainer", eTrainer, "TRAAIR" );
    deviceSubCategories_[ "UAV" ]    = new DeviceSubCategory( "Unmanned aerial vehicle", eUnmannedAerialVehicle, "UAV" );
    deviceSubCategories_[ "UTIAIR" ] = new DeviceSubCategory( "Utility", eUtility, "UTIAIR" );
    deviceSubCategories_[ "NONE" ]   = &notDefined;
}

// -----------------------------------------------------------------------------
// Name: DeviceSubCategory::Terminate
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void DeviceSubCategory::Terminate()
{
    for( CIT_DeviceSubCategoryMap it = deviceSubCategories_.begin(); it != deviceSubCategories_.end(); ++it )
        if( it->second != &notDefined )
            delete it->second;
    deviceSubCategories_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DeviceSubCategory constructor
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
DeviceSubCategory::DeviceSubCategory( const std::string& strName, E_Type nType, const std::string& strKey )
    : strName_ ( strName )
    , nType_   ( nType   )
    , strKey_  ( strKey  )
{
}

// -----------------------------------------------------------------------------
// Name: DeviceSubCategory destructor
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
DeviceSubCategory::~DeviceSubCategory()
{
}