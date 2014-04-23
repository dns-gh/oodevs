// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __TimelineHandler_ABC_h_
#define __TimelineHandler_ABC_h_

#include <boost/noncopyable.hpp>

namespace timeline
{
    struct Event;
}

namespace kernel
{

// =============================================================================
/** @class  TimelineHandler_ABC
    @brief  TimelineHandler_ABC
*/
// Created: ABR 2013-12-06
// =============================================================================
class TimelineHandler_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelineHandler_ABC() {}
    virtual ~TimelineHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void CreateEvent( const timeline::Event& event, bool select = false ) = 0;
    virtual void SelectEvent( const std::string& uuid ) = 0;
    virtual void EditEvent( const timeline::Event& event ) = 0;
    virtual void DeleteEvent( const std::string& uuid ) = 0;
    virtual const std::string& GetCurrentParent() const = 0;
    //@}
};

} //! namespace kernel

#endif // __TimelineHandler_ABC_h_
