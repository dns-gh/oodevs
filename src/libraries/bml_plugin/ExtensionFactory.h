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
}

namespace bml
{
    class Publisher;

// =============================================================================
/** @class  ExtensionFactory
    @brief  ExtensionFactory
*/
// Created: SBO 2008-02-29
// =============================================================================
class ExtensionFactory : public dispatcher::ExtensionFactory_ABC< dispatcher::Agent >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::Automat >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ExtensionFactory( Publisher& publisher );
    virtual ~ExtensionFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Create( dispatcher::Agent& entity );
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
    Publisher& publisher_;
    //@}
};

}

#endif // __ExtensionFactory_h_
