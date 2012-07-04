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

#include "RemoteAgentListener_ABC.h"
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
    class CallsignResolver_ABC;
    template< typename ResponseMessage > class ContextHandler_ABC;

// =============================================================================
/** @class  UnitTeleporter
    @brief  Unit teleporter
*/
// Created: SLI 2011-09-13
// =============================================================================
class UnitTeleporter : private RemoteAgentListener_ABC
                     , private ResponseObserver_ABC< sword::UnitCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitTeleporter( RemoteAgentSubject_ABC& agentSubject, ContextHandler_ABC< sword::UnitCreation >& contextHandler,
                             dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& contextFactory,
                             const CallsignResolver_ABC& callsignResolver, dispatcher::Logger_ABC& logger );
    virtual ~UnitTeleporter();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Created( const std::string& identifier );
    virtual void Destroyed( const std::string& identifier );
    virtual void Moved( const std::string& identifier, double latitude, double longitude );
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
    virtual void NameChanged( const std::string& identifier, const std::string& name );
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type );
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int number );
    virtual void UniqueIdChanged( const std::string& identifier, const std::string& uniqueId );
    virtual void CallsignChanged( const std::string& identifier, const std::string& callsign );
    virtual void EmbeddedUnitListChanged( const std::string& identifier, const std::vector< std::string >& units );
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const sword::UnitCreation& message, const std::string& identifier );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, unsigned long > T_Identifiers;
    typedef std::map< std::string, std::vector< std::string > > T_EmbeddedUnitsMap;
    //@}

private:
    //! @name Member data
    //@{
    RemoteAgentSubject_ABC& agentSubject_;
    ContextHandler_ABC< sword::UnitCreation >& contextHandler_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    const ContextFactory_ABC& contextFactory_;
    const CallsignResolver_ABC& callsignResolver_;
    dispatcher::Logger_ABC& logger_;
    T_Identifiers identifiers_;
    T_EmbeddedUnitsMap pendingLoaded_;
    //@}
};

}
}

#endif // plugins_hla_UnitTeleporter_h
