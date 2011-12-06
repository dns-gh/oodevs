// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************


#ifndef launcher_ControlEndTickMessageHandler_h
#define launcher_ControlEndTickMessageHandler_h

#include "ClientMessageHandlerBase.h"
#include <boost/shared_ptr.hpp>

namespace launcher
{
// =============================================================================
/** @class  ControlEndTickMessageHandler
    @brief  Control end tick message handler
*/
// Created: LGY 2011-09-07
// =============================================================================
class ControlEndTickMessageHandler : public ClientMessageHandlerBase
{
public:
    //! @name Constructors/Destructor
    //@{
             ControlEndTickMessageHandler( boost::shared_ptr< LauncherPublisher > publisher, const std::string& exercise, const std::string& session );
    virtual ~ControlEndTickMessageHandler();
    //@}

    //! @name Operations
    //@{
    virtual bool OnReceiveMessage( const sword::SimToClient& message );
    //@}

private:
    //! @name Member Data
    //@{
    unsigned int timeFactor_;
    unsigned int checkpoint_;
    unsigned int tick_;
    //@}
};

}

#endif // launcher_ControlEndTickMessageHandler_h
