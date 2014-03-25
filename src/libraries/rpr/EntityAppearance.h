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

#define APPEARANCE_FIELD_ACCESSORS(name) \
    uint8_t name() const; \
    void Set##name(uint8_t)

namespace rpr
{
    struct EntityAppearance_Land
    {
    public:
        //! @name Constructors
        //@{
        EntityAppearance_Land()  { value_ = 0; }
        EntityAppearance_Land(uint32_t v)  { value_ = v; }
        //@}

        //! @name Accessors
        //@{
        APPEARANCE_FIELD_ACCESSORS(PaintScheme);
        APPEARANCE_FIELD_ACCESSORS(MobilityKill);
        APPEARANCE_FIELD_ACCESSORS(FirePowerKill);
        APPEARANCE_FIELD_ACCESSORS(Damage);
        APPEARANCE_FIELD_ACCESSORS(Smoke);
        APPEARANCE_FIELD_ACCESSORS(Trailing);
        APPEARANCE_FIELD_ACCESSORS(Hatch);
        APPEARANCE_FIELD_ACCESSORS(HeadLightsOn);
        APPEARANCE_FIELD_ACCESSORS(TailLightsOn);
        APPEARANCE_FIELD_ACCESSORS(BrakeLightsOn);
        APPEARANCE_FIELD_ACCESSORS(Flaming);
        APPEARANCE_FIELD_ACCESSORS(LauncherRaised);
        APPEARANCE_FIELD_ACCESSORS(CamouflageType);
        APPEARANCE_FIELD_ACCESSORS(Concealed);
        APPEARANCE_FIELD_ACCESSORS(Frozen);
        APPEARANCE_FIELD_ACCESSORS(PowerPlantOn);
        APPEARANCE_FIELD_ACCESSORS(Deactivated);
        APPEARANCE_FIELD_ACCESSORS(TentExtension);
        APPEARANCE_FIELD_ACCESSORS(RampDown);
        APPEARANCE_FIELD_ACCESSORS(BlackoutLightsOn);
        APPEARANCE_FIELD_ACCESSORS(BlackoutBreakLightsOn);
        APPEARANCE_FIELD_ACCESSORS(SpotLightsOn);
        APPEARANCE_FIELD_ACCESSORS(InteriorLightsOn);
        APPEARANCE_FIELD_ACCESSORS(Surrendered);
        APPEARANCE_FIELD_ACCESSORS(Masked);
        //@}

        //! @name Operations
        //@{
        template< typename Archive >
        void Serialize( Archive& archive ) const
        {
            archive << value_;
        }
        template< typename Archive >
        void Deserialize( Archive& archive )
        {
            archive >> value_;
        }
        //@}
        uint32_t value_;
    };

    struct EntityAppearance_Air
    {
    public:
        //! @name Constructors
        //@{
        EntityAppearance_Air()  { value_ = 0; }
        EntityAppearance_Air(uint32_t v)  { value_ = v; }
        //@}

        //! @name Accessors
        //@{
        APPEARANCE_FIELD_ACCESSORS(PaintScheme);
        APPEARANCE_FIELD_ACCESSORS(PropulsionKill);
        APPEARANCE_FIELD_ACCESSORS(Damage);
        APPEARANCE_FIELD_ACCESSORS(Smoke);
        APPEARANCE_FIELD_ACCESSORS(Trailing);
        APPEARANCE_FIELD_ACCESSORS(Canopy);
        APPEARANCE_FIELD_ACCESSORS(LandingLightsOn);
        APPEARANCE_FIELD_ACCESSORS(NavigationLightsOn);
        APPEARANCE_FIELD_ACCESSORS(AntiColisionLightsOn);
        APPEARANCE_FIELD_ACCESSORS(Flaming);
        APPEARANCE_FIELD_ACCESSORS(AfterburnerOn);
        APPEARANCE_FIELD_ACCESSORS(Frozen);
        APPEARANCE_FIELD_ACCESSORS(PowerPlantOn);
        APPEARANCE_FIELD_ACCESSORS(Deactivated);
        APPEARANCE_FIELD_ACCESSORS(FormationLightsOn);
        APPEARANCE_FIELD_ACCESSORS(SpotLightsOn);
        APPEARANCE_FIELD_ACCESSORS(InteriorLightsOn);
        //@}

        //! @name Operations
        //@{
        template< typename Archive >
        void Serialize( Archive& archive ) const
        {
            archive << value_;
        }
        template< typename Archive >
        void Deserialize( Archive& archive )
        {
            archive >> value_;
        }
        //@}

        uint32_t value_;
    };

    struct EntityAppearance_Surface
    {
    public:
        //! @name Constructors
        //@{
        EntityAppearance_Surface()  { value_ = 0; }
        EntityAppearance_Surface(uint32_t v)  { value_ = v; }
        //@}

        //! @name Accessors
        //@{
        APPEARANCE_FIELD_ACCESSORS(PaintScheme);
        APPEARANCE_FIELD_ACCESSORS(MobilityKill);
        APPEARANCE_FIELD_ACCESSORS(Damage);
        APPEARANCE_FIELD_ACCESSORS(Smoke);
        APPEARANCE_FIELD_ACCESSORS(Trailing);
        APPEARANCE_FIELD_ACCESSORS(RunningLightsOn);
        APPEARANCE_FIELD_ACCESSORS(Flaming);
        APPEARANCE_FIELD_ACCESSORS(AfterburnerOn);
        APPEARANCE_FIELD_ACCESSORS(Frozen);
        APPEARANCE_FIELD_ACCESSORS(PowerPlantOn);
        APPEARANCE_FIELD_ACCESSORS(Deactivated);
        APPEARANCE_FIELD_ACCESSORS(SpotLightsOn);
        APPEARANCE_FIELD_ACCESSORS(InteriorLightsOn);
        //@}

        //! @name Operations
        //@{
        template< typename Archive >
        void Serialize( Archive& archive ) const
        {
            archive << value_;
        }
        template< typename Archive >
        void Deserialize( Archive& archive )
        {
            archive >> value_;
        }
        //@}

        uint32_t value_;
    };

}

#undef APPEARANCE_FIELD_ACCESSORS

#endif // __rpr_EntityAppearance_h_
