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
#include "dispatcher/Observer.h"
#include <tools/SortedInterfaceContainer.h>

namespace dispatcher
{
    class Object_ABC;
}

namespace sword
{
    class ObjectUpdate;
}

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace plugins
{
namespace hla
{
class SimulationTimeManager_ABC;

/// =============================================================================
/// @class TacticalObjectProxy
/// @brief TacticalObjectProxy
/// @thread This type is not thread safe
///
/// Created: ahc 8 août 2012
/// =============================================================================
class TacticalObjectProxy
    : public TacticalObject_ABC
    , private dispatcher::Observer< sword::ObjectUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
    TacticalObjectProxy( dispatcher::Object_ABC& object, const rpr::EntityTypeResolver_ABC& dotationResolver, const SimulationTimeManager_ABC& timeManager );
    virtual ~TacticalObjectProxy();
    //@}

    //! @name Operations
    //@{
    virtual void Register( TacticalObjectEventListener_ABC& listener );
    virtual void Unregister( TacticalObjectEventListener_ABC& listener );
    //@}

private:
    virtual void Notify( const sword::ObjectUpdate& message );

private:
    //! @name Attributes
    //@{
    dispatcher::Object_ABC& object_;
    const rpr::EntityTypeResolver_ABC& dotationResolver_;
    const SimulationTimeManager_ABC& timeManager_;
    tools::SortedInterfaceContainer< TacticalObjectEventListener_ABC > listeners_;
    //@}
};

}
}
#endif // plugins_hla_TACTICALOBJECTPROXY_H_
