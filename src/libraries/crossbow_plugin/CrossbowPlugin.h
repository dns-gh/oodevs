// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_CrossbowPlugin_h_
#define __crossbow_CrossbowPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "dispatcher/NullClientPublisher.h"

namespace sword
{
    class SimToClient;
    class MessengerToClient;
}

namespace xml
{
    class xistream;
}

namespace kernel
{
    class StaticModel;
}

namespace dispatcher
{
    class Model_ABC;
    class Config;
    class SimulationPublisher_ABC;
    class Logger_ABC;
}

namespace tools
{
    class MessageDispatcher_ABC;
    class ClientNetworker;
}

namespace plugins
{
namespace crossbow
{
    class CrossbowPublisher;

// =============================================================================
/** @class  CrossbowPlugin
    @brief  CrossbowPlugin
*/
// Created: JCR 2007-08-29
// =============================================================================
class CrossbowPlugin : public dispatcher::Plugin_ABC
                     , public dispatcher::NullClientPublisher
{
public:
    //! @name Constructors/Destructor
    //@{
             CrossbowPlugin( dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Config& config, xml::xistream& xis, dispatcher::Logger_ABC& logger );
    virtual ~CrossbowPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& asnMsg );
    virtual void Send   ( const sword::MessengerToClient& msg );
    virtual void Update();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CrossbowPlugin( const CrossbowPlugin& );            //!< Copy constructor
    CrossbowPlugin& operator=( const CrossbowPlugin& ); //!< Assignment operator
    //@}

    //! @name Copy/Assignment
    //@{
    void OnReceiveMessengerToClient( const std::string& /*link*/, const sword::MessengerToClient& message );
    //@}

public:
    class ListenerThread;

private:
    //! @name Member data
    //@{
    std::auto_ptr< CrossbowPublisher > crossbowPublisher_;
    std::auto_ptr< tools::ClientNetworker > clientNetworker_;
    std::auto_ptr< dispatcher::Plugin_ABC > messenger_;
    std::auto_ptr< ListenerThread > listeners_;
    //@}
};

}
}

#endif // __crossbow_CrossbowPlugin_h_
