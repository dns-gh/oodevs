// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef launcher_TimeMessageHandler_h
#define launcher_TimeMessageHandler_h

#include "ClientMessageHandlerBase.h"
#include <boost/shared_ptr.hpp>

namespace launcher
{
// =============================================================================
/** @class  TimeMessageHandler
    @brief  Time message handler
*/
// Created: LGY 2011-06-22
// =============================================================================
class TimeMessageHandler : public ClientMessageHandlerBase
{
public:
    //! @name Constructors/Destructor
    //@{
             TimeMessageHandler( boost::shared_ptr< LauncherPublisher > publisher, const std::string& exercise, const std::string& session );
    virtual ~TimeMessageHandler();
    //@}

    //! @name Operations
    //@{
    virtual bool OnReceiveMessage( const sword::SimToClient& message );
    //@}
};

}

#endif // launcher_TimeMessageHandler_h
