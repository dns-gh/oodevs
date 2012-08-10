// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TACTICALOBJECTPROXY_H_
#define plugins_hla_TACTICALOBJECTPROXY_H_

#include "TacticalObject_ABC.h"
#include "EventListenerComposite.h"

namespace dispatcher
{
    class Object_ABC;
}

namespace plugins
{
namespace hla
{

/// =============================================================================
/// @class TacticalObjectProxy
/// @brief TacticalObjectProxy
/// @thread This type is not thread safe
///
/// Created: ahc 8 août 2012
/// =============================================================================
class TacticalObjectProxy : public TacticalObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    TacticalObjectProxy( dispatcher::Object_ABC& object );
    virtual ~TacticalObjectProxy();
    //@}

    //! @name Operations
    //@{
    virtual void Register( EventListener_ABC& listener );
    virtual void Unregister( EventListener_ABC& listener );
    //@}

private:
    //! @name Attributes
    //@{
    dispatcher::Object_ABC& object_;
    EventListenerComposite listeners_;
    //@}
};

}
}
#endif // plugins_hla_TACTICALOBJECTPROXY_H_
