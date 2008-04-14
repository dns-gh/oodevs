// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessengerPlugin_h_
#define __MessengerPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "game_asn/Messenger.h"

namespace dispatcher
{
    class Model;
    class Config;
    class ClientPublisher_ABC;
    class LinkResolver_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace tools
{
    class MessageDispatcher_ABC;
    class SessionConfig ;
}

namespace xml
{
    class xistream ;
}

namespace messenger
{

    class ExtensionFactory;
    class TacticalLineManager;
    class IntelligenceManager;
    class IdManager;

// =============================================================================
/** @class  MessengerPlugin
    @brief  MessengerPlugin
*/
// Created: AGE 2008-04-01
// =============================================================================
class MessengerPlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MessengerPlugin( dispatcher::ClientPublisher_ABC& client, tools::MessageDispatcher_ABC&, dispatcher::LinkResolver_ABC& links, const dispatcher::Config& config );
    virtual ~MessengerPlugin();
    //@}

    //! @name Network
    //@{
    virtual void Receive                  ( const ASN1T_MsgsSimToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessengerPlugin( const MessengerPlugin& );            //!< Copy constructor
    MessengerPlugin& operator=( const MessengerPlugin& ); //!< Assignment operator
    //@}

    //! @name Network
    //@{
    void OnReceiveClientToMessenger (const std::string&, const ASN1T_MsgsClientToMessenger&);
    //@}

    //! @name xml read / write
    //@{
    void Load(const tools::SessionConfig& config);
    void ReadSide(xml::xistream& xis);
    void ReadFormation(xml::xistream& xis);
    void ReadAutomat(xml::xistream& xis);
    void ReadIntelligence(xml::xistream& xis, unsigned int sideId, unsigned int formationId );
    //@}

    //! @name Checkpoint
    //@{
    std::string GetCheckPointFileName(const std::string& dir);
    void        SaveCheckPoint(const std::string& name);
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Config& config_ ;
          dispatcher::ClientPublisher_ABC& clients_;
          dispatcher::LinkResolver_ABC& links_;
          std::auto_ptr< TacticalLineManager > tacticalLineManager_;
          std::auto_ptr< IntelligenceManager > intelligenceManager_;
          std::auto_ptr< IdManager > idManager_;
          std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    //@}
};

}

#endif // __MessengerPlugin_h_
