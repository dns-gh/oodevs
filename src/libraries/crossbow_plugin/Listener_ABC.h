// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Listener_ABC_h_
#define __Listener_ABC_h_

namespace dispatcher
{
    class Publisher_ABC;
}

namespace crossbow
{

// =============================================================================
/** @class  Listener_ABC
    @brief  Listener_ABC    
*/
// Created: JCR 2007-06-14
// =============================================================================
class Listener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Listener_ABC() {}
    virtual ~Listener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Listen( dispatcher::Publisher_ABC& publisher ) = 0;
    //@}
};

}

#endif // __Listener_ABC_h_
