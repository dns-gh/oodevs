// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterContainer_ABC_h_
#define __ActionParameterContainer_ABC_h_

#include "clients_kernel/Resolver.h"

class ActionParameter_ABC;

// =============================================================================
/** @class  ActionParameterContainer_ABC
    @brief  Action parameter container definition
*/
// Created: AGE 2007-07-11
// =============================================================================
class ActionParameterContainer_ABC : public kernel::Resolver< ActionParameter_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterContainer_ABC() {};
    virtual ~ActionParameterContainer_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void AddParameter( ActionParameter_ABC& parameter ) = 0;
    //@}
};

#endif // __ActionParameterContainer_ABC_h_
