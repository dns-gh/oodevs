// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReplayPlugin_h_
#define __ReplayPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "MT_Tools/MT_Timer_ABC.h"
#include "MT_Tools/MT_TimerManager.h"

namespace sword
{
    class ClientToReplay;
    class TimeTableRequest_TimeRange;
}

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace dispatcher
{
    class Loader;
    class Model_ABC;
    class ReplayModel_ABC;
}

namespace plugins
{
namespace replay
{
    class ReplayExtensionFactory;

// =============================================================================
/** @class  ReplayPlugin
    @brief  ReplayPlugin
*/
// Created: AGE 2007-08-24
// =============================================================================
class ReplayPlugin : public dispatcher::Plugin_ABC, private MT_Timer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ReplayPlugin( dispatcher::Model_ABC& model, dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& clientCommands,
                           dispatcher::Loader& loader, const dispatcher::ReplayModel_ABC& replayModel );
    virtual ~ReplayPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string& link, dispatcher::Profile_ABC& profile );

    virtual void Register( dispatcher::Services& services );
    virtual void Update();
    //@}

private:
    //! @name Operations
    //@{
    virtual void OnTimer();
    //@}

    //! @name Helpers
    //@{
    void OnReceive( const std::string& link, const sword::ClientToReplay& asnMsg );
    void ChangeTimeFactor( unsigned int factor );
    void Pause();
    void Resume();
    void SkipToFrame( unsigned int frame );
    void RequestTimeTable( const sword::TimeTableRequest_TimeRange& msg );
    void SendReplayInfo( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model_ABC& model_;
    dispatcher::ClientPublisher_ABC& clients_;
    dispatcher::Loader& loader_;
    unsigned int factor_;
    unsigned int tickNumber_;
    bool running_;
    int skipToFrame_;
    MT_TimerManager manager_;
    std::auto_ptr< ReplayExtensionFactory > factory_;
    //@}
};

}
}

#endif // __ReplayPlugin_h_
