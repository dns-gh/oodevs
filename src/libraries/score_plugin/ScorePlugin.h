// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ScorePlugin_h_
#define __ScorePlugin_h_

#include "dispatcher/Plugin_ABC.h"

namespace sword
{
    class SimToClient;
    class AarToClient;
    class ClientToAar;
}

namespace xml
{
    class xistream;
}

namespace tools
{
    class SessionConfig;
    class MessageDispatcher_ABC;
}

namespace dispatcher
{
    class CompositeRegistrable;
    class LinkResolver_ABC;
}

namespace plugins
{
namespace score
{
    class ScoresModel;

// =============================================================================
/** @class  ScorePlugin
    @brief  Score plugin
*/
// Created: AGE 2008-08-04
// =============================================================================
class ScorePlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ScorePlugin( tools::MessageDispatcher_ABC& dispatcher, dispatcher::LinkResolver_ABC& resolver, dispatcher::ClientPublisher_ABC& clients, const tools::SessionConfig& config, dispatcher::CompositeRegistrable& registrables );
    virtual ~ScorePlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive                  ( const sword::SimToClient& message );
    virtual void Receive                  ( const sword::AarToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string& link,
                                            dispatcher::Profile_ABC& profile, bool uncounted );
    virtual void Register                 ( dispatcher::Services& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScorePlugin( const ScorePlugin& );            //!< Copy constructor
    ScorePlugin& operator=( const ScorePlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void LoadScores( const std::string& scores );
    void LoadIndicators( xml::xistream& xis );
    void LoadIndicator ( xml::xistream& xis );
    void OnReceive( const std::string&, const sword::ClientToAar& message );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::LinkResolver_ABC& resolver_;
    const tools::SessionConfig& config_;
    std::auto_ptr< ScoresModel > scores_;
    //@}
};

}
}

#endif // __ScorePlugin_h_
