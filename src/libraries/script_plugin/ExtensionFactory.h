// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExtensionFactory_h_
#define __ExtensionFactory_h_

#include "dispatcher/ExtensionFactory_ABC.h"

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class Agent;
    class AgentKnowledge;
    class Automat;
    class Object;
    class ObjectKnowledge;
    class Population;
    class PopulationKnowledge;
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace script
{
// =============================================================================
/** @class  ExtensionFactory
    @brief  Extension factory
*/
// Created: AGE 2008-06-12
// =============================================================================
class ExtensionFactory : public dispatcher::ExtensionFactory_ABC< dispatcher::Agent >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::AgentKnowledge >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::Automat >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::Object >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::ObjectKnowledge >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::Population >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::PopulationKnowledge >
{
public:
    //! @name Constructors/Destructor
    //@{
             ExtensionFactory( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, dispatcher::SimulationPublisher_ABC& publisher );
    virtual ~ExtensionFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Create( dispatcher::Agent& entity );
    virtual void Create( dispatcher::AgentKnowledge& entity );
    virtual void Create( dispatcher::Automat& entity );
    virtual void Create( dispatcher::Object& entity );
    virtual void Create( dispatcher::ObjectKnowledge& entity );
    virtual void Create( dispatcher::Population& entity );
    virtual void Create( dispatcher::PopulationKnowledge& entity );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExtensionFactory( const ExtensionFactory& );            //!< Copy constructor
    ExtensionFactory& operator=( const ExtensionFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    //@}
};

}
}

#endif // __ExtensionFactory_h_
