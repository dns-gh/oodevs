// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteAgentController_h
#define plugins_hla_RemoteAgentController_h

#include "ClassListener_ABC.h"
#include "ObjectListener_ABC.h"
#include "AgentListener_ABC.h"
#include "ResponseObserver_ABC.h"
#include "dispatcher/ExtensionFactory_ABC.h"
#include <tools/Resolver_ABC.h>
#include <boost/shared_ptr.hpp>
#include <map>

namespace dispatcher
{
    class Team_ABC;
    class Logger_ABC;
    class Agent;
}

namespace kernel
{
    class Karma;
}

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace sword
{
    class AutomatCreation;
    class UnitCreation;
}

namespace simulation
{
    class UnitMagicAction;
}

namespace plugins
{
namespace hla
{
    class RemoteAgentSubject_ABC;
    template< typename Response > class ContextHandler_ABC;
    class UnitTypeResolver_ABC;
    class ExtentResolver_ABC;
    class ComponentTypes_ABC;
    class Agent_ABC;
    class AgentSubject_ABC;
    class SideResolver_ABC;

// =============================================================================
/** @class  RemoteAgentController
    @brief  Remote agent controller
*/
// Created: SLI 2011-09-01
// =============================================================================
class RemoteAgentController : private ResponseObserver_ABC< sword::AutomatCreation >
                            , private ClassListener_ABC
                            , private ObjectListener_ABC
                            , private ResponseObserver_ABC< sword::UnitCreation >
                            , private AgentListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAgentController( RemoteAgentSubject_ABC& agentSubject,
                                    ContextHandler_ABC< sword::AutomatCreation >& automatHandler,
                                    ContextHandler_ABC< sword::UnitCreation >& unitHandler,
                                    const SideResolver_ABC& sideResolver,
                                    const UnitTypeResolver_ABC& typeResolver, dispatcher::Logger_ABC& logger,
                                    const ExtentResolver_ABC& extent, AgentSubject_ABC& subject );
    virtual ~RemoteAgentController();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::AutomatCreation& message, const std::string& identifier );
    virtual void Notify( const sword::UnitCreation& message, const std::string& identifier );
    //@}

    //! @name Operations
    //@{
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Divested( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes );
    virtual void Acquired( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes );

    virtual void Moved( const std::string& identifier, double latitude, double longitude );
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
    virtual void NameChanged( const std::string& identifier, const std::string& name );
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type );
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int available,
            unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages );
    virtual void UniqueIdChanged( const std::string& identifier, const T_UniqueId& uniqueId );
    virtual void CallsignChanged( const std::string& identifier, const std::string& callsign );
    virtual void EmbeddedUnitListChanged( const std::string& identifier, const std::vector< T_UniqueId >& units );
    virtual void GeometryChanged( const std::string& identifier, const std::vector< rpr::WorldLocation >& perimeter, ObjectListener_ABC::GeometryType type );
    virtual void ParentChanged( const std::string& rtiIdentifier, const std::string& parentRtiId );
    virtual void SubAgregatesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children );
    virtual void SubEntitiesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children );
    virtual void PropagationChanged( const std::string& rtiIdentifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
            int col, int lig, double xll, double yll, double dx, double dy );

    virtual void AggregateCreated( Agent_ABC& agent, unsigned long identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol, bool isLocal, const std::vector< char >& uniqueId );
    virtual void PlatformCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol, const std::vector< char >& uniqueId ) ;
    //@}

private:
    //! @name Helpers
    //@{
    unsigned long FindAutomat( rpr::ForceIdentifier ) const;
    void Send( simulation::UnitMagicAction& message, const std::string& identifier );
    void Attach( unsigned long simId );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< simulation::UnitMagicAction > T_UnitCreation;
    typedef std::map< std::string, T_UnitCreation > T_UnitCreations;
    typedef std::map< unsigned long, unsigned long > T_Parties;
    typedef std::map< std::string, rpr::ForceIdentifier > T_WaitingAutomats;
    typedef std::map< std::string, HlaObject_ABC* > T_RemoteObjects;
    typedef std::map< std::string, unsigned long > T_Hla2SimIds;
    typedef std::map< unsigned long, Agent_ABC* > T_Agents;
    //@}

private:
    //! @name Member data
    //@{
    RemoteAgentSubject_ABC& agentSubject_;
    ContextHandler_ABC< sword::AutomatCreation >& automatHandler_;
    ContextHandler_ABC< sword::UnitCreation >& unitHandler_;
    const SideResolver_ABC& sideResolver_;
    const UnitTypeResolver_ABC& typeResolver_;
    dispatcher::Logger_ABC& logger_;
    const ExtentResolver_ABC& extent_;
    AgentSubject_ABC& simSubject_;
    T_UnitCreations unitCreations_;
    T_Parties parties_;
    T_WaitingAutomats waitingAutomats_;
    T_RemoteObjects remoteObjects_;
    T_Hla2SimIds remoteIds_;
    T_Agents remoteAgents_;
    //@}
};

}
}

#endif // plugins_hla_RemoteAgentController_h
