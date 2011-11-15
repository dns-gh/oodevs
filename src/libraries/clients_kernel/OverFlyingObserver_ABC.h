// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __OverFlyingObserver_ABC_h_
#define __OverFlyingObserver_ABC_h_

#include <boost/noncopyable.hpp>
#include "tools/Observer_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  OverFlyingObserver_ABC
    @brief  OverFlyingObserver_ABC
*/
// Created: ABR 2011-10-27
// =============================================================================
class OverFlyingObserver_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             OverFlyingObserver_ABC() {}
    virtual ~OverFlyingObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void BeforeOverFlying() = 0;
    virtual void AfterOverFlying() = 0;
    //@}
};

// =============================================================================
/** @class  OverFlyingObserver_Base
    @brief  OverFlyingObserver_Base
*/
// Created: ABR 2011-10-27
// =============================================================================
template< typename T >
class OverFlyingObserver_Base : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             OverFlyingObserver_Base() {}
    virtual ~OverFlyingObserver_Base() {}
    //@}

    //! @name Operations
    //@{
    virtual void OverFly( const T& element ) = 0;
    //@}
};

// =============================================================================
/** @class  OverFlyingObserver
    @brief  OverFlyingObserver
*/
// Created: ABR 2011-10-27
// =============================================================================
template< typename T >
class OverFlyingObserver : public OverFlyingObserver_Base< T >, public OverFlyingObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             OverFlyingObserver() : element_( 0 ) {}
    virtual ~OverFlyingObserver() {}
    //@}

    //! @name Operations
    //@{
    virtual void BeforeOverFlying() { element_ = 0; };
    virtual void AfterOverFlying() { NotifyOverFlown( element_ ); };
    virtual void OverFly( const T& element ) { element_ = &element; };

    virtual void NotifyOverFlown( const T* element ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    const T* element_;
    //@}
};

}

#endif // __OverFlyingObserver_ABC_h_
