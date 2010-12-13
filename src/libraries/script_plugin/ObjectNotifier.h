// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ObjectNotifier_h_
#define __ObjectNotifier_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace sword
{
    class ObjectDestruction;
}

namespace kernel
{
    class Controller;
}

namespace dispatcher
{
    class Object_ABC;
}

namespace plugins
{
namespace script
{
// =============================================================================
/** @class  ObjectNotifier
    @brief  Object notifier
*/
// Created: SBO 2010-07-13
// =============================================================================
class ObjectNotifier : public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< sword::ObjectDestruction >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectNotifier( kernel::Controller& controller, const dispatcher::Object_ABC& entity );
    virtual ~ObjectNotifier();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectNotifier( const ObjectNotifier& );            //!< Copy constructor
    ObjectNotifier& operator=( const ObjectNotifier& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectDestruction& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const dispatcher::Object_ABC& entity_;
    //@}
};

}
}

#endif // __ObjectNotifier_h_
