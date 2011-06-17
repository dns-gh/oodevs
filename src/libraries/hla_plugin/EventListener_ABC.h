// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_EventListener_ABC_h
#define plugins_hla_EventListener_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  EventListener_ABC
    @brief  Event listener definition
*/
// Created: SLI 2011-02-07
// =============================================================================
class EventListener_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             EventListener_ABC() {}
    virtual ~EventListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction ) = 0;
    virtual void FormationChanged( bool isOnRoad ) = 0;
    virtual void EquipmentChanged( unsigned int type, unsigned int available ) = 0;
    virtual void EmbarkmentChanged( bool mounted ) = 0;
    //@}
};

}
}

#endif // plugins_hla_EventListener_ABC_h
