// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EventCondition_h_
#define __EventCondition_h_

#include "BaseCondition.h"
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controller;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  EventCondition
    @brief  EventCondition
*/
// Created: AGE 2008-06-17
// =============================================================================
class EventCondition : public BaseCondition
                     , public kernel::Observer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit EventCondition( kernel::Controller& controller );
    virtual ~EventCondition();
    //@}

    //! @name Operations
    //@{
    virtual void Activate( bool active );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EventCondition( const EventCondition& );            //!< Copy constructor
    EventCondition& operator=( const EventCondition& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    bool active_;
    //@}
};

// =============================================================================
/** @class  SimpleEventCondition
    @brief  SimpleEventCondition
*/
// Created: AGE 2008-06-17
// =============================================================================
template< typename E >
class SimpleEventCondition : public EventCondition
                           , public kernel::ElementObserver_ABC< E >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SimpleEventCondition( kernel::Controller& controller ) : EventCondition( controller ) {};
    virtual ~SimpleEventCondition()
    {
        Activate( false );
    };
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const E& event ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimpleEventCondition( const SimpleEventCondition& );            //!< Copy constructor
    SimpleEventCondition& operator=( const SimpleEventCondition& ); //!< Assignment operator
    //@}
};

}
}

#endif // __EventCondition_h_
