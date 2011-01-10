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

namespace dispatcher
{
    class Model_ABC;
    class Agent;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  ExtensionFactory
    @brief  Extension factory
*/
// Created: SBO 2008-02-18
// =============================================================================
class ExtensionFactory : public dispatcher::ExtensionFactory_ABC< dispatcher::Agent >,
                         public AgentSubject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ExtensionFactory( dispatcher::Model_ABC& model );
    virtual ~ExtensionFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Create( dispatcher::Agent& entity );
    virtual void Register( AgentListener_ABC& listener );
    virtual void Unregister( AgentListener_ABC& listener );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< AgentListener_ABC* > T_Listeners;
    typedef T_Listeners::const_iterator     CIT_Listeners;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model_ABC& model_;
    T_Listeners listeners_;
    //@}
};

}
}

#endif // plugins_hla_ExtensionFactory_h
