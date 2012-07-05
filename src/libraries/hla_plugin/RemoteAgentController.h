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
#include "ResponseObserver_ABC.h"
#include "tools/Resolver_ABC.h"
#include <boost/shared_ptr.hpp>
#include <map>

namespace dispatcher
{
    class Team_ABC;
    class Logger_ABC;
}

namespace kernel
{
    class Karma;
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

// =============================================================================
/** @class  RemoteAgentController
    @brief  Remote agent controller
*/
// Created: SLI 2011-09-01
// =============================================================================
class RemoteAgentController : private ResponseObserver_ABC< sword::AutomatCreation >
                            , private ClassListener_ABC
                            , private ObjectListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAgentController( RemoteAgentSubject_ABC& agentSubject,
                                    ContextHandler_ABC< sword::AutomatCreation >& automatHandler,
                                    ContextHandler_ABC< sword::UnitCreation >& unitHandler,
                                    const tools::Resolver_ABC< dispatcher::Team_ABC >& sides,
                                    const UnitTypeResolver_ABC& typeResolver, dispatcher::Logger_ABC& logger,
                                    const ExtentResolver_ABC& extent );
    virtual ~RemoteAgentController();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::AutomatCreation& message, const std::string& identifier );
    //@}

    //! @name Operations
    //@{
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Moved( const std::string& identifier, double latitude, double longitude );
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
    virtual void NameChanged( const std::string& identifier, const std::string& name );
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type );
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int number );
    virtual void UniqueIdChanged( const std::string& identifier, const std::string& uniqueId );
    virtual void CallsignChanged( const std::string& identifier, const std::string& callsign );
    virtual void EmbeddedUnitListChanged( const std::string& identifier, const std::vector< std::string >& units );
    //@}

private:
    //! @name Helpers
    //@{
    unsigned long FindAutomat( rpr::ForceIdentifier ) const;
    void Send( simulation::UnitMagicAction& message, const std::string& identifier );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< simulation::UnitMagicAction > T_UnitCreation;
    typedef std::map< std::string, T_UnitCreation > T_UnitCreations;
    typedef std::map< unsigned long, unsigned long > T_Parties;
    typedef std::map< kernel::Karma, unsigned long > T_Karmas;
    typedef std::map< std::string, rpr::ForceIdentifier > T_WaitingAutomats;
    //@}

private:
    //! @name Member data
    //@{
    RemoteAgentSubject_ABC& agentSubject_;
    ContextHandler_ABC< sword::AutomatCreation >& automatHandler_;
    ContextHandler_ABC< sword::UnitCreation >& unitHandler_;
    const tools::Resolver_ABC< dispatcher::Team_ABC >& sides_;
    const UnitTypeResolver_ABC& typeResolver_;
    dispatcher::Logger_ABC& logger_;
    const ExtentResolver_ABC& extent_;
    T_UnitCreations unitCreations_;
    T_Parties parties_;
    T_Karmas karmas_;
    T_WaitingAutomats waitingAutomats_;
    //@}
};

}
}

#endif // plugins_hla_RemoteAgentController_h
