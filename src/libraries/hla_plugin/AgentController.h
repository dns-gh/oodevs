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
#include <boost/shared_ptr.hpp>

namespace dispatcher
{
    class Model_ABC;
    class Agent;
}

namespace plugins
{
namespace hla
{
    class Agent_ABC;

// =============================================================================
/** @class  AgentController
    @brief  Extension factory
*/
// Created: SBO 2008-02-18
// =============================================================================
class AgentController : public dispatcher::ExtensionFactory_ABC< dispatcher::Agent >,
                         public AgentSubject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentController( dispatcher::Model_ABC& model );
    virtual ~AgentController();
    //@}

    //! @name Operations
    //@{
    virtual void Create( dispatcher::Agent& entity ); // $$$$ _RC_ SLI 2011-02-04: handle also destruction
    virtual void Register( AgentListener_ABC& listener );
    virtual void Unregister( AgentListener_ABC& listener );
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
    T_Listeners listeners_;
    T_Agents agents_;
    //@}
};

}
}

#endif // plugins_hla_ExtensionFactory_h
