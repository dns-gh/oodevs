// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Agent_ABC_h_
#define __Agent_ABC_h_

#include "Entity_ABC.h"

// =============================================================================
/** @class  Agent_ABC
    @brief  Agent base class. 
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent_ABC : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent_ABC();
    virtual ~Agent_ABC();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const = 0;
    //@}
};

#endif // __Agent_ABC_h_
