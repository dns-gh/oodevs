// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __TimelinePublisher_h_
#define __TimelinePublisher_h_

#include "protocol/ServerPublisher_ABC.h"
#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  TimelinePublisher
    @brief  TimelinePublisher
*/
// Created: ABR 2013-05-31
// =============================================================================
class TimelinePublisher : public Publisher_ABC
                        , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelinePublisher();
    virtual ~TimelinePublisher();
    //@}

    //! @name Operations
    //@{
    const std::string& GetPayload() const;
    //@}

    //! @name Publisher_ABC implementation
    //@{
    virtual void Send( const sword::ClientToSim& message );
    virtual void Register( T_SimHandler handler );
    virtual void Register( T_ReplayHandler handler );
    virtual void Send( const sword::ClientToAuthentication& message );
    virtual void Send( const sword::ClientToReplay& message );
    virtual void Send( const sword::ClientToAar& message );
    virtual void Send( const sword::ClientToMessenger& message );
    //@}

private:
    //! @name Member data
    //@{
    std::string currentPayload_;
    //@}
};

} //! namespace gui

#endif // __TimelinePublisher_h_
