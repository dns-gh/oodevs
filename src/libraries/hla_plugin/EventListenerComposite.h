// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_EventListenerComposite_h
#define plugins_hla_EventListenerComposite_h

#include "EventListener_ABC.h"
#include <set>

namespace plugins
{
namespace hla
{

class EventListenerComposite : public EventListener_ABC
{
public:
    EventListenerComposite();
    virtual ~EventListenerComposite();

    //! @name Operations
    //@{
    void Register( EventListener_ABC& listener );
    void Unregister( EventListener_ABC& listener );

    virtual void SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction );
    virtual void FormationChanged( bool isOnRoad );
    virtual void EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available,
            unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages );
    virtual void EmbarkmentChanged( bool mounted );
    virtual void PlatformAdded( const std::string& name, unsigned int id );
    virtual void ChildrenChanged( const T_ChildrenIds& children );
    virtual void ParentChanged( const std::string& parentId );
    virtual void StateChanged( rpr::DamageState32 state );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< EventListener_ABC* > T_Listeners;
    //@}

private:
    T_Listeners listeners_;
};

}
}

#endif // plugins_hla_EventListenerComposite_h
