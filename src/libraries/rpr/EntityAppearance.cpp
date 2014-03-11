// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "EntityAppearance.h"

using namespace rpr;


#define APPEARANCE_ACCESSOR_BODY(type, name, mask, shift) \
        uint8_t type::name() const \
        { \
            return getValue( value_, static_cast<uint32_t>(mask), shift) ; \
        } \
        void type::Set##name(uint8_t v) \
        { \
            setValue(value_, v, static_cast<uint32_t>(mask), shift); \
        }

namespace
{
    enum AppearanceLand_FieldShifts
    {
        land_shift_paintScheme = 0,
        land_shift_mobilityKill = 1, // 1 - Characteristics of mobility kill
        land_shift_firePowerKill = 2, // 2 - Characteristics of fire-power kill
        land_shift_damage = 3, // 3-4 - Damaged appearance of an entity  0 - No damage, 1 - Slight damage, 2 - Moderate damage, 3 - Destroyed
        land_shift_smoke = 5, // 5-6 - Status or location of smoke emanating from an entity 0 - Not smoking, 1 - Smoke plume rising from the entity, 2 - Entity is emitting engine smoke, 3 - Entity is emitting engine smoke, and smoke plume is rising from the entity
        land_shift_trailing = 7, // 7-8 - Size of the dust cloud trailing effect for the Effects entity 0 - None, 1 - Small, 2 - Medium, 3 - Large
        land_shift_hatch = 9, // 9-11 - Describes the state of the primary hatch, 0 - Not applicable, 1 - Primary hatch is closed, 2 - Primary hatch is popped, 3 - Primary hatch is popped and a person is visible under hatch, 4 - Primary hatch is open, 5 - Primary hatch is open and person is visible, 6-7 Unused
        land_shift_headLightsOn = 12, // 12 - Describes whether Head Lights are on or off.
        land_shift_tailLightsOn = 13, // 13 - Describes whether Tail Lights are on or off.
        land_shift_brakeLightsOn = 14, // 14 - Describes whether Brake Lights are on or off.
        land_shift_flaming = 15, // 15 - Describes whether flames are rising from an entity
        land_shift_launcherRaised = 16, // 16 - Describes the elevated status of the platform's primary launcher
        land_shift_camouflageType = 17, // 17-18 - Describes the type of camouflage 0 - Desert camouflage, 1 - Winter camouflage, 2 - Forest camouflage, 3 - Unused
        land_shift_concealed = 19, // 19 - Describes the type of concealment
        land_shift_frozen = 21, // 21 - Describes the frozen status of a Land Entity
        land_shift_powerPlantOn = 22, // 22 - Describes the power-plant status of platform
        land_shift_deactivated = 23, // 23 - Describes the state of a Land Entity
        land_shift_tentExtension = 24, // 24 - Describes the status of a tent extension
        land_shift_rampDown = 25, // 25 - Describes the status of a ramp
        land_shift_blackoutLightsOn = 26, // 26 - Describes whether Blackout Lights are on or off.
        land_shift_blackoutBreakLightsOn = 27, // 27 - Describes whether Blackout Brake Lights are on or off.
        land_shift_spotLightsOn = 28, // 28 - Describes whether Spot Lights are on or off.
        land_shift_interiorLightsOn = 29, // 29 - Describes whether Interior Lights are on or off.
        land_shift_surrendered = 30, // 30 - Describes the surrender state of the vehicle occupants
        land_shift_masked = 31 // 31 - 0:not masked, 1:masked
    };
    enum AppearanceLand_FieldMasks
    {
        land_mask_paintScheme = 1u, // 0 - 0:Uniform, 1:Camouflage
        land_mask_mobilityKill = 1u << land_shift_mobilityKill, // 1 - Characteristics of mobility kill
        land_mask_firePowerKill = 1u << land_shift_firePowerKill, // 2 - Characteristics of fire-power kill
        land_mask_damage = 3u << land_shift_damage , // 3-4 - Damaged appearance of an entity  0 - No damage, 1 - Slight damage, 2 - Moderate damage, 3 - Destroyed
        land_mask_smoke= 3u << land_shift_smoke, // 5-6 - Status or location of smoke emanating from an entity 0 - Not smoking, 1 - Smoke plume rising from the entity, 2 - Entity is emitting engine smoke, 3 - Entity is emitting engine smoke, and smoke plume is rising from the entity
        land_mask_trailing = 3u << land_shift_trailing, // 7-8 - Size of the dust cloud trailing effect for the Effects entity 0 - None, 1 - Small, 2 - Medium, 3 - Large
        land_mask_hatch = 7u << land_shift_hatch, // 9-11 - Describes the state of the primary hatch, 0 - Not applicable, 1 - Primary hatch is closed, 2 - Primary hatch is popped, 3 - Primary hatch is popped and a person is visible under hatch, 4 - Primary hatch is open, 5 - Primary hatch is open and person is visible, 6-7 Unused
        land_mask_headLightsOn = 1u << land_shift_headLightsOn, // 12 - Describes whether Head Lights are on or off.
        land_mask_tailLightsOn = 1u << land_shift_tailLightsOn, // 13 - Describes whether Tail Lights are on or off.
        land_mask_brakeLightsOn = 1u << land_shift_brakeLightsOn, // 14 - Describes whether Brake Lights are on or off.
        land_mask_flaming = 1u << land_shift_flaming, // 15 - Describes whether flames are rising from an entity
        land_mask_launcherRaised = 1u << land_shift_launcherRaised, // 16 - Describes the elevated status of the platform's primary launcher
        land_mask_camouflageType = 3u << land_shift_camouflageType, // 17-18 - Describes the type of camouflage 0 - Desert camouflage, 1 - Winter camouflage, 2 - Forest camouflage, 3 - Unused
        land_mask_concealed = 1u << land_shift_concealed, // 19 - Describes the type of concealment
        land_mask_frozen = 1u << land_shift_frozen, // 21 - Describes the frozen status of a Land Entity
        land_mask_powerPlantOn = 1u << land_shift_powerPlantOn, // 22 - Describes the power-plant status of platform
        land_mask_deactivated = 1u << land_shift_deactivated, // 23 - Describes the state of a Land Entity
        land_mask_tentExtension = 1u << land_shift_tentExtension, // 24 - Describes the status of a tent extension
        land_mask_rampDown = 1u << land_shift_rampDown, // 25 - Describes the status of a ramp
        land_mask_blackoutLightsOn = 1u << land_shift_blackoutLightsOn, // 26 - Describes whether Blackout Lights are on or off.
        land_mask_blackoutBreakLightsOn = 1u << land_shift_blackoutBreakLightsOn, // 27 - Describes whether Blackout Brake Lights are on or off.
        land_mask_spotLightsOn = 1u << land_shift_spotLightsOn, // 28 - Describes whether Spot Lights are on or off.
        land_mask_interiorLightsOn = 1u << land_shift_interiorLightsOn, // 29 - Describes whether Interior Lights are on or off.
        land_mask_surrendered = 1u << land_shift_surrendered, // 30 - Describes the surrender state of the vehicle occupants
        land_mask_masked = 1u << land_shift_masked // 31 - 0:not masked, 1:masked
    };
    inline uint8_t getValue( uint32_t v, uint32_t mask, uint32_t shift )
    {
        return static_cast< uint8_t >( (v & mask) >> shift );
    }
    inline void setValue( uint32_t& val, uint8_t newVal, uint32_t mask, uint32_t shift )
    {
        uint32_t t = (newVal<<shift);
        val &= ~(mask);
        val |= t;
    }
}

APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, PaintScheme, land_mask_paintScheme, land_shift_paintScheme);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, MobilityKill, land_mask_mobilityKill, land_shift_mobilityKill);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, FirePowerKill, land_mask_firePowerKill, land_shift_firePowerKill);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, Damage, land_mask_damage, land_shift_damage);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, Smoke, land_mask_damage, land_shift_damage);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, Trailing, land_mask_trailing, land_shift_trailing);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, Hatch, land_mask_hatch, land_shift_hatch);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, HeadLightsOn, land_mask_headLightsOn, land_shift_headLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, TailLightsOn, land_mask_tailLightsOn, land_shift_tailLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, BrakeLightsOn, land_mask_brakeLightsOn, land_shift_brakeLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, Flaming, land_mask_flaming, land_shift_flaming);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, LauncherRaised, land_mask_launcherRaised, land_shift_launcherRaised);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, CamouflageType, land_mask_camouflageType, land_shift_camouflageType);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, Concealed, land_mask_camouflageType, land_shift_camouflageType);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, Frozen, land_mask_frozen, land_shift_frozen);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, PowerPlantOn, land_mask_powerPlantOn, land_shift_powerPlantOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, Deactivated, land_mask_deactivated, land_shift_deactivated);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, TentExtension, land_mask_tentExtension, land_shift_tentExtension);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, RampDown, land_mask_rampDown, land_shift_rampDown);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, BlackoutLightsOn, land_mask_blackoutLightsOn, land_shift_blackoutLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, BlackoutBreakLightsOn, land_mask_blackoutBreakLightsOn, land_shift_blackoutBreakLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, SpotLightsOn, land_mask_spotLightsOn, land_shift_spotLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, InteriorLightsOn, land_mask_interiorLightsOn, land_shift_interiorLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, Surrendered, land_mask_surrendered, land_shift_surrendered);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Land, Masked, land_mask_masked, land_shift_masked);

namespace
{
    enum AppearanceAir_FieldShifts
    {
        air_shift_paintScheme = 0,
        air_shift_propulsionKill = 1, // 1 - Characteristics of mobility kill
        air_shift_damage = 3, // 3-4 - Damaged appearance of an entity  0 - No damage, 1 - Slight damage, 2 - Moderate damage, 3 - Destroyed
        air_shift_smoke = 5, // 5-6 - Status or location of smoke emanating from an entity 0 - Not smoking, 1 - Smoke plume rising from the entity, 2 - Entity is emitting engine smoke, 3 - Entity is emitting engine smoke, and smoke plume is rising from the entity
        air_shift_trailing = 7, // 7-8 - Size of the dust cloud trailing effect for the Effects entity 0 - None, 1 - Small, 2 - Medium, 3 - Large
        air_shift_canopy = 9, // 9-11 - Describes the state of the primary hatch, 0 - Not applicable, 1 - Primary hatch is closed, 2 - Primary hatch is popped, 3 - Primary hatch is popped and a person is visible under hatch, 4 - Primary hatch is open, 5 - Primary hatch is open and person is visible, 6-7 Unused
        air_shift_landingLightsOn = 12, // 12 - Describes whether Head Lights are on or off.
        air_shift_navigationLightsOn = 13, // 13 - Describes whether Tail Lights are on or off.
        air_shift_antiColisionLightsOn = 14, // 14 - Describes whether Brake Lights are on or off.
        air_shift_flaming = 15, // 15 - Describes whether flames are rising from an entity
        air_shift_afterburnerOn = 16, // 16 - Describes the elevated status of the platform's primary launcher
        air_shift_frozen = 21, // 21 - Describes the frozen status of a Land Entity
        air_shift_powerPlantOn = 22, // 22 - Describes the power-plant status of platform
        air_shift_deactivated = 23, // 23 - Describes the state of a Air Entity
        air_shift_formationLightsOn = 27, // 27 - Describes whether Blackout Brake Lights are on or off.
        air_shift_spotLightsOn = 28, // 28 - Describes whether Spot Lights are on or off.
        air_shift_interiorLightsOn = 29, // 29 - Describes whether Interior Lights are on or off.
    };
    enum AppearanceAir_FieldMasks
    {
        air_mask_paintScheme = 1u, // 0 - 0:Uniform, 1:Camouflage
        air_mask_propulsionKill = 1u << air_shift_propulsionKill, // 1 - Characteristics of mobility kill
        air_mask_damage = 3u << air_shift_damage , // 3-4 - Damaged appearance of an entity  0 - No damage, 1 - Slight damage, 2 - Moderate damage, 3 - Destroyed
        air_mask_smoke= 3u << air_shift_smoke, // 5-6 - Status or location of smoke emanating from an entity 0 - Not smoking, 1 - Smoke plume rising from the entity, 2 - Entity is emitting engine smoke, 3 - Entity is emitting engine smoke, and smoke plume is rising from the entity
        air_mask_trailing = 3u << air_shift_trailing, // 7-8 - Size of the dust cloud trailing effect for the Effects entity 0 - None, 1 - Small, 2 - Medium, 3 - Large
        air_mask_canopy = 7u << air_shift_canopy, // 9-11 - Describes the state of the primary hatch, 0 - Not applicable, 1 - Primary hatch is closed, 2 - Primary hatch is popped, 3 - Primary hatch is popped and a person is visible under hatch, 4 - Primary hatch is open, 5 - Primary hatch is open and person is visible, 6-7 Unused
        air_mask_landingLightsOn = 1u << air_shift_landingLightsOn, // 12 - Describes whether Head Lights are on or off.
        air_mask_navigationLightsOn = 1u << air_shift_navigationLightsOn, // 13 - Describes whether Tail Lights are on or off.
        air_mask_antiColisionLightsOn = 1u << air_shift_antiColisionLightsOn, // 14 - Describes whether Brake Lights are on or off.
        air_mask_flaming = 1u << air_shift_flaming, // 15 - Describes whether flames are rising from an entity
        air_mask_afterburnerOn = 1u << air_shift_afterburnerOn, // 16 - Describes the elevated status of the platform's primary launcher
        air_mask_frozen = 1u << air_shift_frozen, // 21 - Describes the frozen status of a Land Entity
        air_mask_powerPlantOn = 1u << air_shift_powerPlantOn, // 22 - Describes the power-plant status of platform
        air_mask_deactivated = 1u << air_shift_deactivated, // 23 - Describes the state of a Land Entity
        air_mask_formationLightsOn = 1u << air_shift_formationLightsOn, // 24 - Describes the status of a tent extension
        air_mask_spotLightsOn = 1u << air_shift_spotLightsOn, // 28 - Describes whether Spot Lights are on or off.
        air_mask_interiorLightsOn = 1u << air_shift_interiorLightsOn, // 29 - Describes whether Interior Lights are on or off.
    };
}

APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, PaintScheme, air_mask_paintScheme, air_shift_paintScheme);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, PropulsionKill, air_mask_propulsionKill, air_shift_propulsionKill);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, Damage, air_mask_damage, air_shift_damage);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, Smoke, air_mask_smoke, air_shift_smoke);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, Trailing, air_mask_trailing, air_shift_trailing);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, Canopy, air_mask_canopy, air_shift_canopy);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, LandingLightsOn, air_mask_landingLightsOn, air_mask_landingLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, NavigationLightsOn, air_mask_navigationLightsOn, air_shift_navigationLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, AntiColisionLightsOn, air_mask_antiColisionLightsOn, air_mask_antiColisionLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, Flaming, air_mask_flaming, air_shift_flaming);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, AfterburnerOn, air_mask_afterburnerOn, air_shift_afterburnerOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, Frozen, air_mask_frozen, air_shift_frozen);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, PowerPlantOn, air_mask_powerPlantOn, air_shift_powerPlantOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, Deactivated, air_mask_deactivated, air_shift_deactivated);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, FormationLightsOn, air_mask_formationLightsOn, air_shift_formationLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, SpotLightsOn, air_mask_spotLightsOn, air_shift_spotLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Air, InteriorLightsOn, air_mask_interiorLightsOn, air_shift_interiorLightsOn);

namespace
{
    enum AppearanceSurface_FieldShifts
    {
        surface_shift_paintScheme = 0,
        surface_shift_mobilityKill = 1, // 1 - Characteristics of mobility kill
        surface_shift_damage = 3, // 3-4 - Damaged appearance of an entity  0 - No damage, 1 - Slight damage, 2 - Moderate damage, 3 - Destroyed
        surface_shift_smoke = 5, // 5-6 - Status or location of smoke emanating from an entity 0 - Not smoking, 1 - Smoke plume rising from the entity, 2 - Entity is emitting engine smoke, 3 - Entity is emitting engine smoke, and smoke plume is rising from the entity
        surface_shift_trailing = 7, // 7-8 - Size of the dust cloud trailing effect for the Effects entity 0 - None, 1 - Small, 2 - Medium, 3 - Large
        surface_shift_runningLightsOn = 12, // 12 - Describes whether Head Lights are on or off.
        surface_shift_flaming = 15, // 15 - Describes whether flames are rising from an entity
        surface_shift_afterburnerOn = 16, // 16 - Describes the elevated status of the platform's primary launcher
        surface_shift_frozen = 21, // 21 - Describes the frozen status of a Land Entity
        surface_shift_powerPlantOn = 22, // 22 - Describes the power-plant status of platform
        surface_shift_deactivated = 23, // 23 - Describes the state of a Air Entity
        surface_shift_spotLightsOn = 28, // 28 - Describes whether Spot Lights are on or off.
        surface_shift_interiorLightsOn = 29, // 29 - Describes whether Interior Lights are on or off.
    };
    enum AppearanceSurface_FieldMasks
    {
        surface_mask_paintScheme = 1u, // 0 - 0:Uniform, 1:Camouflage
        surface_mask_mobilityKill = 1u << surface_shift_mobilityKill, // 1 - Characteristics of mobility kill
        surface_mask_damage = 3u << surface_shift_damage , // 3-4 - Damaged appearance of an entity  0 - No damage, 1 - Slight damage, 2 - Moderate damage, 3 - Destroyed
        surface_mask_smoke= 3u << surface_shift_smoke, // 5-6 - Status or location of smoke emanating from an entity 0 - Not smoking, 1 - Smoke plume rising from the entity, 2 - Entity is emitting engine smoke, 3 - Entity is emitting engine smoke, and smoke plume is rising from the entity
        surface_mask_trailing = 3u << surface_shift_trailing, // 7-8 - Size of the dust cloud trailing effect for the Effects entity 0 - None, 1 - Small, 2 - Medium, 3 - Large
        surface_mask_runningLightsOn = 1u << surface_shift_runningLightsOn, // 12 - Describes whether Head Lights are on or off.
        surface_mask_flaming = 1u << surface_shift_flaming, // 15 - Describes whether flames are rising from an entity
        surface_mask_afterburnerOn = 1u << surface_shift_afterburnerOn, // 16 - Describes the elevated status of the platform's primary launcher
        surface_mask_frozen = 1u << surface_shift_frozen, // 21 - Describes the frozen status of a Land Entity
        surface_mask_powerPlantOn = 1u << surface_shift_powerPlantOn, // 22 - Describes the power-plant status of platform
        surface_mask_deactivated = 1u << surface_shift_deactivated, // 23 - Describes the state of a Land Entity
        surface_mask_spotLightsOn = 1u << surface_shift_spotLightsOn, // 28 - Describes whether Spot Lights are on or off.
        surface_mask_interiorLightsOn = 1u << surface_shift_interiorLightsOn, // 29 - Describes whether Interior Lights are on or off.
    };
}

APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, PaintScheme, surface_mask_paintScheme, surface_shift_paintScheme);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, MobilityKill, surface_mask_mobilityKill, surface_shift_mobilityKill);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, Damage, surface_mask_damage, surface_shift_damage);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, Smoke, surface_mask_smoke, surface_shift_smoke);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, Trailing, surface_mask_trailing, surface_shift_trailing);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, RunningLightsOn, surface_mask_runningLightsOn, surface_shift_runningLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, Flaming, surface_mask_flaming, surface_shift_flaming);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, AfterburnerOn, surface_mask_afterburnerOn, surface_shift_afterburnerOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, Frozen, surface_mask_frozen, surface_shift_frozen);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, PowerPlantOn, surface_mask_powerPlantOn, surface_shift_powerPlantOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, Deactivated, surface_mask_deactivated, surface_shift_deactivated);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, SpotLightsOn, surface_mask_spotLightsOn, surface_shift_spotLightsOn);
APPEARANCE_ACCESSOR_BODY(EntityAppearance_Surface, InteriorLightsOn, surface_mask_interiorLightsOn, surface_shift_interiorLightsOn);

