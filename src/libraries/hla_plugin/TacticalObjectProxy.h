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
#include "TacticalObjectEventListenerComposite.h"

namespace dispatcher
{
    class Object_ABC;
}

namespace rpr
{
    class EntityTypeResolver_ABC;
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
/// Created: ahc 8 ao�t 2012
/// =============================================================================
class TacticalObjectProxy : public TacticalObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    TacticalObjectProxy( dispatcher::Object_ABC& object, const rpr::EntityTypeResolver_ABC& dotationResolver );
    virtual ~TacticalObjectProxy();
    //@}

    //! @name Operations
    //@{
    virtual void Register( TacticalObjectEventListener_ABC& listener );
    virtual void Unregister( TacticalObjectEventListener_ABC& listener );
    //@}

private:
    //! @name Attributes
    //@{
    dispatcher::Object_ABC& object_;
    const rpr::EntityTypeResolver_ABC& dotationResolver_;
    TacticalObjectEventListenerComposite listeners_;
    //@}
};

}
}
#endif // plugins_hla_TACTICALOBJECTPROXY_H_
