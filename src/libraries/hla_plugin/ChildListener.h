// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ChildListener_h
#define plugins_hla_ChildListener_h

#include "EventListener_ABC.h"
#include <functional>
#include <set>

namespace plugins
{
namespace hla
{
    class LocalAgentResolver_ABC;

// =============================================================================
/** @class  ChildListener
    @brief  ChildListener
*/
// Created: AHC 2012-10-02
// =============================================================================
struct ChildListener : public EventListener_ABC
{
    //! @name Types
    //@{
    typedef std::function< void( const ChildListener& ) > T_Callback;
    struct LocationStruct
    {
        LocationStruct( double lat, double lon, float alt, float spd, float dir )
            : latitude( lat ), longitude( lon ), altitude( alt ), speed( spd ), direction( dir )
        { }
        LocationStruct( )
            : latitude( 0. ), longitude( 0. ), altitude( 0.f ), speed( 0.f ), direction( 0.f )
        { }
        const LocationStruct& operator+( const LocationStruct& rhs)
        {
            latitude += rhs.latitude;
            longitude += rhs.longitude;
            altitude += rhs.altitude;
            speed += rhs.speed;
            direction += rhs.direction;
            return *this;
        }
        const LocationStruct& operator/( double v )
        {
            float f = (float)v;
            latitude /= v;
            longitude /= v;
            altitude /= f;
            speed /= f;
            direction /= f;
            return *this;
        }
        double latitude;
        double longitude;
        float altitude;
        float speed;
        float direction;
    };
    //@}
    //! @name Contructor
    //@{
    ChildListener( T_Callback callback );
    //@}
    //! @name Accessors
    //@{
    const LocationStruct& GetLocation() const;
    const std::set< unsigned int >& GetPlatforms() const;
    //@}
    //! @name Operations
    //@{
    virtual void SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction );
    virtual void FormationChanged( bool isOnRoad );
    virtual void EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available,
            unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages );
    virtual void EmbarkmentChanged( bool mounted );
    virtual void ChildrenChanged( const EventListener_ABC::T_ChildrenIds& children );
    virtual void ParentChanged( const std::string& parentId );
    virtual void PlatformAdded( const std::string& name, unsigned int id );
    virtual void StateChanged( rpr::DamageState32 state );
    //@}
private:
    T_Callback locationCallback_;
    LocationStruct location_;
    std::set< unsigned int > childPlatforms_;
};

}
}

#endif // plugins_hla_ChildListener_h
