// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ElementObserver_ABC_h_
#define __ElementObserver_ABC_h_

#include "Observer_ABC.h"

// =============================================================================
/** @class  ElementObserver_ABC
    @brief  Extension observer definition
*/
// Created: AGE 2006-02-13
// =============================================================================
template< typename Element >
class ElementObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ElementObserver_ABC() {};
    virtual ~ElementObserver_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const Element& ) {};
    virtual void NotifyUpdated( const Element& ) {};
    virtual void NotifyDeleted( const Element& ) {};
    //@}
};

#endif // __ElementObserver_ABC_h_
