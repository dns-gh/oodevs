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
#include "game_asn/Asn.h"

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace dispatcher
{
    class Loader;

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
             ReplayPlugin( ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& clientCommands, Loader& loader );
    virtual ~ReplayPlugin();
    //@}

    //! @name Operations
    //@{
    void Update();

    virtual void Receive( const ASN1T_MsgsSimToClient& message );
    virtual void NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& profile );
    virtual void NotifyClientLeft         ( ClientPublisher_ABC& client );
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
    //@}

private:
    //! @name Member data
    //@{
    ClientPublisher_ABC& clients_;
    Loader& loader_;

    unsigned factor_;
    bool running_;
    int skipToFrame_;

    MT_TimerManager manager_;
    //@}
};

}

#endif // __ReplayPlugin_h_
