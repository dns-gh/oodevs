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
        unsigned short constituentPartNature = 0;
        unsigned short constituentPartPosition = 0;
        short stationNumber = 0;
        unsigned short stationName = static_cast< unsigned short >( constituentPartStationName );
        disId_.Serialize( archive );
        rtiId_.Serialize( archive );
        archive << constituentPartNature
                << constituentPartPosition
                << stationNumber
                << stationName;
        if( constituentPartStationName == OnStationXYZ )
        {
            archive << x
                    << y
                    << z;
        }
        else if( constituentPartStationName == OnStationRangeBearing )
        {
            archive << range
                    << bearing;
        }
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        unsigned short constituentPartNature = 0;
        unsigned short constituentPartPosition = 0;
        short stationNumber = 0;
        unsigned short stationName = 0;
        disId_.Deserialize( archive );
        rtiId_.Deserialize( archive );
        archive >> constituentPartNature
                >> constituentPartPosition
                >> stationNumber
                >> stationName;
        constituentPartStationName = static_cast< ConstituentPartStationNameEnum16 >( stationName );
        if( constituentPartStationName == OnStationXYZ )
        {
            archive >> x
                    >> y
                    >> z;
        }
        else if( constituentPartStationName == OnStationRangeBearing )
        {
            archive >> range
                    >> bearing;
        }
    }
    //@}

public:
    //! @name Member data
    //@{
    rpr::EntityIdentifier disId_;
    Omt13String rtiId_;
    ConstituentPartStationNameEnum16 constituentPartStationName;
    float x, y, z;
    float range, bearing;
    //@}
};

}
}

#endif // plugins_hla_IsPartOfStruct_h
