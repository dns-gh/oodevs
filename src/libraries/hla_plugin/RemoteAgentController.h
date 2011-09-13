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

#include "tools/MessageObserver.h"
#include "RemoteAgentListener_ABC.h"
#include "ResponseObserver_ABC.h"
#include "tools/Resolver_ABC.h"
#include <boost/shared_ptr.hpp>
#include <map>

namespace dispatcher
{
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace kernel
{
    class AutomatType;
}

namespace sword
{
    class ControlEndTick;
    class AutomatCreation;
    class FormationCreation;
    class UnitCreation;
    class SimToClient_Content;
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
    class ContextFactory_ABC;
    template< typename Response > class ContextHandler_ABC;

// =============================================================================
/** @class  RemoteAgentController
    @brief  Remote agent controller
*/
// Created: SLI 2011-09-01
// =============================================================================
class RemoteAgentController : private tools::MessageObserver< sword::ControlEndTick >
                            , private ResponseObserver_ABC< sword::FormationCreation >
                            , private ResponseObserver_ABC< sword::AutomatCreation >
                            , private RemoteAgentListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAgentController( tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                    dispatcher::Model_ABC& model, tools::Resolver_ABC< kernel::AutomatType >& automatTypes,
                                    dispatcher::SimulationPublisher_ABC& publisher, RemoteAgentSubject_ABC& agentSubject,
                                    const ContextFactory_ABC& contextFactory,
                                    ContextHandler_ABC< sword::FormationCreation >& formationHandler,
                                    ContextHandler_ABC< sword::AutomatCreation >& automatHandler,
                                    ContextHandler_ABC< sword::UnitCreation >& unitHandler );
    virtual ~RemoteAgentController();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::ControlEndTick& message, int context );
    virtual void Notify( const sword::FormationCreation& message, const std::string& identifier );
    virtual void Notify( const sword::AutomatCreation& message, const std::string& identifier );
    //@}

    //! @name Operations
    //@{
    virtual void Created( const std::string& identifier );
    virtual void Destroyed( const std::string& identifier );
    virtual void Moved( const std::string& identifier, double latitude, double longitude );
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
    //@}

private:
    //! @name Helpers
    //@{
    void AddFormation( unsigned long party );
    void AddAutomat( unsigned long formation, unsigned long knowledgeGroup );
    unsigned long FindKnowledgeGroup( unsigned long party ) const;
    unsigned long FindAutomat( rpr::ForceIdentifier ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< simulation::UnitMagicAction > T_UnitCreation;
    typedef std::map< std::string, T_UnitCreation > T_UnitCreations;
    typedef std::map< unsigned long, unsigned long > T_Parties;
    typedef std::map< std::string, unsigned long > T_Units;
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageController_ABC< sword::SimToClient_Content >& controller_;
    dispatcher::Model_ABC& model_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    RemoteAgentSubject_ABC& agentSubject_;
    const unsigned long automatType_;
    const ContextFactory_ABC& contextFactory_;
    ContextHandler_ABC< sword::FormationCreation >& formationHandler_;
    ContextHandler_ABC< sword::AutomatCreation >& automatHandler_;
    ContextHandler_ABC< sword::UnitCreation >& unitHandler_;
    T_UnitCreations unitCreations_;
    T_Parties parties_;
    T_Units units_;
    //@}
};

}
}

#endif // plugins_hla_RemoteAgentController_h
