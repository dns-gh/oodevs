// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_HlaFactories_h
#define plugins_hla_HlaFactories_h

#include "HlaObjectFactory_ABC.h"
#include "RemoteHlaObjectFactory_ABC.h"
#include "CallsignResolver_ABC.h"
#include <boost/lexical_cast.hpp>

namespace plugins
{
namespace hla
{
    template< typename T >
    class HlaObjectFactory : public HlaObjectFactory_ABC
    {
        virtual std::auto_ptr< HlaObject_ABC > Create( Agent_ABC& agent, const std::string& name, unsigned int identifier, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& /*symbol*/ ) const
        {
            return std::auto_ptr< HlaObject_ABC >( new T( agent, identifier, name, force, type ) );
        }
    };
    template< typename T >
    class NetnHlaObjectFactory : public HlaObjectFactory_ABC
    {
    public:
        NetnHlaObjectFactory( std::auto_ptr< HlaObjectFactory_ABC > factory, CallsignResolver_ABC& resolver )
            : factory_ ( factory )
            , resolver_( resolver )
        {}
        virtual std::auto_ptr< HlaObject_ABC > Create( Agent_ABC& agent, const std::string& name, unsigned int identifier, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol ) const
        {
            std::auto_ptr< HlaObject_ABC > object = factory_->Create( agent, name, identifier, force, type, symbol );
            const std::string uniqueIdentifier( "SWORD" + boost::lexical_cast< std::string >( identifier ) );
            const std::string callsign( name + boost::lexical_cast< std::string >( identifier ) );
            resolver_.Add( identifier, callsign, uniqueIdentifier );
            return std::auto_ptr< HlaObject_ABC >( new T( object, agent, callsign, uniqueIdentifier, symbol ) );
        }
    private:
        std::auto_ptr< HlaObjectFactory_ABC > factory_;
        CallsignResolver_ABC& resolver_;
    };
    template< typename T >
    class RemoteHlaObjectFactory : public RemoteHlaObjectFactory_ABC
    {
    public:
        virtual std::auto_ptr< HlaObject_ABC > Create( const std::string& name, RemoteAgentListener_ABC& listener ) const
        {
            return std::auto_ptr< HlaObject_ABC >( new T( name, listener ) );
        }
    };
    template< typename T >
    class NetnRemoteHlaObjectFactory : public RemoteHlaObjectFactory_ABC
    {
    public:
        explicit NetnRemoteHlaObjectFactory( std::auto_ptr< RemoteHlaObjectFactory_ABC > factory )
            : factory_( factory )
        {}
        virtual std::auto_ptr< HlaObject_ABC > Create( const std::string& name, RemoteAgentListener_ABC& listener ) const
        {
            std::auto_ptr< HlaObject_ABC > remote = factory_->Create( name, listener );
            return std::auto_ptr< HlaObject_ABC >( new T( remote, listener, name ) );
        }
    private:
        std::auto_ptr< RemoteHlaObjectFactory_ABC > factory_;
    };
}
}

#endif // plugins_hla_HlaFactories_h
