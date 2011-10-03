// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_EquipmentUpdater_h
#define plugins_hla_EquipmentUpdater_h

#include "ResponseObserver_ABC.h"
#include "RemoteAgentListener_ABC.h"
#include "tools/MessageObserver.h"
#include <map>
#pragma warning( push, 0 )
#include <boost/bimap.hpp>
#pragma warning( pop )

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace tools
{
    template< typename Category > class MessageController_ABC;
}

namespace sword
{
    class UnitCreation;
    class UnitAttributes;
    class SimToClient_Content;
}

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace plugins
{
namespace hla
{
    class RemoteAgentSubject_ABC;
    template< typename ResponseMessage > class ContextHandler_ABC;
    class ContextFactory_ABC;
    class ComponentTypes_ABC;

// =============================================================================
/** @class  EquipmentUpdater
    @brief  Equipment updater
*/
// Created: SLI 2011-09-29
// =============================================================================
class EquipmentUpdater : private RemoteAgentListener_ABC
                       , private ResponseObserver_ABC< sword::UnitCreation >
                       , private tools::MessageObserver< sword::UnitAttributes >
{
public:
    //! @name Constructors/Destructor
    //@{
             EquipmentUpdater( RemoteAgentSubject_ABC& subject, ContextHandler_ABC< sword::UnitCreation >& handler,
                               dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& factory,
                               const rpr::EntityTypeResolver_ABC& resolver, const ComponentTypes_ABC& componentTypes,
                               tools::MessageController_ABC< sword::SimToClient_Content >& messageController );
    virtual ~EquipmentUpdater();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::UnitCreation& message, const std::string& identifier );
    virtual void Notify( const sword::UnitAttributes& message, int context );
    virtual void Created( const std::string& identifier );
    virtual void Destroyed( const std::string& identifier );
    virtual void Moved( const std::string& identifier, double latitude, double longitude );
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
    virtual void NameChanged( const std::string& identifier, const std::string& name );
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type );
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int number );
    //@}

private:
    //! @name Helpers
    //@{
    void SendUpdate( const std::string& identifier );
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< unsigned int, unsigned int > T_Component;
    typedef std::map< std::string, T_Component > T_Components;
    typedef std::map< std::string, T_Components > T_Agents;
    typedef boost::bimap< std::string, unsigned int > T_Identifiers;
    //@}

private:
    //! @name Member data
    //@{
    RemoteAgentSubject_ABC& subject_;
    ContextHandler_ABC< sword::UnitCreation >& handler_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    const ContextFactory_ABC& factory_;
    const rpr::EntityTypeResolver_ABC& resolver_;
    T_Identifiers identifiers_;
    T_Agents remoteAgents_;
    T_Agents agentTypes_;
    const ComponentTypes_ABC& componentTypes_;
    //@}
};

}
}

#endif // plugins_hla_EquipmentUpdater_h
