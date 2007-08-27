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

namespace DIN
{
    class DIN_Link;
}

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace dispatcher
{
    class LoaderFacade;

// =============================================================================
/** @class  ReplayPlugin
    @brief  ReplayPlugin
    // $$$$ AGE 2007-08-27: fusionner avec LoaderFacade
*/
// Created: AGE 2007-08-24
// =============================================================================
class ReplayPlugin : public Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ReplayPlugin( LoaderFacade& loader, tools::MessageDispatcher_ABC& clientCommands );
    virtual ~ReplayPlugin();
    //@}

    //! @name Operations
    //@{
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
    void OnReceive( DIN::DIN_Link& link, const ASN1T_MsgsClientToReplay& asnMsg );
    //@}

private:
    //! @name Member data
    //@{
    LoaderFacade& loader_;
    //@}
};

}

#endif // __ReplayPlugin_h_
