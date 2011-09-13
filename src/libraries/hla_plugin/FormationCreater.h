// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_FormationCreater_h
#define plugins_hla_FormationCreater_h

#include "tools/MessageObserver.h"
#include "tools/Resolver_ABC.h"
#include <vector>

namespace tools
{
    template< typename Category > class MessageController_ABC;
}

namespace dispatcher
{
    class Team_ABC;
}

namespace sword
{
    class ControlEndTick;
    class FormationCreation;
    class SimToClient_Content;
}

namespace plugins
{
namespace hla
{
    template< typename ResponseMessage > class ContextHandler_ABC;

// =============================================================================
/** @class  FormationCreater
    @brief  Formation creater
*/
// Created: SLI 2011-09-13
// =============================================================================
class FormationCreater : private tools::MessageObserver< sword::ControlEndTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             FormationCreater( tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                               const tools::Resolver_ABC< dispatcher::Team_ABC >& sides,
                               ContextHandler_ABC< sword::FormationCreation >& contextHandler );
    virtual ~FormationCreater();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::ControlEndTick& message, int context );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< unsigned long > T_Sides;
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageController_ABC< sword::SimToClient_Content >& messageController_;
    ContextHandler_ABC< sword::FormationCreation >& contextHandler_;
    T_Sides sides_;
    //@}
};

}
}

#endif // FormationCreater_h
