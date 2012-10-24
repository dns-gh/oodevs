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
#include "EntityIdentifierResolver_ABC.h"
#include "CallsignResolver_ABC.h"
#include <boost/lexical_cast.hpp>

namespace plugins
{
namespace hla
{
    class MarkingFactory_ABC;
    class EntityIdentifierResolver_ABC;
    class FOM_Serializer_ABC;

    template< typename T >
    class HlaObjectFactory : public HlaObjectFactory_ABC
    {
    public:
        HlaObjectFactory( const MarkingFactory_ABC& markingFactory,
                EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer )
            : markingFactory_( markingFactory )
            , entityIdentifierResolver_( entityIdentifierResolver )
            , fomSerializer_( fomSerializer )
        {}
        virtual std::auto_ptr< HlaObject_ABC > Create( Agent_ABC& agent, const std::string& name, unsigned long /*identifier*/, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& /*symbol*/, const std::string& rtiId, const std::string& /*uniqueId*/ ) const
        {
            rpr::EntityIdentifier entityId;
            entityIdentifierResolver_.Create( rtiId, entityId );
            return std::auto_ptr< HlaObject_ABC >( new T( agent, name, force, type, markingFactory_, entityId, entityIdentifierResolver_, fomSerializer_, rtiId ) );
        }
    private:
        const MarkingFactory_ABC& markingFactory_;
        EntityIdentifierResolver_ABC& entityIdentifierResolver_;
        FOM_Serializer_ABC& fomSerializer_;
    };
    template< typename T >
    class NetnHlaObjectFactory : public HlaObjectFactory_ABC
    {
    public:
        NetnHlaObjectFactory( std::auto_ptr< HlaObjectFactory_ABC > factory, CallsignResolver_ABC& resolver, FOM_Serializer_ABC& fomSerializer )
            : factory_ ( factory )
            , resolver_( resolver )
            , fomSerializer_( fomSerializer )
        {}
        virtual std::auto_ptr< HlaObject_ABC > Create( Agent_ABC& agent, const std::string& name, unsigned long simId, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol, const std::string& rtiId, const std::string& uniqueId ) const
        {
            std::auto_ptr< HlaObject_ABC > object = factory_->Create( agent, name, simId, force, type, symbol, rtiId, uniqueId );
            const std::string callsign( name + boost::lexical_cast< std::string >( simId ) );
            resolver_.Add( simId, callsign, uniqueId );
            return std::auto_ptr< HlaObject_ABC >( new T( object, agent, callsign, uniqueId, symbol, fomSerializer_ ) );
        }
    private:
        std::auto_ptr< HlaObjectFactory_ABC > factory_;
        CallsignResolver_ABC& resolver_;
        FOM_Serializer_ABC& fomSerializer_;
    };
    template< typename T >
    class RemoteHlaObjectFactory : public RemoteHlaObjectFactory_ABC
    {
    public:
        explicit RemoteHlaObjectFactory( EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer )
            : entityIdentifierResolver_( entityIdentifierResolver )
            , fomSerializer_( fomSerializer )
        {
            // NOTHING
        }
        virtual std::auto_ptr< HlaObject_ABC > Create( const std::string& name ) const
        {
            std::auto_ptr< HlaObject_ABC > retval( new T( name, entityIdentifierResolver_, fomSerializer_ ) );
            return retval;
        }
    private:
        EntityIdentifierResolver_ABC& entityIdentifierResolver_;
        FOM_Serializer_ABC& fomSerializer_;
    };
    template< typename T >
    class NetnRemoteHlaObjectFactory : public RemoteHlaObjectFactory_ABC
    {
    public:
        explicit NetnRemoteHlaObjectFactory( std::auto_ptr< RemoteHlaObjectFactory_ABC > factory, FOM_Serializer_ABC& fomSerializer )
            : factory_( factory )
            , fomSerializer_( fomSerializer )
        {}
        virtual std::auto_ptr< HlaObject_ABC > Create( const std::string& name ) const
        {
            std::auto_ptr< HlaObject_ABC > remote = factory_->Create( name );
            std::auto_ptr< HlaObject_ABC > retval( new T( remote, name, fomSerializer_ ) );
            return retval;
        }
    private:
        std::auto_ptr< RemoteHlaObjectFactory_ABC > factory_;
        FOM_Serializer_ABC& fomSerializer_;
    };

    template< typename T >
    class TacticalObjectFactory : public HlaTacticalObjectFactory_ABC
    {
    public:
        TacticalObjectFactory( EntityIdentifierResolver_ABC& entityIdentifierResolver )
            : entityIdentifierResolver_( entityIdentifierResolver )
        {}
        virtual std::auto_ptr< HlaObject_ABC > Create( TacticalObject_ABC& object, const std::string& name, unsigned int identifier, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& rtiId ) const
        {
            rpr::EntityIdentifier entityId;
            entityIdentifierResolver_.Create( rtiId, entityId );
            return std::auto_ptr< HlaObject_ABC >( new T( object, identifier, name, force, type, entityId, rtiId ) );
        }
    private:
        EntityIdentifierResolver_ABC& entityIdentifierResolver_;
    };
}
}

#endif // plugins_hla_HlaFactories_h
