// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TransportationController_h
#define plugins_hla_TransportationController_h

#include "tools/MessageObserver.h"

namespace xml
{
    class xisubstream;
}

namespace tools
{
    template< typename Category > class MessageController_ABC;
}

namespace sword
{
    class SimToClient_Content;
    class AutomatOrder;
}

namespace plugins
{
namespace hla
{
    class MissionResolver_ABC;
    class TransportationListener_ABC;

// =============================================================================
/** @class  TransportationController
    @brief  Transportation controller
*/
// Created: SLI 2011-10-06
// =============================================================================
class TransportationController : private tools::MessageObserver< sword::AutomatOrder >
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportationController( xml::xisubstream xis, const MissionResolver_ABC& resolver,
                                       tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                       TransportationListener_ABC& listener );
    virtual ~TransportationController();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify(  const sword::AutomatOrder& message, int context );
    //@}

private:
    //! @name Member data
    //@{
    const unsigned int transportIdentifier_;
    TransportationListener_ABC& listener_;
    //@}
};

}
}

#endif // plugins_hla_TransportationController_h
