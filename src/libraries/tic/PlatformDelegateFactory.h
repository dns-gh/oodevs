// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PlatformDelegateFactory_h_
#define __PlatformDelegateFactory_h_

#include "PlatformDelegateFactory_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace tic
{
// =============================================================================
/** @class  PlatformDelegateFactory
    @brief  Plaform delegate factory implementation
*/
// Created: AGE 2008-03-31
// =============================================================================
class PlatformDelegateFactory : public PlatformDelegateFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PlatformDelegateFactory( const kernel::CoordinateConverter_ABC& converter, float timeStep );
    virtual ~PlatformDelegateFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::unique_ptr< PlatformDelegate_ABC > Create( dispatcher::Agent_ABC& entity ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const float timeStep_;
    //@}
};
}

#endif // __PlatformDelegateFactory_h_
