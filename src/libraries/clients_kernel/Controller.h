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

#include "tools/InterfaceContainer.h"
#include "ElementObserver_ABC.h"
#include "ControllerObserver_ABC.h"
#include "Observer_ABC.h"

namespace kernel
{
    class Agent_ABC;

// =============================================================================
/** @class  Controller
    @brief  Controller
*/
// Created: AGE 2006-02-13
// =============================================================================
class Controller : private tools::InterfaceContainer< Observer_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Controller();
    virtual ~Controller();
    //@}

    //! @name Operations
    //@{
    void Register  ( Observer_ABC& observer );
    void Unregister( Observer_ABC& observer );

    template< typename T >
    void Create( const T& extension ) {
        Apply( & ElementObserver_ABC< T >::NotifyCreated, extension );
        Apply( & ControllerObserver_ABC::NotifyCreated );
    };

    template< typename T >
    void Update( const T& extension ) {
        Apply( & ElementObserver_ABC< T >::NotifyUpdated, extension );
        Apply( & ControllerObserver_ABC::NotifyUpdated );
    };

    template< typename T >
    void Delete( const T& extension ) {
        Apply( & ElementObserver_ABC< T >::NotifyDeleted, extension );
        Apply( & ControllerObserver_ABC::NotifyDeleted );
    };
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Controller( const Controller& );            //!< Copy constructor
    Controller& operator=( const Controller& ); //!< Assignement operator
    //@}
};

}

#endif // __Controller_h_
