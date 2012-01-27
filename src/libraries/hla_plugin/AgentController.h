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
}

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class ComponentTypes_ABC;

// =============================================================================
/** @class  AgentController
    @brief  Agent controller
*/
// Created: SBO 2008-02-18
// =============================================================================
class AgentController : public AgentSubject_ABC
                      , private dispatcher::ExtensionFactory_ABC< dispatcher::Agent >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentController( dispatcher::Model_ABC& model, const rpr::EntityTypeResolver_ABC& aggregatesResolver,
                              const rpr::EntityTypeResolver_ABC& componentTypeResolver, const ComponentTypes_ABC& componentTypes );
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
    //@}

private:
    //! @name Helpers
    //@{
    void CreateAgent( dispatcher::Agent_ABC& agent );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< AgentListener_ABC* > T_Listeners;
    typedef T_Listeners::const_iterator     CIT_Listeners;
    typedef boost::shared_ptr< Agent_ABC > T_Agent;
    typedef std::vector< T_Agent >         T_Agents;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model_ABC& model_;
    const rpr::EntityTypeResolver_ABC& aggregatesResolver_;
    const rpr::EntityTypeResolver_ABC& componentTypeResolver_;
    const ComponentTypes_ABC& componentTypes_;
    T_Listeners listeners_;
    T_Agents agents_;
    //@}
};

}
}

#endif // plugins_hla_ExtensionFactory_h
