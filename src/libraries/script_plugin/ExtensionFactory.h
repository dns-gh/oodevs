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
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  ExtensionFactory
    @brief  ExtensionFactory
*/
// Created: AGE 2008-06-12
// =============================================================================
class ExtensionFactory : public dispatcher::ExtensionFactory_ABC< dispatcher::Agent >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::AgentKnowledge >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::Automat >
{

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionFactory( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ExtensionFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Create( dispatcher::Agent& entity );
    virtual void Create( dispatcher::AgentKnowledge& entity );
    virtual void Create( dispatcher::Automat& entity );
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
    //@}
};

}
}

#endif // __ExtensionFactory_h_
