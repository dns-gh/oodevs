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
#include "protocol/SimulationSenders.h"

#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>
#include <vector>

namespace boost
{
    class shared_mutex;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace tools
{
    template< typename category > class MessageController;
}

namespace plugins
{
namespace web_control
{
// -----------------------------------------------------------------------------
// Name: Observer structure
// Created: BAX 2012-30-07
// -----------------------------------------------------------------------------
struct Observer
{
             Observer() {}
    virtual ~Observer() {}
};

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

    //! @name Public methods
    //@{
    void Receive( const sword::SimToClient& client );
    void NotifyClientAuthenticated( const std::string& link, bool uncounted );
    void NotifyClientLeft( const std::string& link );
    //@}

    //! @name WebObserver_ABC methods
    //@{
    virtual std::string Notify( const std::string& method, const std::string& uri );
    //@}

private:
    //! @name Private operations
    //@{
    std::string Get();
    std::string Play();
    std::string Pause();
    std::string Stop();
    void        OnSimulationState( sword::EnumSimulationState state );
    void        OnControlInformation( const sword::ControlInformation& control );
    void        OnControlBeginTick( const sword::ControlBeginTick& msg );
    //@}

    //! @name Private helpers
    //@{
    typedef boost::unordered_set< std::string > T_Clients;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    boost::shared_ptr< boost::shared_mutex > access_;
    boost::shared_ptr< tools::MessageController< sword::SimToClient_Content > > controller_;
    std::vector< boost::shared_ptr< Observer > > observers_;
    T_Clients clients_;
    sword::EnumSimulationState state_;
    std::string start_time_;
    std::string current_time_;
    //@}
};
}
}

#endif // WEB_CONTROL_PLUGIN_H
