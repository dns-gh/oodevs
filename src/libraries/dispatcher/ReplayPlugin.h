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

#include "Plugin_ABC.h"
#include "MT/MT_Time/MT_Timer_ABC.h"
#include "MT/MT_Time/MT_TimerManager.h"
#include "game_asn/Replay.h"

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace dispatcher
{
    class Loader;
    class Model;
    class ReplayModel_ABC;
    class ReplayExtensionFactory;

// =============================================================================
/** @class  ReplayPlugin
    @brief  ReplayPlugin
*/
// Created: AGE 2007-08-24
// =============================================================================
class ReplayPlugin : public Plugin_ABC
                   , private MT_Timer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ReplayPlugin( Model& model, ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& clientCommands, Loader& loader, const ReplayModel_ABC& replayModel );
    virtual ~ReplayPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Update();

    virtual void Receive( const ASN1T_MsgsSimToClient& message );
    virtual void NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& profile );
    virtual void NotifyClientLeft         ( ClientPublisher_ABC& client );
    virtual void Register                 ( dispatcher::Services& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReplayPlugin( const ReplayPlugin& );            //!< Copy constructor
    ReplayPlugin& operator=( const ReplayPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void OnTimer();
    void OnReceive( const std::string& link, const ASN1T_MsgsClientToReplay& asnMsg );
    void ChangeTimeFactor( unsigned factor );
    void Pause();
    void Resume();
    void SkipToFrame( unsigned frame );
    void SendReplayInfo( ClientPublisher_ABC& client );
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    ClientPublisher_ABC& clients_;
    Loader& loader_;

    unsigned factor_;
    bool running_;
    int skipToFrame_;

    MT_TimerManager manager_;

    std::auto_ptr< ReplayExtensionFactory > factory_;
    //@}
};

}

#endif // __ReplayPlugin_h_
