// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_EVENTLISTENERCOMPOSITE_H_
#define plugins_hla_EVENTLISTENERCOMPOSITE_H_

#include "EventListener_ABC.h"
#include <set>

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  PlatformProxy
    @brief  PlatformProxy
*/
// Created: AHC 2012-07-27
// =============================================================================
class EventListenerComposite : public EventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    EventListenerComposite();
    virtual ~EventListenerComposite();
    //@}

    //! @name Registration
    //@{
    virtual void Register( EventListener_ABC& listener );
    virtual void Unregister( EventListener_ABC& listener );
    //@}

    //! @name Operations
    //@{
    virtual void SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction );
    virtual void FormationChanged( bool isOnRoad );
    virtual void EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available );
    virtual void EmbarkmentChanged( bool mounted );
    virtual void PlatformAdded( const std::string& name, unsigned int id );
    //@}

private:
    typedef std::set< EventListener_ABC* > T_Listeners;

private:
    T_Listeners listeners_;
};

}
}
#endif // plugins_hla_EVENTLISTENERCOMPOSITE_H_
