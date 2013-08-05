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
#include "ClassListener_ABC.h"
#include "ObjectListener_ABC.h"
#include "tools/MessageObserver.h"
#include <map>
#pragma warning( push, 0 )
#include <boost/bimap.hpp>
#pragma warning( pop )

namespace dispatcher
{
    class SimulationPublisher_ABC;
    class Logger_ABC;
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
    class LocalAgentResolver_ABC;

// =============================================================================
/** @class  EquipmentUpdater
    @brief  Equipment updater
*/
// Created: SLI 2011-09-29
// =============================================================================
class EquipmentUpdater : private ResponseObserver_ABC< sword::UnitCreation >
                       , private tools::MessageObserver< sword::UnitAttributes >
                       , private ClassListener_ABC
                       , private ObjectListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             EquipmentUpdater( RemoteAgentSubject_ABC& subject, ContextHandler_ABC< sword::UnitCreation >& handler,
                               dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& factory,
                               const rpr::EntityTypeResolver_ABC& resolver, const ComponentTypes_ABC& componentTypes,
                               tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                               dispatcher::Logger_ABC& logger, const LocalAgentResolver_ABC& localAgentResolver );
    virtual ~EquipmentUpdater();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::UnitCreation& message, const std::string& identifier );
    virtual void Notify( const sword::UnitAttributes& message, int context );
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
    //@}

private:
    //! @name Helpers
    //@{
    void SendUpdate( const std::string& identifier );
    //@}

public:
    //! @name Types
    //@{
    struct T_Component
    {
        T_Component();
        T_Component( unsigned int avail, unsigned int dead, unsigned int light, unsigned int heavy);
        bool operator == (const T_Component& rhs );
        unsigned int available_;
        unsigned int dead_;
        unsigned int lightDamages_;
        unsigned int heavyDamages_;
    };
    typedef std::map< std::string, T_Component > T_Components;
    typedef std::map< std::string, T_Components > T_Agents;
    typedef std::pair< unsigned int, unsigned int > T_StaticComponent;
    typedef std::map< std::string, T_StaticComponent > T_StaticComponents;
    typedef std::map< std::string, T_StaticComponents > T_AgentsTypes;
    typedef boost::bimap< std::string, unsigned int > T_Identifiers;
    typedef std::map< std::string, HlaObject_ABC* > T_HLAObjects;
    //@}

private:
    //! @name Member data
    //@{
    RemoteAgentSubject_ABC& subject_;
    ContextHandler_ABC< sword::UnitCreation >& handler_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    const ContextFactory_ABC& factory_;
    const rpr::EntityTypeResolver_ABC& resolver_;
    const ComponentTypes_ABC& componentTypes_;
    dispatcher::Logger_ABC& logger_;
    const LocalAgentResolver_ABC& localAgentResolver_;
    T_Identifiers identifiers_;
    T_Agents remoteAgents_;
    T_AgentsTypes agentTypes_;
    T_HLAObjects hlaObjects_;
    //@}
};

}
}

#endif // plugins_hla_EquipmentUpdater_h
