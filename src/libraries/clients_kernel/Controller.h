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

#include <tools/SortedInterfaceContainer.h>
#include <tools/ElementObserver_ABC.h>
#include <tools/ControllerObserver_ABC.h>
#include <tools/Observer_ABC.h>

namespace kernel
{

// =============================================================================
/** @class  Controller
    @brief  Controller
*/
// Created: AGE 2006-02-13
// =============================================================================
class Controller : private tools::SortedInterfaceContainer< tools::Observer_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Controller();
    virtual ~Controller();
    //@}

    //! @name Operations
    //@{
    void Register  ( tools::Observer_ABC& observer );
    void Unregister( tools::Observer_ABC& observer );

    template< typename T >
    void Create( const T& extension ) {
        Apply( & tools::ElementObserver_ABC< T >::NotifyCreated, extension );
        Apply( & tools::ControllerObserver_ABC::NotifyCreated );
    };

    template< typename T >
    void Update( const T& extension ) {
        Apply( & tools::ElementObserver_ABC< T >::NotifyUpdated, extension );
        Apply( & tools::ControllerObserver_ABC::NotifyUpdated );
    };

    template< typename T >
    void Delete( const T& extension ) {
        Apply( & tools::ElementObserver_ABC< T >::NotifyDeleted, extension );
        Apply( & tools::ControllerObserver_ABC::NotifyDeleted );
    };
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Controller( const Controller& );            //!< Copy constructor
    Controller& operator=( const Controller& ); //!< Assignment operator
    //@}
};

}

#endif // __Controller_h_
