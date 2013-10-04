// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CompositePlugin_h_
#define __CompositePlugin_h_

#include "Plugin_ABC.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace dispatcher
{
    class Logger_ABC;

// =============================================================================
/** @class  CompositePlugin
    @brief  Composite plugin
*/
// Created: AGE 2007-08-24
// =============================================================================
class CompositePlugin : public Plugin_ABC
{
public:
    //! @name Operations
    //@{
    void Add( const boost::shared_ptr< Plugin_ABC >& plugin )
    {
        plugins_.push_back( plugin );
        handlers_.push_back( plugin );
    }

    void Add( const boost::shared_ptr< Plugin_ABC >& plugin, const boost::shared_ptr< Logger_ABC >& logger )
    {
        Add( plugin );
        loggers_.push_back( logger );
    }

    void Add( Plugin_ABC* plugin )
    {
        Add( boost::shared_ptr< Plugin_ABC >( plugin ) );
    }

    void AddHandler( const boost::shared_ptr< MessageHandler_ABC >& handler )
    {
        handlers_.push_back( handler );
    }

    virtual void Receive( const sword::SimToClient& message )
    {
        for( auto it = handlers_.begin(); it != handlers_.end(); ++it )
            (*it)->Receive( message );
    }

    virtual void Receive( const sword::AarToClient& message )
    {
        for( auto it = handlers_.begin(); it != handlers_.end(); ++it )
            (*it)->Receive( message );
    }

    virtual void Receive( const sword::MessengerToClient& message )
    {
        for( auto it = handlers_.begin(); it != handlers_.end(); ++it )
            (*it)->Receive( message );
    }

    virtual void Register( dispatcher::Services& services )
    {
        for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
            (*it)->Register( services );
    }

    virtual void NotifyClientAuthenticated( ClientPublisher_ABC& client, const std::string& link, Profile_ABC& profile, bool uncounted )
    {
        for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
            (*it)->NotifyClientAuthenticated( client, link, profile, uncounted );
    }

    virtual void NotifyClientLeft( ClientPublisher_ABC& client, const std::string& link )
    {
        for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
            (*it)->NotifyClientLeft( client, link );
    }

    virtual void Update()
    {
        for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
            (*it)->Update();
    }

    virtual void Close()
    {
        for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
            (*it)->Close();
    }

    virtual bool Filter( const sword::SimToClient& message ) const
    {
        for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
            if( (*it)->Filter( message ) )
                return true;
        return false;
    }
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< Logger_ABC > > loggers_;
    std::vector< boost::shared_ptr< Plugin_ABC > > plugins_;
    std::vector< boost::shared_ptr< MessageHandler_ABC > > handlers_;
    //@}
};

}

#endif // __CompositePlugin_h_
