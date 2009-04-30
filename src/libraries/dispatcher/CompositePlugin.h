// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __CompositePlugin_h_
#define __CompositePlugin_h_

#include "Plugin_ABC.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace dispatcher
{
// =============================================================================
/** @class  CompositePlugin
    @brief  Composite plugin
*/
// Created: AGE 2007-08-24
// =============================================================================
class CompositePlugin : public Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CompositePlugin() {};
    virtual ~CompositePlugin() {};
    //@}

    //! @name Operations
    //@{
    void Add( const boost::shared_ptr< Plugin_ABC >& plugin )
    {
        plugins_.push_back( plugin );
        handlers_.push_back( plugin );
    }

    void Add( Plugin_ABC* plugin )
    {
        Add( boost::shared_ptr< Plugin_ABC >( plugin ) );
    }

    void AddHandler( const boost::shared_ptr< MessageHandler_ABC >& handler )
    {
        handlers_.push_back( handler );
    }

    void AddHandler( MessageHandler_ABC* handler )
    {
        handlers_.push_back( boost::shared_ptr< MessageHandler_ABC >( handler ) );
    }

    virtual void Receive( const ASN1T_MsgsSimToClient& message )
    {
        for( CIT_Handlers it = handlers_.begin(); it != handlers_.end(); ++it )
            (*it)->Receive( message );
    }

    virtual void Receive( const ASN1T_MsgsAarToClient& message )
    {
        for( CIT_Handlers it = handlers_.begin(); it != handlers_.end(); ++it )
            (*it)->Receive( message );
    }

    virtual void Register( dispatcher::Services& services )
    {
        for( CIT_Plugins it = plugins_.begin(); it != plugins_.end(); ++it )
            (*it)->Register( services );
    }

    virtual void NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& profile )
    {
        for( CIT_Plugins it = plugins_.begin(); it != plugins_.end(); ++it )
            (*it)->NotifyClientAuthenticated( client, profile );
    }

    virtual void NotifyClientLeft( ClientPublisher_ABC& client )
    {
        for( CIT_Plugins it = plugins_.begin(); it != plugins_.end(); ++it )
            (*it)->NotifyClientLeft( client );
    }

    virtual void Update()
    {
        for( CIT_Plugins it = plugins_.begin(); it != plugins_.end(); ++it )
            (*it)->Update();
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CompositePlugin( const CompositePlugin& );            //!< Copy constructor
    CompositePlugin& operator=( const CompositePlugin& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< Plugin_ABC > > T_Plugins;
    typedef T_Plugins::const_iterator                    CIT_Plugins;
    typedef std::vector< boost::shared_ptr< MessageHandler_ABC > > T_Handlers;
    typedef T_Handlers::const_iterator                           CIT_Handlers;
    //@}

private:
    //! @name Member data
    //@{
    T_Plugins plugins_;
    T_Handlers handlers_;
    //@}
};

}

#endif // __CompositePlugin_h_
