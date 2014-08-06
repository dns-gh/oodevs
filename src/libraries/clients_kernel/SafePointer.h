// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SafePointer_h_
#define __SafePointer_h_

#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controller;
    class Controllers;

// =============================================================================
/** @class  SafePointer
    @brief  safer pointer. duh !
*/
// Created: AGE 2006-04-20
// =============================================================================
template< typename T >
class SafePointer : public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< T >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SafePointer( Controller& controller, const T* element = 0 );
    explicit SafePointer( Controllers& controllers, const T* element = 0 );
             SafePointer( const SafePointer& other );
    virtual ~SafePointer();
    //@}

    //! @name Operations
    //@{
    operator const T*() const { return element_; };
    const T& operator *() const { return *element_; };
    const T* operator->() const { return &*element_; };
    SafePointer& operator=( const T* element ) { element_ = element; return *this; };
    SafePointer& operator=( const SafePointer& other ) { element_ = other.element_; return *this; }
    T* ConstCast() const { return const_cast< T* >( element_ ); }
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const T& );
    //@}

private:
    //! @name Member data
    //@{
    Controller* controller_;
    const T* element_;
    //@}
};

}

#include "Controllers.h"
#include "Controller.h"

namespace kernel
{

// -----------------------------------------------------------------------------
// Name: SafePointer constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename T >
SafePointer< T >::SafePointer( kernel::Controllers& controllers, const T* element /* = 0*/ )
    : controller_( &controllers.controller_ )
    , element_( element )
{
    controller_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SafePointer constructor
// Created: JSR 2013-01-30
// -----------------------------------------------------------------------------
template< typename T >
SafePointer< T >::SafePointer( kernel::Controller& controller, const T* element /*= 0*/ )
    : controller_( &controller )
    , element_( element )
{
    controller_->Register( *this );
}

template< typename T >
SafePointer< T >::SafePointer( const SafePointer& other )
    : controller_( other.controller_ )
    , element_( other.element_ )
{
    controller_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SafePointer destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename T >
SafePointer< T >::~SafePointer()
{
    controller_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SafePointer::NotifyDeleted
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename T >
void SafePointer< T >::NotifyDeleted( const T& element )
{
    if( &element == element_ )
        element_ = 0;
}

}

#endif // __SafePointer_h_
