// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExtensionObserver_ABC_h_
#define __ExtensionObserver_ABC_h_

// =============================================================================
/** @class  ExtensionObserver_ABC
    @brief  Extension observer definition
*/
// Created: AGE 2006-02-13
// =============================================================================
template< typename Extension >
class ExtensionObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionObserver_ABC() {};
    virtual ~ExtensionObserver_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const Extension& extension ) = 0;
    virtual void NotifyUpdated( const Extension& extension ) = 0;
    virtual void NotifyDeleted( const Extension& extension ) = 0;
    //@}
};

#endif // __ExtensionObserver_ABC_h_
