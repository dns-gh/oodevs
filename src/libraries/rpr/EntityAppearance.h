// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __rpr_EntityAppearance_h_
#define __rpr_EntityAppearance_h_

#include <cstdint>

namespace rpr
{
    struct EntityAppearance_Land
    {
    public:
        union Content
        {
            struct Bitfield
            {
                unsigned paintScheme : 1; // O:Uniform, 1:Camouflage
                bool mobilityKill : 1; // Characteristics of mobility kill
                bool firePowerKill : 1; // Characteristics of fire-power kill
                unsigned damage : 2; // Damaged appearance of an entity  0 - No damage, 1 - Slight damage, 2 - Moderate damage, 3 - Destroyed
                unsigned smoke : 2; // Status or location of smoke emanating from an entity 0 - Not smoking, 1 - Smoke plume rising from the entity, 2 - Entity is emitting engine smoke, 3 - Entity is emitting engine smoke, and smoke plume is rising from the entity
                unsigned trailing : 2; // Size of the dust cloud trailing effect for the Effects entity 0 - None, 1 - Small, 2 - Medium, 3 - Large
                unsigned hatch : 3; // Describes the state of the primary hatch, 0 - Not applicable, 1 - Primary hatch is closed, 2 - Primary hatch is popped, 3 - Primary hatch is popped and a person is visible under hatch, 4 - Primary hatch is open, 5 - Primary hatch is open and person is visible, 6-7 Unused
                bool headLightsOn : 1; // Describes whether Head Lights are on or off.
                bool tailLightsOn : 1; // Describes whether Tail Lights are on or off.
                bool brakeLightsOn : 1; // Describes whether Brake Lights are on or off.
                bool flaming : 1; // Describes whether flames are rising from an entity
                bool launcherRaised : 1; // Describes the elevated status of the platform's primary launcher
                unsigned camouflageType : 2; // Describes the type of camouflage 0 - Desert camouflage, 1 - Winter camouflage, 2 - Forest camouflage, 3 - Unused
                bool concealed  : 1; // Describes the type of concealment
                unsigned unused_1 : 1;
                bool frozen : 1; //Describes the frozen status of a Land Entity
                bool powerPlantOn : 1; // Describes the power-plant status of platform
                bool deactivated : 1; // Describes the state of a Land Entity
                bool tentExtension : 1; // Describes the status of a tent extension
                bool rampDown : 1; // Describes the status of a ramp
                bool blackoutLightsOn : 1; // Describes whether Blackout Lights are on or off.
                bool blackoutBreakLightsOn : 1; // Describes whether Blackout Brake Lights are on or off.
                bool spotLightsOn : 1; // Describes whether Spot Lights are on or off.
                bool interiorLightsOn : 1; // Describes whether Interior Lights are on or off.
                bool surrendered : 1; // Describes the surrender state of the vehicle occupants
                unsigned unused_2 : 1;
            };
            Bitfield fields_;
            uint32_t value_;
        };

        //! @name Operations
        //@{
        template< typename Archive >
        void Serialize( Archive& archive ) const
        {
            archive << appearance_.value_;
        }
        template< typename Archive >
        void Deserialize( Archive& archive )
        {
            archive >> appearance_.value_;
        }
        //@}
        Content appearance_;
    };

    struct EntityAppearance_Air
    {
    public:
        union Content
        {
            struct Bitfield
            {
                unsigned paintScheme : 1; // O:Uniform, 1:Camouflage
                bool propulsionKill : 1; // Characteristics of propulsion kill
                bool unused_0 : 1;
                unsigned damage : 2; // Damaged appearance of an entity  0 - No damage, 1 - Slight damage, 2 - Moderate damage, 3 - Destroyed
                unsigned smoke : 2; // Status or location of smoke emanating from an entity 0 - Not smoking, 1 - Smoke plume rising from the entity, 2 - Entity is emitting engine smoke, 3 - Entity is emitting engine smoke, and smoke plume is rising from the entity
                unsigned trailing : 2; // Size of the dust cloud trailing effect for the Effects entity 0 - None, 1 - Small, 2 - Medium, 3 - Large
                unsigned canopy : 3; // Describes the state of the canopy, 0 - Not applicable, 1 - canopy is closed, 4 - Canopy is open, 5-7 Unused
                bool landingLightsOn : 1; // Describes whether Landing Lights are on or off.
                bool navigationLightsOn : 1; // Describes whether Navigation Lights are on or off.
                bool antiColisionLightsOn : 1; // Describes whether Anti Collision Lights are on or off.
                bool flaming : 1; // Describes whether flames are rising from an entity
                bool afterburnerOn : 1; // Describes whether afterburners are on
                unsigned unused_1 : 4;
                bool frozen : 1; //Describes the frozen status of a Land Entity
                bool powerPlantOn : 1; // Describes the power-plant status of platform
                bool deactivated : 1; // Describes the state of a Land Entity
                bool formationLightsOn : 1; // Describes wheter formation lights are on or off.
                bool unused_2 : 3;
                bool spotLightsOn : 1; // Describes whether Spot Lights are on or off.
                bool interiorLightsOn : 1; // Describes whether Interior Lights are on or off.
                unsigned unused_3 : 2;
            };
            Bitfield fields_;
            uint32_t value_;
        };

        //! @name Operations
        //@{
        template< typename Archive >
        void Serialize( Archive& archive ) const
        {
            archive << appearance_.value_;
        }
        template< typename Archive >
        void Deserialize( Archive& archive )
        {
            archive >> appearance_.value_;
        }
        //@}

        Content appearance_;
    };

    struct EntityAppearance_Surface
    {
    public:
        union Content
        {
            struct Bitfield
            {
                unsigned paintScheme : 1; // O:Uniform, 1:Camouflage
                bool mobilityKill : 1; // Characteristics of mobility kill
                unsigned unused_0 : 1;
                unsigned damage : 2; // Damaged appearance of an entity  0 - No damage, 1 - Slight damage, 2 - Moderate damage, 3 - Destroyed
                unsigned smoke : 2; // Status or location of smoke emanating from an entity 0 - Not smoking, 1 - Smoke plume rising from the entity, 2 - Entity is emitting engine smoke, 3 - Entity is emitting engine smoke, and smoke plume is rising from the entity
                unsigned trailing : 2; // Size of the dust cloud trailing effect for the Effects entity 0 - None, 1 - Small, 2 - Medium, 3 - Large
                unsigned unused_1 : 3; // Describes the state of the primary hatch, 0 - Not applicable, 1 - Primary hatch is closed, 2 - Primary hatch is popped, 3 - Primary hatch is popped and a person is visible under hatch, 4 - Primary hatch is open, 5 - Primary hatch is open and person is visible, 6-7 Unused
                bool runningLightsOn : 1; // Describes whether Running Lights are on or off.
                unsigned unused_2 : 2;
                bool flaming : 1; // Describes whether flames are rising from an entity
                unsigned unused_3 : 5;
                bool frozen : 1; //Describes the frozen status of a Land Entity
                bool powerPlantOn : 1; // Describes the power-plant status of platform
                bool deactivated : 1; // Describes the state of a Land Entity
                unsigned unused_4 : 4;
                bool spotLightsOn : 1; // Describes whether Spot Lights are on or off.
                bool interiorLightsOn : 1; // Describes whether Interior Lights are on or off.
                unsigned unused_5 : 2;
            };
            Bitfield fields_;
            uint32_t value_;
        };

        //! @name Operations
        //@{
        template< typename Archive >
        void Serialize( Archive& archive ) const
        {
            archive << appearance_.value_;
        }
        template< typename Archive >
        void Deserialize( Archive& archive )
        {
            archive >> appearance_.value_;
        }
        //@}

        Content appearance_;
    };

}

#endif // __rpr_EntityAppearance_h_
