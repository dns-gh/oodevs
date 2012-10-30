// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_UnitTeleporter_h
#define plugins_hla_UnitTeleporter_h

#include "CLassListener_ABC.h"
#include "ObjectListener_ABC.h"
#include "ResponseObserver_ABC.h"
#include <map>

namespace sword
{
    class UnitCreation;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
    class Logger_ABC;
}

namespace plugins
{
namespace hla
{
    class RemoteAgentSubject_ABC;
    class ContextFactory_ABC;
    class LocalAgentResolver_ABC;
    class CallsignResolver_ABC;
    template< typename ResponseMessage > class ContextHandler_ABC;

// =============================================================================
/** @class  UnitTeleporter
    @brief  Unit teleporter
*/
// Created: SLI 2011-09-13
// =============================================================================
class UnitTeleporter : private ResponseObserver_ABC< sword::UnitCreation >
                     , private ClassListener_ABC
                     , private ObjectListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitTeleporter( RemoteAgentSubject_ABC& agentSubject, ContextHandler_ABC< sword::UnitCreation >& contextHandler,
                             dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& contextFactory,
                             const LocalAgentResolver_ABC& localResolver, const CallsignResolver_ABC& callsignResolver, dispatcher::Logger_ABC& logger );
    virtual ~UnitTeleporter();
    //@}

private:
    //! @name Operations
    //@{
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Divested( const std::string& identifier );
    virtual void Acquired( const std::string& identifier );
    virtual void Moved( const std::string& identifier, double latitude, double longitude );
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
    virtual void NameChanged( const std::string& identifier, const std::string& name );
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type );
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int number );
    virtual void UniqueIdChanged( const std::string& identifier, const T_UniqueId& uniqueId );
    virtual void CallsignChanged( const std::string& identifier, const std::string& callsign );
    virtual void EmbeddedUnitListChanged( const std::string& identifier, const std::vector< T_UniqueId >& units );
    virtual void PerimeterChanged( const std::string& identifier, const std::vector< rpr::PerimeterPoint >& perimeter );
    virtual void ParentChanged( const std::string& rtiIdentifier, const std::string& parentRtiId );
    virtual void SubAgregatesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children );
    virtual void SubEntitiesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children );
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const sword::UnitCreation& message, const std::string& identifier );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, unsigned long > T_Identifiers;
    typedef std::map< std::string, HlaObject_ABC* > T_Objects;
    typedef std::map< std::string, std::vector< T_UniqueId > > T_EmbeddedUnitsMap;
    //@}

private:
    //! @name Member data
    //@{
    RemoteAgentSubject_ABC& agentSubject_;
    ContextHandler_ABC< sword::UnitCreation >& contextHandler_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    const ContextFactory_ABC& contextFactory_;
    const LocalAgentResolver_ABC& localResolver_;
    const CallsignResolver_ABC& callsignResolver_;
    dispatcher::Logger_ABC& logger_;
    T_Identifiers identifiers_;
    T_Objects objects_;
    T_EmbeddedUnitsMap pendingLoaded_;
    //@}
};

}
}

#endif // plugins_hla_UnitTeleporter_h
