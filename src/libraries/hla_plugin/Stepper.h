// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Stepper_h
#define plugins_hla_Stepper_h

#include "tools/MessageObserver.h"

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace sword
{
    class ControlEndTick;
    class SimToClient_Content;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  Stepper
    @brief  Stepper
*/
// Created: SLI 2011-06-24
// =============================================================================
class Stepper : private tools::MessageObserver< sword::ControlEndTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             Stepper( xml::xistream& xis, tools::MessageController_ABC< sword::SimToClient_Content >& controller, dispatcher::SimulationPublisher_ABC& publisher );
    virtual ~Stepper();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::ControlEndTick& message, int context );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    //@}
};

}
}

#endif // plugins_hla_Stepper_h
