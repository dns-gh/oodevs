// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SelectionObserver_ABC_h_
#define __SelectionObserver_ABC_h_

#include "Observer_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  SelectionObserver_ABC
    @brief  SelectionObserver_ABC
*/
// Created: AGE 2006-02-16
// =============================================================================
class SelectionObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SelectionObserver_ABC() {};
    virtual ~SelectionObserver_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void BeforeSelection() = 0;
    virtual void AfterSelection() = 0;
    //@}
};

// =============================================================================
/** @class  SelectionObserver_Base
    @brief  SelectionObserver_Base
*/
// Created: AGE 2006-02-16
// =============================================================================
template< typename T >
class SelectionObserver_Base
{

public:
    //! @name Constructors/Destructor
    //@{
             SelectionObserver_Base() {};
    virtual ~SelectionObserver_Base() {};
    //@}

    //! @name Operations
    //@{
    virtual void Select( const T& element ) = 0;
    //@}
};

// =============================================================================
/** @class  SelectionObserver
    @brief  SelectionObserver
*/
// Created: AGE 2006-02-16
// =============================================================================
template< typename T >
class SelectionObserver : public SelectionObserver_Base< T >, public SelectionObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SelectionObserver() : element_( 0 ){};
    virtual ~SelectionObserver() {};
    //@}

    //! @name Operations
    //@{
    virtual void BeforeSelection() { element_ = 0; };
    virtual void AfterSelection() { NotifySelected( element_ ); };
    virtual void Select( const T& element ) { element_ = &element; };

    virtual void NotifySelected( const T* element ) = 0;
    //@}

private:
    const T* element_;
};

}

#endif // __SelectionObserver_ABC_h_
