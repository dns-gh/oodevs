// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ScriptPlugin_h_
#define __ScriptPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "dispatcher/Registrable_ABC.h"
#include "dispatcher/Position.h"
#include "directia/tools/binders/ScriptRef.h"
#include "protocol/Protocol.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace dispatcher
{
    class Config;
    class Model_ABC;
    class SimulationPublisher_ABC;
    class AuthenticatedLinkResolver_ABC;
    class Registrable_ABC;
    class CompositeRegistrable;
}

namespace kernel
{
    class Controller;
    class CoordinateConverter;
}

namespace tools
{
    class MessageDispatcher_ABC;
    class Path;
}

namespace plugins
{
namespace script
{
    class Script;
    class ExtensionFactory;

// =============================================================================
/** @class  ScriptPlugin
    @brief  ScriptPlugin
*/
// Created: AGE 2008-06-12
// =============================================================================
class ScriptPlugin : public dispatcher::Plugin_ABC
                   , public dispatcher::Registrable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ScriptPlugin( dispatcher::Model_ABC& model,
                           const dispatcher::Config& config,
                           dispatcher::SimulationPublisher_ABC& publisher,
                           tools::MessageDispatcher_ABC& dispatcher,
                           dispatcher::ClientPublisher_ABC& clients,
                           dispatcher::AuthenticatedLinkResolver_ABC& resolver,
                           dispatcher::CompositeRegistrable& registrables );
    virtual ~ScriptPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive                  ( const sword::SimToClient& message );
    virtual void Receive                  ( const sword::AarToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string& link,
                                            dispatcher::Profile_ABC& profile, bool uncounted );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client, const std::string& link, bool uncounted );
    virtual void Update();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScriptPlugin( const ScriptPlugin& );            //!< Copy constructor
    ScriptPlugin& operator=( const ScriptPlugin& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    void LoadScripts();
    void LoadScript( const tools::Path& file );
    void OnReceiveClientToMessenger( const std::string&, const sword::ClientToMessenger& );
    virtual void RegisterIn( directia::brain::Brain& brain );

    void Reset();
    void Later( const directia::tools::binders::ScriptRef& function );
    std::string ToUtm( const dispatcher::Position& position );
    dispatcher::Position UtmPosition( const std::string& utm );

    tools::Path GenerateOrdersScript( const std::vector< tools::Path >& files );

    void ApplyPendings();
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model_ABC& model_;
    const dispatcher::Config& config_;
    std::auto_ptr< kernel::Controller > controller_;
    dispatcher::CompositeRegistrable& registrables_;
    std::auto_ptr< kernel::CoordinateConverter > converter_;
    std::auto_ptr< ExtensionFactory > factory_;
    boost::ptr_vector< Script > scripts_;
    boost::ptr_vector< directia::tools::binders::ScriptRef > pending_;
    long time_;
    bool reset_;
    unsigned int tickDuration_;
    bool loaded_;
    //@}
};

}
}

#endif // __ScriptPlugin_h_
