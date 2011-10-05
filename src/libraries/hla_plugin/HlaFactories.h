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
#include "AggregateEntity.h"
#include "NetnAggregate.h"
#include "RemoteAggregate.h"
#include "NetnRemoteAggregate.h"
#include "SurfaceVessel.h"
#include "NetnSurfaceVessel.h"
#include "RemoteSurfaceVessel.h"
#include "NetnRemoteSurfaceVessel.h"

namespace plugins
{
namespace hla
{
    class HlaObjectFactory : public HlaObjectFactory_ABC
    {
        virtual std::auto_ptr< HlaObject_ABC > CreateAggregate( Agent_ABC& agent, const std::string& name, unsigned short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const
        {
            return std::auto_ptr< HlaObject_ABC >( new AggregateEntity( agent, identifier, name, force, type ) );
        }
        virtual std::auto_ptr< HlaObject_ABC > CreateSurfaceVessel( Agent_ABC& agent, const std::string& name, unsigned short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const
        {
            return std::auto_ptr< HlaObject_ABC >( new SurfaceVessel( agent, identifier, name, force, type ) );
        }
    };
    class NetnHlaObjectFactory : public HlaObjectFactory_ABC
    {
    public:
        explicit NetnHlaObjectFactory( const HlaObjectFactory_ABC& factory )
            : factory_( factory )
        {}
        virtual std::auto_ptr< HlaObject_ABC > CreateAggregate( Agent_ABC& agent, const std::string& name, unsigned short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const
        {
            std::auto_ptr< HlaObject_ABC > aggregate = factory_.CreateAggregate( agent, name, identifier, force, type );
            return std::auto_ptr< HlaObject_ABC >( new NetnAggregate( aggregate, agent, name, identifier ) );
        }
        virtual std::auto_ptr< HlaObject_ABC > CreateSurfaceVessel( Agent_ABC& agent, const std::string& name, unsigned short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const
        {
            std::auto_ptr< HlaObject_ABC > aggregate = factory_.CreateSurfaceVessel( agent, name, identifier, force, type );
            return std::auto_ptr< HlaObject_ABC >( new NetnSurfaceVessel( aggregate, name, identifier ) );
        }
    private:
        const HlaObjectFactory_ABC& factory_;
    };
    class RemoteHlaObjectFactory : public RemoteHlaObjectFactory_ABC
    {
    public:
        virtual std::auto_ptr< HlaObject_ABC > CreateAggregate( const std::string& name, RemoteAgentListener_ABC& listener ) const
        {
            return std::auto_ptr< HlaObject_ABC >( new RemoteAggregate( name, listener ) );
        }
        virtual std::auto_ptr< HlaObject_ABC > CreateSurfaceVessel( const std::string& name, RemoteAgentListener_ABC& listener ) const
        {
            return std::auto_ptr< HlaObject_ABC >( new RemoteSurfaceVessel( name, listener ) );
        }
    };
    class NetnRemoteHlaObjectFactory : public RemoteHlaObjectFactory_ABC
    {
    public:
        explicit NetnRemoteHlaObjectFactory( const RemoteHlaObjectFactory_ABC& factory )
            : factory_( factory )
        {}
        virtual std::auto_ptr< HlaObject_ABC > CreateAggregate( const std::string& name, RemoteAgentListener_ABC& listener ) const
        {
            std::auto_ptr< HlaObject_ABC > remote = factory_.CreateAggregate( name, listener );
            return std::auto_ptr< HlaObject_ABC >( new NetnRemoteAggregate( remote ) );
        }
        virtual std::auto_ptr< HlaObject_ABC > CreateSurfaceVessel( const std::string& name, RemoteAgentListener_ABC& listener ) const
        {
            std::auto_ptr< HlaObject_ABC > remote = factory_.CreateAggregate( name, listener );
            return std::auto_ptr< HlaObject_ABC >( new NetnRemoteAggregate( remote ) );
        }
    private:
        const RemoteHlaObjectFactory_ABC& factory_;
    };
}
}

#endif // plugins_hla_HlaFactories_h
