// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_PLATFORMPROXY_H_
#define plugins_hla_PLATFORMPROXY_H_

#include "Agent_ABC.h"
#include "EventListenerComposite.h"

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
class PlatformProxy : public Agent_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    PlatformProxy();
    //@}

    //! @name Operations
    //@{
    virtual void Register( EventListener_ABC& listener );
    virtual void Unregister( EventListener_ABC& listener );
    virtual void AddSubordinate( unsigned int id, Agent_ABC& agent );
    virtual void RemoveSubordinate( unsigned int id );
    void SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction );
    //@}

private:
    //! @name Attributes
    //@{
    EventListenerComposite listeners_;
    //@}
};

}
}
#endif // plugins_hla_PLATFORMPROXY_H_
