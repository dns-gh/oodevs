// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TacticalObjectEventListener_ABC_h
#define plugins_hla_TacticalObjectEventListener_ABC_h

#include <boost/noncopyable.hpp>
#include <geometry/Types.h>

namespace sword
{
    class CoordLatLong;
}

namespace rpr
{
    class EntityType;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  TacticalObjectEventListener_ABC
    @brief  TacticalObjectEventListener_ABC
*/
// Created: AHC 201-08-10
// =============================================================================
struct TacticalObjectEventListener_ABC
{
    virtual ~TacticalObjectEventListener_ABC() {}
};

class ObjectLocationEventListener_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< sword::CoordLatLong > T_PositionVector;
    typedef std::vector< rpr::EntityType > T_ResourceVector;
    //@}

    //! @name Constructors/Destructor
    //@{
    ObjectLocationEventListener_ABC() {}
    virtual ~ObjectLocationEventListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SpatialChanged( const T_PositionVector& pos )  = 0;
    virtual void ResourcesChanged( const T_ResourceVector& pos )  = 0;
    //@}
};

class ObjectPropagationEventListener_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    struct ConcentrationData
    {
        ConcentrationData( double lat, double lon, double c ) : latitude( lat ), longitude( lon ), concentration( c ) {}
        double latitude;
        double longitude;
        double concentration;
    };
    struct Data
    {
        Data( int r, int c, const geometry::Rectangle2d& ex, double maxV ) :  cols( c ), rows( r ), extent( ex ), maxValue( maxV ) {}
        int cols;
        int rows;
        geometry::Rectangle2d extent;
        double maxValue;
        std::vector< float > concentrations;
    };
    typedef std::vector< Data > T_DataVector;
    //@}

    //! @name Constructors/Destructor
    //@{
    ObjectPropagationEventListener_ABC() {}
    virtual ~ObjectPropagationEventListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void PropagationChanged( const T_DataVector& pos )  = 0;
    //@}
};

}
}

#endif // plugins_hla_TacticalObjectEventListener_ABC_h
