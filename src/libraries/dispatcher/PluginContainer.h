// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef DISPATCHER_PLUGINCONTAINER_H
#define DISPATCHER_PLUGINCONTAINER_H

#include "Plugin_ABC.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace dispatcher
{

class PluginContainer : public Plugin_ABC
{
public:
    //! @name Operations
    //@{
    void Add( const boost::shared_ptr< Plugin_ABC >& plugin );
    void AddHandler( const boost::shared_ptr< MessageHandler_ABC >& handler );

    virtual void Receive( const sword::SimToClient& message );
    virtual void Receive( const sword::AarToClient& message );
    virtual void Receive( const sword::MessengerToClient& message );

    virtual void Register( dispatcher::Services& services );

    virtual void NotifyClientAuthenticated( ClientPublisher_ABC& client,
            const std::string& link, Profile_ABC& profile, bool uncounted );
    virtual void NotifyClientLeft( ClientPublisher_ABC& client,
            const std::string& link, bool uncounted );

    virtual void Update();
    virtual void Close();
    virtual bool Filter( const sword::SimToClient& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< Plugin_ABC > > plugins_;
    std::vector< boost::shared_ptr< MessageHandler_ABC > > handlers_;
    //@}
};

}

#endif //DISPATCHER_PLUGINCONTAINER_H

