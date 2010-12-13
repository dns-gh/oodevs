// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __AutomatNotifier_h_
#define __AutomatNotifier_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace sword
{
    class AutomatOrder;
}

namespace kernel
{
    class Controller;
}

namespace dispatcher
{
    class Automat;
}

namespace plugins
{
namespace script
{
// =============================================================================
/** @class  AutomatNotifier
    @brief  Automat notifier
*/
// Created: SBO 2008-08-13
// =============================================================================
class AutomatNotifier : public kernel::Extension_ABC
                      , public kernel::Updatable_ABC< sword::AutomatOrder >
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatNotifier( kernel::Controller& controller, const dispatcher::Automat& automat );
    virtual ~AutomatNotifier();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatNotifier( const AutomatNotifier& );            //!< Copy constructor
    AutomatNotifier& operator=( const AutomatNotifier& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::AutomatOrder& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const dispatcher::Automat& automat_;
    //@}
};

}
}

#endif // __AutomatNotifier_h_
