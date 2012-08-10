// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TACTICALOBJECTCONTROLLER_H_
#define plugins_hla_TACTICALOBJECTCONTROLLER_H_

#include "TacticalObjectSubject_ABC.h"

#include "dispatcher/ExtensionFactory_ABC.h"

#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>

namespace dispatcher
{
    class Model_ABC;
    class Object_ABC;
    class Object;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace plugins
{
namespace hla
{
    class TacticalObjectListener_ABC;
    class TacticalObjectProxy;

/// =============================================================================
/// @class TacticalObjectController
/// @brief TacticalObjectController
/// @thread This type is not thread safe
///
/// Created: ahc 8 ao�t 2012
/// =============================================================================
class TacticalObjectController : public TacticalObjectSubject_ABC
    , private dispatcher::ExtensionFactory_ABC< dispatcher::Object >
{
public:
    //! @name Constructors/Destructor
    //@{
    TacticalObjectController( dispatcher::Model_ABC& model, const kernel::CoordinateConverter_ABC& converter );
    virtual ~TacticalObjectController();
    //@}

    //! @name Operations
    //@{
    virtual void Register( TacticalObjectListener_ABC& listener );
    virtual void Unregister( TacticalObjectListener_ABC& listener );
    void Visit( dispatcher::Model_ABC& model );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< TacticalObjectListener_ABC* >        T_Listeners;
    typedef T_Listeners::const_iterator     CIT_Listeners;
    typedef boost::shared_ptr< TacticalObjectProxy > T_Object;
    typedef std::map< unsigned long, T_Object >      T_Objects;
    //@}

    //! @name Operations
    //@{
    virtual void Create( dispatcher::Object& object );
    void CreateObject( dispatcher::Object_ABC& object );
    //@}

    //! @name Types
    //@{
    dispatcher::Model_ABC& model_;
    const kernel::CoordinateConverter_ABC& converter_;
    T_Listeners listeners_;
    T_Objects objects_;
    //@}
};

}
}
#endif // TACTICALOBJECTCONTROLLER_H_
