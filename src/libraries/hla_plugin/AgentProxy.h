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
#include "EventListenerComposite.h"
#include "dispatcher/Observer.h"
#include <vector>

namespace dispatcher
{
    class Agent_ABC;
}

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace sword
{
    class UnitAttributes;
    class UnitEnvironmentType;
}

namespace plugins
{
namespace hla
{
    class ComponentTypes_ABC;
    class LocalAgentResolver_ABC;

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
             AgentProxy( dispatcher::Agent_ABC& agent, const ComponentTypes_ABC& componentTypes, const rpr::EntityTypeResolver_ABC& componentTypeResolver,
                     const LocalAgentResolver_ABC& localAgentResolver, bool doDisaggregation );
    virtual ~AgentProxy();
    //@}

    //! @name Registration
    //@{
    virtual void Register( EventListener_ABC& listener );
    virtual void Unregister( EventListener_ABC& listener );
    virtual void AddSubordinate( unsigned int id, Agent_ABC& agent );
    virtual void RemoveSubordinate( unsigned int id );
    //@}

    void PlatformAdded( const std::string& name, unsigned int id );

private:
    //! @name Observer
    //@{
    virtual void Notify( const sword::UnitAttributes& attributes );
    virtual void Notify( const sword::UnitEnvironmentType& message );
    //@}

private:
    //! @name Member data
    //@{
    bool doDisaggregation_;
    const dispatcher::Agent_ABC& agent_;
    const ComponentTypes_ABC& componentTypes_;
    const rpr::EntityTypeResolver_ABC& componentTypeResolver_;
    const LocalAgentResolver_ABC& localAgentResolver_;
    EventListenerComposite listeners_;
    //@}
};

}
}

#endif // plugins_hla_AgentProxy_h
