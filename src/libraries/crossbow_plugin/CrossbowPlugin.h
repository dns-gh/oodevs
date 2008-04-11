// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossbowPlugin_h_
#define __CrossbowPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "game_asn/Messenger.h"

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Model;
    class Config;    
    class SimulationPublisher_ABC;     
}

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace crossbow
{
    class DatabasePublisher;

// =============================================================================
/** @class  CrossbowPlugin
    @brief  CrossbowPlugin
*/
// Created: JCR 2007-08-29
// =============================================================================
class CrossbowPlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CrossbowPlugin( dispatcher::Model& model, tools::MessageDispatcher_ABC&, const dispatcher::Config& config, dispatcher::SimulationPublisher_ABC& publisher, xml::xistream& xis );
    virtual ~CrossbowPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive                    ( const ASN1T_MsgsSimToClient& asnMsg );
            void OnReceiveMessengerToClient (const std::string&, const ASN1T_MsgsMessengerToClient&);
    virtual void NotifyClientAuthenticated  ( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft           ( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CrossbowPlugin( const CrossbowPlugin& );            //!< Copy constructor
    CrossbowPlugin& operator=( const CrossbowPlugin& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< DatabasePublisher > databasePublisher_;
    //@}
};
}
#endif // __CrossbowPlugin_h_
