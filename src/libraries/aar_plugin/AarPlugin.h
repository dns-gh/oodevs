// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AarPlugin_h_
#define __AarPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include <memory>
#include <string>

namespace sword
{
    class ClientToAar;
    class PlotRequest;
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
    class AuthenticatedLinkResolver_ABC;
}

namespace aar
{
    class StaticModel;
}

namespace plugins
{
namespace aar
{

// =============================================================================
/** @class  AarPlugin
    @brief  AAR plugin
*/
// Created: AGE 2007-09-17
// =============================================================================
class AarPlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AarPlugin( tools::MessageDispatcher_ABC& dispatcher, dispatcher::AuthenticatedLinkResolver_ABC& resolver, const dispatcher::Config& config );
    virtual ~AarPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string& link,
                                            dispatcher::Profile_ABC& profile, unsigned int clientId, bool uncounted );

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
    void OnReceive( const std::string&, const sword::ClientToAar& message );
    void OnReceiveIndicatorRequest( const std::string& client, const sword::PlotRequest& request );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::AuthenticatedLinkResolver_ABC& resolver_;
    std::unique_ptr< dispatcher::MessageLoader > messages_;
    std::unique_ptr< ::aar::StaticModel > model_;
    //@}
};

}
}

#endif // __AarPlugin_h_
