// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_ExtensionFactory_h
#define plugins_hla_ExtensionFactory_h

#include "AgentSubject_ABC.h"
#include "dispatcher/ExtensionFactory_ABC.h"
#include <vector>
#include <set>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xisubstream;
    class xistream;
}

namespace dispatcher
{
    class Model_ABC;
    class Agent;
    class Agent_ABC;    
    class Formation_ABC;
    class Formation;
    class Automat_ABC;
    class Automat;
    class Logger_ABC;
}

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace tic
{
    class PlatformDelegateFactory_ABC;
    class Platform_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class AgentProxy;
    class ComponentTypes_ABC;
    class SideResolver_ABC;
    class LocalAgentResolver_ABC;
    class AgentAdapter;

// =============================================================================
/** @class  AgentController
    @brief  Agent controller
*/
// Created: SBO 2008-02-18
// =============================================================================
class AgentController : public AgentSubject_ABC
                      , private dispatcher::ExtensionFactory_ABC< dispatcher::Agent >
                      , private dispatcher::ExtensionFactory_ABC< dispatcher::Automat >
                      , private dispatcher::ExtensionFactory_ABC< dispatcher::Formation >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentController( dispatcher::Model_ABC& model, const rpr::EntityTypeResolver_ABC& aggregatesResolver,
                              const rpr::EntityTypeResolver_ABC& componentTypeResolver, const ComponentTypes_ABC& componentTypes,
                              tic::PlatformDelegateFactory_ABC& factory, const kernel::CoordinateConverter_ABC& converter, bool sendPlatforms,
                              const SideResolver_ABC& sideResolver, LocalAgentResolver_ABC& localAgentResolver, bool fullOrbat,
                              dispatcher::Logger_ABC& logger, const rpr::EntityTypeResolver_ABC& automatEntityTypeResolver, int netnVersion );
    virtual ~AgentController();
    //@}

    //! @name Operations
    //@{
    virtual void Register( AgentListener_ABC& listener );
    virtual void Unregister( AgentListener_ABC& listener );
    void Visit( dispatcher::Model_ABC& model );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Create( dispatcher::Agent& entity );
    virtual void Create( dispatcher::Automat& entity );
    virtual void Create( dispatcher::Formation& entity );

    //@}

private:
    //! @name Helpers
    //@{
    void CreateAgent( dispatcher::Agent_ABC& agent );
    void CreateAutomat( dispatcher::Automat_ABC& agent );
    void CreateFormation( dispatcher::Formation_ABC& agent );
    void NotifyPlatformCreation( Agent_ABC& agent, dispatcher::Agent_ABC&, const tic::Platform_ABC&, int childIndex );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< AgentListener_ABC* > T_Listeners;
    typedef T_Listeners::const_iterator     CIT_Listeners;
    typedef boost::shared_ptr< Agent_ABC > T_Agent;
    typedef std::map< unsigned long, T_Agent >         T_Agents;
    typedef std::map< unsigned long, unsigned long > T_Parents;
    typedef boost::shared_ptr< AgentAdapter > T_AgentAdapter;
    typedef std::vector< T_AgentAdapter >     T_AgentAdapters;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model_ABC& model_;
    const rpr::EntityTypeResolver_ABC& aggregatesResolver_;
    const rpr::EntityTypeResolver_ABC& componentTypeResolver_;
    const ComponentTypes_ABC& componentTypes_;
    tic::PlatformDelegateFactory_ABC& factory_;
    const kernel::CoordinateConverter_ABC& converter_;
    bool doDisaggregation_;
    bool fullOrbat_;
    const SideResolver_ABC& sideResolver_;
    LocalAgentResolver_ABC& localAgentResolver_;
    dispatcher::Logger_ABC& logger_;
    const rpr::EntityTypeResolver_ABC& automatEntityTypeResolver_;
    const int netnVersion_;
    T_Listeners listeners_;
    T_Agents agents_;
    T_AgentAdapters adapters_;
    T_Parents parents_;
    //@}
};

}
}

#endif // plugins_hla_ExtensionFactory_h
