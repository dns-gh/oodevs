// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Controller_h_
#define __Controller_h_

#include "InterfaceContainer.h"
#include "ExtensionObserver_ABC.h"

// =============================================================================
/** @class  Controller
    @brief  Controller
*/
// Created: AGE 2006-02-13
// =============================================================================
class Controller : private InterfaceContainer< Observer_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Controller();
    virtual ~Controller();
    //@}

    //! @name Operations
    //@{
    void Register( Observer_ABC& observer );
    void Remove  ( Observer_ABC& observer );

    template< typename T >
    void Create( const T& extension ) {
        Apply( ExtensionObserver_ABC< T >::NotifyCreated, extension );
    };

    template< typename T >
    void Update( const T& extension ) {
        Apply( ExtensionObserver_ABC< T >::NotifyUpdated, extension );
    };

    template< typename T >
    void Delete( const T& extension ) {
        Apply( ExtensionObserver_ABC< T >::NotifyDeleted, extension );
    };
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Controller( const Controller& );            //!< Copy constructor
    Controller& operator=( const Controller& ); //!< Assignement operator
    //@}
};

#endif // __Controller_h_
