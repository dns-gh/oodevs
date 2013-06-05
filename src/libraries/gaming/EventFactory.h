// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventFactory_h_
#define __EventFactory_h_

#include <boost/noncopyable.hpp>

namespace timeline
{
    struct Event;
}

class Event;

// =============================================================================
/** @class  EventFactory
    @brief  EventFactory
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventFactory : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             EventFactory();
    virtual ~EventFactory();
    //@}

    //! @name Operations
    //@{
    Event* Create( const timeline::Event& event ) const;
    Event* Create( E_EventTypes type, const timeline::Event* event = 0 ) const;
    //@}
};

#endif // __EventFactory_h_
