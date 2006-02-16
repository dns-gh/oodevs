// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionController_h_
#define __ActionController_h_

#include "InterfaceContainer.h"
#include "Observer_ABC.h"

class Agent;

// =============================================================================
/** @class  ActionController
    @brief  Action controller
*/
// Created: AGE 2006-02-16
// =============================================================================
class ActionController : private InterfaceContainer< Observer_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionController();
    virtual ~ActionController();
    //@}

    //! @name Operations
    //@{
    void Register( Observer_ABC& observer );
    void Remove  ( Observer_ABC& observer );
    //@}

    //! @name Operations
    //@{
    void Select( const Agent& agent );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ActionController( const ActionController& );            //!< Copy constructor
    ActionController& operator=( const ActionController& ); //!< Assignement operator
    //@}
};

#endif // __ActionController_h_
