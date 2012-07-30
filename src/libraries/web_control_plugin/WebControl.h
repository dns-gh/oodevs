// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef WEB_CONTROL_PLUGIN_H
#define WEB_CONTROL_PLUGIN_H

#include "WebObserver_ABC.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace boost
{
    class shared_mutex;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace sword
{
    class SimToClient;
    class SimToClient_Content;
    enum  EnumSimulationState;
}

namespace tools
{
    template< typename category > class MessageController;
}

namespace plugins
{
namespace web_control
{
    struct StateObserver;

// =============================================================================
/** @class  WebControl
    @brief  Web control
*/
// Created: BAX 2012-02-24
// =============================================================================
class WebControl : public WebObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit WebControl( dispatcher::SimulationPublisher_ABC& publisher );
    virtual ~WebControl();
    //@}

    //! @name Operations
    //@{
            void        Receive( const sword::SimToClient& client );
    virtual std::string Notify( const std::string& method, const std::string& uri );
    //@}

private:
    //! @name Private operations
    //@{
    std::string Get();
    std::string Play();
    std::string Pause();
    std::string Stop();
    void        UpdateState( sword::EnumSimulationState state );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    sword::EnumSimulationState state_;
    boost::shared_ptr< boost::shared_mutex > access_;
    boost::shared_ptr< tools::MessageController< sword::SimToClient_Content > > controller_;
    std::vector< boost::shared_ptr< StateObserver > > observers_;
    //@}
};
}
}

#endif // WEB_CONTROL_PLUGIN_H
