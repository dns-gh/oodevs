// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionObserver_ABC_h_
#define __ActionObserver_ABC_h_

class Agent;

// =============================================================================
/** @class  ActionObserver_ABC
    @brief  Action observer definition
*/
// Created: AGE 2006-02-16
// =============================================================================
class ActionObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionObserver_ABC() {};
    virtual ~ActionObserver_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelected( const Agent& agent ) = 0;
    //@}
};

#endif // __ActionObserver_ABC_h_
