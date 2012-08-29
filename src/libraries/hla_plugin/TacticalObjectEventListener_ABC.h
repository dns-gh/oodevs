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
class TacticalObjectEventListener_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< sword::CoordLatLong > T_PositionVector;
    //@}

    //! @name Constructors/Destructor
    //@{
            TacticalObjectEventListener_ABC() {}
    virtual ~TacticalObjectEventListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SpatialChanged( const T_PositionVector& pos )  = 0;
    //@}
};

}
}

#endif // plugins_hla_TacticalObjectEventListener_ABC_h
