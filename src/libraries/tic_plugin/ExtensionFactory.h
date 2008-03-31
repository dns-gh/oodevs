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

namespace tic
{

// =============================================================================
/** @class  ExtensionFactory
    @brief  Extension Factory
*/
// Created: AGE 2008-03-31
// =============================================================================
class ExtensionFactory : public dispatcher::ExtensionFactory_ABC< dispatcher::Agent >
{

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionFactory();
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
};

}

#endif // __ExtensionFactory_h_
