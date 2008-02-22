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

namespace dispatcher
{
    class Agent;
    class Automat;
    class Formation;
    class Object;
    class AgentKnowledge;
}

namespace gearth
{

// =============================================================================
/** @class  ExtensionFactory
    @brief  ExtensionFactory
*/
// Created: SBO 2008-02-13
// =============================================================================
class ExtensionFactory : public dispatcher::ExtensionFactory_ABC< dispatcher::Agent >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::Automat >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::Formation >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::Object >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::AgentKnowledge >
{

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionFactory();
    virtual ~ExtensionFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Create( dispatcher::Agent&          entity );
    virtual void Create( dispatcher::Automat&        entity );
    virtual void Create( dispatcher::Formation&      entity );
    virtual void Create( dispatcher::Object&         entity );
    virtual void Create( dispatcher::AgentKnowledge& entity );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExtensionFactory( const ExtensionFactory& );            //!< Copy constructor
    ExtensionFactory& operator=( const ExtensionFactory& ); //!< Assignment operator
    //@}
};

}

#endif // __ExtensionFactory_h_
