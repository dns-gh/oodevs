// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_UnitModelReloader_h
#define plugins_hla_UnitModelReloader_h

#include "ResponseObserver_ABC.h"
#include "ClassListener_ABC.h"
#include "tools/Resolver_ABC.h"

namespace sword
{
    class UnitCreation;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace kernel
{
    class AgentType;
}

namespace plugins
{
namespace hla
{
    template< typename ResponseMessage > class ContextHandler_ABC;
    class ContextFactory_ABC;
    class RemoteAgentSubject_ABC;
    class LocalAgentResolver_ABC;

// =============================================================================
/** @class  UnitModelReloader
    @brief  UnitModelReloader
*/
// Created: SLI 2011-09-20
// =============================================================================
class UnitModelReloader : private ResponseObserver_ABC< sword::UnitCreation >,
    private ClassListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitModelReloader( ContextHandler_ABC< sword::UnitCreation >& contextHandler, dispatcher::SimulationPublisher_ABC& publisher,
                     const ContextFactory_ABC& contextFactory, const tools::Resolver_ABC< kernel::AgentType >& agentTypeResolver,
                     RemoteAgentSubject_ABC& remoteSubject, LocalAgentResolver_ABC& localResolver );
    virtual ~UnitModelReloader();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::UnitCreation& message, const std::string& identifier );
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Divested( const std::string& identifier, const T_AttributeIdentifiers& attributes );
    virtual void Acquired( const std::string& identifier, const T_AttributeIdentifiers& attributes );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, unsigned long > T_SimulationIds;
    typedef std::map< std::string, std::string > T_BrainMap;
    //@}

    //! @name Member data
    //@{
    ContextHandler_ABC< sword::UnitCreation >& contextHandler_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    const ContextFactory_ABC& contextFactory_;
    const tools::Resolver_ABC< kernel::AgentType >& agentTypeResolver_;
    RemoteAgentSubject_ABC& remoteSubject_;
    LocalAgentResolver_ABC& localResolver_;
    T_SimulationIds simIds_;
    T_BrainMap brains_;
    //@}
};

}
}

#endif // plugins_hla_UnitModelReloader_h

