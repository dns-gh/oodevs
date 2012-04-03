// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentNotifier_h_
#define __AgentNotifier_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
}

namespace dispatcher
{
    class Agent;
}

namespace sword
{
    class UnitAttributes;
    class UnitCreation;
    class UnitOrder;
}

namespace plugins
{
namespace script
{
// =============================================================================
/** @class  AgentNotifier
    @brief  Agent notifier
*/
// Created: AGE 2008-06-13
// =============================================================================
class AgentNotifier : public kernel::Extension_ABC
                    , public kernel::Updatable_ABC< sword::UnitCreation >
                    , public kernel::Updatable_ABC< sword::UnitAttributes >
                    , public kernel::Updatable_ABC< sword::UnitOrder >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentNotifier( kernel::Controller& controller, const dispatcher::Agent& agent );
    virtual ~AgentNotifier();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentNotifier( const AgentNotifier& );            //!< Copy constructor
    AgentNotifier& operator=( const AgentNotifier& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitCreation&   message );
    virtual void DoUpdate( const sword::UnitAttributes& message );
    virtual void DoUpdate( const sword::UnitOrder&               message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const dispatcher::Agent& agent_;
    //@}
};

}
}

#endif // __AgentNotifier_h_
