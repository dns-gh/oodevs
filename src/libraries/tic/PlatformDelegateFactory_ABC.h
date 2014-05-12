// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PlatformDelegateFactory_ABC_h_
#define __PlatformDelegateFactory_ABC_h_

#include <boost/noncopyable.hpp>

namespace dispatcher
{
    class Agent_ABC;
}

namespace tic
{
    class PlatformDelegate_ABC;
// =============================================================================
/** @class  PlatformDelegateFactory_ABC
    @brief  Platform delegate factory definition
*/
// Created: SBO 2011-04-15
// =============================================================================
class PlatformDelegateFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PlatformDelegateFactory_ABC() {}
    virtual ~PlatformDelegateFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::unique_ptr< PlatformDelegate_ABC > Create( dispatcher::Agent_ABC& entity ) const = 0;
    //@}
};
}

#endif // __PlatformDelegateFactory_ABC_h_
