// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventsModel_h_
#define __EventsModel_h_

#include "Event.h"
#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
}

namespace timeline
{
    struct Event;
}

namespace gui
{
    class EventFactory;

// =============================================================================
/** @class  EventsModel
    @brief  EventsModel
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventsModel : public tools::StringResolver< Event >
                  , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             EventsModel( const EventFactory& factory, kernel::Controller& controller );
    virtual ~EventsModel();
    //@}

    //! @name Operations
    //@{
    Event* Create( const timeline::Event& event );
    void Update( const timeline::Event& event );
    void Destroy( const std::string& uuid );
    void Purge();
    //@}

private:
    //! @name Member data
    //@{
    const EventFactory& factory_;
    kernel::Controller& controller_;
    //@}
};

} //! namespace gui

#endif // __EventsModel_h_
