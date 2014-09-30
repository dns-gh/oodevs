// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_IsPartOfStruct_h
#define plugins_hla_IsPartOfStruct_h

#include "Omt13String.h"
#include "rpr/Coordinates.h"
#include "rpr/EntityIdentifier.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  IsPartOfStruct
    @brief  IsPartOfStruct
*/
// Created: AHC 2012-10-02
// =============================================================================
class IsPartOfStruct
{
public:
    enum ConstituentPartNatureEnum16
    {
        constituentPartNatureEnum16_Other     = 0,
        constituentPartNatureEnum16_HostFireableMunition     = 1,
        constituentPartNatureEnum16_MunitionCarriedAsCargo     = 2,
        constituentPartNatureEnum16_FuelCarriedAsCargo     = 3,
        constituentPartNatureEnum16_GunmountAttachedToHost     = 4,
        constituentPartNatureEnum16_ComputerGeneratedForcesCarriedAsCargo     = 5,
        constituentPartNatureEnum16_VehicleCarriedAsCargo     = 6,
        constituentPartNatureEnum16_EmitterMountedOnHost     = 7,
        constituentPartNatureEnum16_MobileCommandAndControlEntityCarriedAboardHost     = 8,
        constituentPartNatureEnum16_EntityStationedWithRespectToHost     = 9,
        constituentPartNatureEnum16_TeamMemberInFormationWith     = 10
    };
    enum ConstituentPartPositionEnum16
    {
        constituentPartPosition_Other     = 0,
        constituentPartPosition_OnTopOf     = 1,
        constituentPartPosition_Inside     = 2
    };
    enum ConstituentPartStationNameEnum16
    {
        Other   = 0,
        AircraftWingstation     = 1,
        ShipsForwardGunmountStarboard  = 2,
        ShipsForwardGunmountPort    = 3,
        ShipsForwardGunmountCenterline  = 4,
        ShipsAftGunmountStarboard   = 5,
        ShipsAftGunmountPort    = 6,
        ShipsAftGunmountCenterline  = 7,
        ForwardTorpedoTube  = 8,
        AftTorpedoTube  = 9,
        BombBay     = 10,
        CargoBay    = 11,
        TruckBed    = 12,
        TrailerBed  = 13,
        WellDeck    = 14,
        OnStationRangeBearing   = 15,
        OnStationXYZ    = 16
    };

    //! @name Constructors/Destructor
    //@{
             IsPartOfStruct();
             IsPartOfStruct( const rpr::EntityIdentifier& disId, const Omt13String& rtiId );
    virtual ~IsPartOfStruct();
    //@}
    
    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        const uint8_t padding = 0;
        unsigned short stationName = static_cast< unsigned short >( constituentPartStationName );
        disId_.Serialize( archive );
        rtiId_.Serialize( archive );
        if( rtiId_.str().size() % 2 == 0)            
            archive << padding;
        archive << constituentPartNature_
                << constituentPartPosition_
                << stationNumber_
                << padding << padding
                << stationName;
        if( constituentPartStationName == OnStationXYZ )
        {
            archive << padding << padding
                    << x
                    << y
                    << z;
        }
        else if( constituentPartStationName == OnStationRangeBearing )
        {
            archive << padding << padding
                    << range
                    << bearing;
        }
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint8_t padding = 0;
        unsigned short stationName = 0;
        disId_.Deserialize( archive );
        rtiId_.Deserialize( archive );
        if( rtiId_.str().size() % 2 == 0)            
            archive >> padding;
        archive >> constituentPartNature_
                >> constituentPartPosition_
                >> stationNumber_
                >> padding >> padding
                >> stationName;
        constituentPartStationName = static_cast< ConstituentPartStationNameEnum16 >( stationName );
        if( constituentPartStationName == OnStationXYZ )
        {
            archive >> padding >> padding
                    >> x
                    >> y
                    >> z;
        }
        else if( constituentPartStationName == OnStationRangeBearing )
        {
            archive >> padding >> padding
                    >> range
                    >> bearing;
        }
    }
    //@}

public:
    //! @name Member data
    //@{
    rpr::EntityIdentifier disId_;
    Omt13String rtiId_;
    unsigned short constituentPartNature_;
    unsigned short constituentPartPosition_;
    short stationNumber_;
    ConstituentPartStationNameEnum16 constituentPartStationName;
    float x, y, z;
    float range, bearing;
    //@}
};

}
}

#endif // plugins_hla_IsPartOfStruct_h
