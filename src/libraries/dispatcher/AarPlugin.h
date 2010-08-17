// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AarPlugin_h_
#define __AarPlugin_h_

#include "Plugin_ABC.h"

namespace MsgsClientToAar
{
    class MsgClientToAar;
    class MsgPlotRequest;
}

namespace tools
{
    class MessageDispatcher_ABC;
}

class Task;

namespace dispatcher
{
    class Config;
    class MessageLoader;
    class LinkResolver_ABC;

// =============================================================================
/** @class  AarPlugin
    @brief  AAR plugin
*/
// Created: AGE 2007-09-17
// =============================================================================
class AarPlugin : public Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AarPlugin( tools::MessageDispatcher_ABC& dispatcher, LinkResolver_ABC& resolver, const Config& config );
    virtual ~AarPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& message );

    virtual void NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& profile );
    virtual void NotifyClientLeft( ClientPublisher_ABC& client );

    virtual void Register( dispatcher::Services& services );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AarPlugin( const AarPlugin& );            //!< Copy constructor
    AarPlugin& operator=( const AarPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void OnReceive( const std::string&, const MsgsClientToAar::MsgClientToAar& message );
    void OnReceiveIndicatorRequest( const std::string& client, const MsgsClientToAar::MsgPlotRequest& request );
    //@}

private:
    //! @name Member data
    //@{
    LinkResolver_ABC& resolver_;
    std::auto_ptr< MessageLoader >   messages_;
    //@}
};

}

#endif // __AarPlugin_h_
