// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ClassBuilders_h
#define plugins_hla_ClassBuilders_h

#include "ClassBuilder_ABC.h"
#include "Federate_ABC.h"
#include "HlaObject_ABC.h"
#include "Federate_ABC.h"
#include <hla/Class.h>
#include <hla/AttributeIdentifier.h>
#include <hla/ClassIdentifier.h>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

namespace plugins
{
namespace hla
{
    class EmptyFederate : public Federate_ABC
    {
        virtual bool Connect() { return true; }
        virtual void Disconnect() {}
        virtual bool Create( const std::string&, const std::string& ) { return true; }
        virtual bool Destroy( const std::string& ) { return true; }
        virtual bool Join( const std::string&, bool, bool ) { return true; }
        virtual void Resign() {}
        virtual void Step() {}
        virtual void Register( const ::hla::ClassIdentifier&, ::hla::Class_ABC&, bool, bool ) {}
        virtual void Register( const ::hla::InteractionIdentifier&, ::hla::Interaction_ABC&, bool, bool ) {}
        virtual void Register( ::hla::FederateAmbassador_ABC& ) {}
    };
    class ClassBuilder : public ClassBuilder_ABC
    {
    protected:
                 ClassBuilder( const std::string& name, const std::vector< std::string >& attributes )
                     : name_      ( name )
                     , attributes_( attributes.begin(), attributes.end() )
                 {}
        virtual ~ClassBuilder() {}

    public:
        virtual void Build( Federate_ABC& federate, ::hla::Class< HlaObject_ABC >& hlaClass, bool publish, bool subscribe ) const
        {
            BOOST_FOREACH( const std::string& attribute, attributes_ )
                hlaClass.Register( ::hla::AttributeIdentifier( attribute ) );
            hlaClass.ActivateUpdates( true );
            federate.Register( ::hla::ClassIdentifier( name_ ), hlaClass, publish, subscribe );
        }
    private:
        const std::string name_;
        const std::vector< std::string > attributes_;
    };
    class NetnClassBuilder : public ClassBuilder
    {
    protected:
                 NetnClassBuilder( const std::string& name, const std::vector< std::string >& attributes, std::auto_ptr< ClassBuilder_ABC > builder )
                     : ClassBuilder( name, attributes )
                     , builder_( builder )
                 {}
        virtual ~NetnClassBuilder() {}
    public:
        virtual void Build( Federate_ABC& federate, ::hla::Class< HlaObject_ABC >& hlaClass, bool publish, bool subscribe ) const
        {
            EmptyFederate empty;
            builder_->Build( empty, hlaClass, publish, subscribe );
            ClassBuilder::Build( federate, hlaClass, publish, subscribe );
        }
    private:
        std::auto_ptr< ClassBuilder_ABC > builder_;
    };

    class AggregateEntityBuilder : public ClassBuilder
    {
    public:
        AggregateEntityBuilder()
            : ClassBuilder( "BaseEntity.AggregateEntity"
            , boost::assign::list_of( "EntityType" )
                                    ( "EntityIdentifier" )
                                    ( "ForceIdentifier" )
                                    ( "AggregateMarking" )
                                    ( "AggregateState" )
                                    ( "Dimensions" )
                                    ( "Spatial" )
                                    ( "Formation" )
                                    ( "NumberOfSilentEntities" )
                                    ( "SilentEntities" )
                                    ( "SilentAggregates" )
                                    ( "SubAggregateIdentifiers" )
                                    ( "EntityIdentifiers" ) )
        {}
    };
    class NetnAggregateEntityBuilder : public NetnClassBuilder
    {
    public:
        NetnAggregateEntityBuilder()
            : NetnClassBuilder( "BaseEntity.AggregateEntity.NETN_Aggregate"
            , boost::assign::list_of( "Mounted" )
                                    ( "Echelon" )
                                    ( "UniqueID" )
                                    ( "HigherHeadquarters" )
                                    ( "Callsign" )
                                    ( "Status" )
            , std::auto_ptr< ClassBuilder_ABC >( new AggregateEntityBuilder() ) )
        {}
    };
    class SurfaceVesselBuilder : public ClassBuilder
    {
    public:
        SurfaceVesselBuilder()
            : ClassBuilder( "BaseEntity.PhysicalEntity.Platform.SurfaceVessel"
            , boost::assign::list_of( "EntityType" )
                                    ( "EntityIdentifier" )
                                    ( "ForceIdentifier" )
                                    ( "Marking" )
                                    ( "Spatial" ) )
        {}
    };
    class NetnSurfaceVesselBuilder : public NetnClassBuilder
    {
    public:
        NetnSurfaceVesselBuilder()
            : NetnClassBuilder( "BaseEntity.PhysicalEntity.Platform.SurfaceVessel.NETN_SurfaceVessel"
            , boost::assign::list_of( "UniqueID" )
                                    ( "Callsign" )
            , std::auto_ptr< ClassBuilder_ABC >( new SurfaceVesselBuilder() ) )
        {}
    };
}
}

#endif // plugins_hla_ClassBuilders_h
