// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "FederateFacade.h"
#include "AggregateEntityClass.h"
#include "RtiAmbassadorFactory_ABC.h"
#include "FederateAmbassadorFactory_ABC.h"
#include <hla/hla_lib.h>
#include <hla/SimpleTimeFactory.h>
#include <hla/SimpleTimeIntervalFactory.h>
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    std::auto_ptr< hla::Federate > CreateFederate( xml::xisubstream xis, hla::RtiAmbassador_ABC& ambassador, const FederateAmbassadorFactory_ABC& factory )
    {
        std::auto_ptr< hla::Federate > federate = factory.Create( ambassador, xis.attribute< std::string >( "name", "SWORD" ), xis.attribute< int >( "lookahead", -1 ) );
        if( !federate->Connect() )
            throw std::runtime_error( "Could not connect to '" + xis.attribute< std::string >( "host", "localhost" ) + ":" + xis.attribute< std::string >( "port", "8989" ) + "'" );
        const std::string name = xis.attribute< std::string >( "federation", "Federation" );
        const bool joined = federate->Join( name, xis.attribute< bool >( "time-constrained", true ), xis.attribute< bool >( "time-regulating", true ) );
        if( !joined )
        {
            if( xis.attribute< bool >( "creation", false ) )
            {
                if( !federate->Create( name, xis.attribute< std::string >( "fom", "ASI_FOM_v2.0.8_2010.xml" ) ) )
                    throw std::runtime_error( "Could not create the federation '" + name + "'" );
                if( !federate->Join( name, xis.attribute< bool >( "time-constrained", true ), xis.attribute< bool >( "time-regulating", true ) ) )
                    throw std::runtime_error( "Could not join the federation '" + name + "'" );
            }
            else
                throw std::runtime_error( "Could not join the federation '" + name + "'" );
        }
        return federate;
    }
}

struct FederateFacade::FederationDestructor : private boost::noncopyable
{
public:
    FederationDestructor( ::hla::Federate& federate, const std::string& federation )
        : federate_  ( federate )
        , federation_( federation )
    {
        // NOTHING
    }
    virtual ~FederationDestructor()
    {
        try
        {
            if( !federate_.Destroy( federation_ ) )
                throw std::runtime_error( "Could not destroy the federation '" + federation_ + "'" );
        }
        catch( ... )
        {
            // NOTHING
        }
    }
private:
    ::hla::Federate& federate_;
    const std::string federation_;
};

// -----------------------------------------------------------------------------
// Name: FederateFacade constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::FederateFacade( xml::xisubstream xis, AgentSubject_ABC& subject, const RtiAmbassadorFactory_ABC& rtiFactory, const FederateAmbassadorFactory_ABC& federateFactory )
    : timeFactory_    ( new ::hla::SimpleTimeFactory() )
    , intervalFactory_( new ::hla::SimpleTimeIntervalFactory() )
    , ambassador_     ( rtiFactory.CreateAmbassador( *timeFactory_, *intervalFactory_, ::hla::RtiAmbassador_ABC::TimeStampOrder, xis.attribute< std::string >( "host", "localhost" ), xis.attribute< std::string >( "port", "8989" ) ) )
    , federate_       ( CreateFederate( xis, *ambassador_, federateFactory ) )
    , destructor_     ( xis.attribute< bool >( "destruction", false ) ? new FederateFacade::FederationDestructor( *federate_, xis.attribute< std::string >( "federation", "Federation" ) ) : 0 )
    , agentClass_     ( new AggregateEntityClass( *federate_, subject ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FederateFacade destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::~FederateFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Step
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void FederateFacade::Step()
{
    federate_->Step();
}
