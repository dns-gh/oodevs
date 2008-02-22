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
}

namespace hla
{
    class AggregateEntityClass;

// =============================================================================
/** @class  ExtensionFactory
    @brief  ExtensionFactory
*/
// Created: SBO 2008-02-18
// =============================================================================
class ExtensionFactory : public dispatcher::ExtensionFactory_ABC< dispatcher::Agent >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ExtensionFactory( AggregateEntityClass& agentClass );
    virtual ~ExtensionFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Create( dispatcher::Agent& entity );
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
    AggregateEntityClass& agentClass_;
    //@}
};

}

#endif // __ExtensionFactory_h_
