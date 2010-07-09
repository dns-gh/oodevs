// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ScriptPlugin_h_
#define __ScriptPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "dispatcher/Registrable_ABC.h"
#include "dispatcher/Position.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include "directia/ScriptRef.h"

#include "protocol/protocol.h"

using namespace Common;
using namespace MsgsClientToMessenger;
using namespace MsgsSimToClient;
using namespace MsgsAarToClient;


namespace dispatcher
{
    class Config;
    class Model;
    class SimulationPublisher_ABC;
    class LinkResolver_ABC;
    class Registrable_ABC;
    class CompositeRegistrable;
}

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
    class CoordinateConverter;
    class StaticModel;
}

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace plugins
{
namespace script
{
    class Script;
    class Actions;
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
             ScriptPlugin( dispatcher::Model& model, const kernel::StaticModel& staticModel, const dispatcher::Config& config, dispatcher::SimulationPublisher_ABC& publisher
                 , tools::MessageDispatcher_ABC& dispatcher, dispatcher::ClientPublisher_ABC& clients, dispatcher::LinkResolver_ABC& resolver
                 , dispatcher::CompositeRegistrable& registrables );
    virtual ~ScriptPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive                  ( const MsgSimToClient& message );
    virtual void Receive                  ( const MsgAarToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
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
    void LoadScript( const std::string& file );
    void OnReceiveClientToMessenger( const std::string&, const MsgClientToMessenger& );
    virtual void RegisterIn( directia::Brain& brain );

    void Reset();
    void Later( const directia::ScriptRef& function );
    std::string ToUtm( const dispatcher::Position& position );
    dispatcher::Position UtmPosition( const std::string& utm );

    void ApplyPendings();
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model& model_;
    const dispatcher::Config& config_;
    dispatcher::CompositeRegistrable& registrables_;
    std::auto_ptr< kernel::Controller > controller_;
    std::auto_ptr< kernel::CoordinateConverter > converter_;
    std::auto_ptr< ExtensionFactory > factory_;
    std::auto_ptr< Actions > actions_;
    boost::ptr_vector< Script > scripts_;
    boost::ptr_vector< directia::ScriptRef > pending_;
    long time_;
    bool reset_;
    unsigned int tickDuration_;
    //@}
};

}
}

#endif // __ScriptPlugin_h_
