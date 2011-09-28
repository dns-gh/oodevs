// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AgentProxy_h
#define plugins_hla_AgentProxy_h

#include "Agent_ABC.h"
#include "dispatcher/Observer.h"
#include "protocol/Protocol.h"
#include <vector>

namespace dispatcher
{
    class Agent_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  AgentProxy
    @brief  Agent proxy
*/
// Created: SLI 2011-02-04
// =============================================================================
class AgentProxy : public Agent_ABC
                 , private dispatcher::Observer< sword::UnitAttributes >
                 , private dispatcher::Observer< sword::UnitEnvironmentType >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentProxy( dispatcher::Agent_ABC& agent );
    virtual ~AgentProxy();
    //@}

    //! @name Registration
    //@{
    virtual void Register( EventListener_ABC& listener );
    virtual void Unregister( EventListener_ABC& listener );
    //@}

private:
    //! @name Observer
    //@{
    virtual void Notify( const sword::UnitAttributes& attributes );
    virtual void Notify( const sword::UnitEnvironmentType& message );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< EventListener_ABC* > T_Listeners;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Agent_ABC& agent_;
    T_Listeners listeners_;
    //@}
};

}
}

#endif // plugins_hla_AgentProxy_h
