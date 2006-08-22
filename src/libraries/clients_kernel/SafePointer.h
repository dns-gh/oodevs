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

#include "ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;

// =============================================================================
/** @class  SafePointer
    @brief  safer pointer. duh !
*/
// Created: AGE 2006-04-20
// =============================================================================
template< typename T >
class SafePointer : public Observer_ABC
                  , public ElementObserver_ABC< T >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SafePointer( Controllers& controllers, const T* element = 0 );
    virtual ~SafePointer();
    //@}

    //! @name Operations
    //@{
    operator const T*() const { return element_; };
    const T& operator *() const { return *element_; };
    const T* operator->() const { return &*element_; };
    SafePointer& operator=( const T* element ) { element_ = element; return *this; };
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const T& );
    //@}

private:
    //! @name Member data
    //@{
    Controllers* controllers_;
    const T* element_;
    //@}
};

}

#include "Controllers.h"
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SafePointer constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename T >
SafePointer< T >::SafePointer( Controllers& controllers, const T* element /*= 0*/ )
    : controllers_( &controllers )
    , element_( element )
{
    controllers_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SafePointer destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
template< typename T >
SafePointer< T >::~SafePointer()
{
    controllers_->Remove( *this );
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

#endif // __SafePointer_h_
