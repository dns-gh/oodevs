// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Controller_h_
#define __Controller_h_

#include "InterfaceContainer.h"
#include "ElementObserver_ABC.h"
#include "Observer_ABC.h"

class Agent;

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
//        MT_LOG_INFO_MSG( "Creating " << typeid( T ).name() );
        Apply( ElementObserver_ABC< T >::NotifyCreated, extension );
    };

    template< typename T >
    void Update( const T& extension ) {
//        MT_LOG_INFO_MSG( "Updating " << typeid( T ).name() );
        Apply( ElementObserver_ABC< T >::NotifyUpdated, extension );
    };

    template< typename T >
    void Delete( const T& extension ) {
//        MT_LOG_INFO_MSG( "Deleting " << typeid( T ).name() );
        Apply( ElementObserver_ABC< T >::NotifyDeleted, extension );
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
