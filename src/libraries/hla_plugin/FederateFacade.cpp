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
#include "Federate_ABC.h"
#include "FederateAmbassadorFactory_ABC.h"
#include "RtiAmbassadorFactory_ABC.h"
#include "AggregateFactory.h"
#include "NetnAggregateFactory.h"
#include "ClassBuilder.h"
#include "NetnClassBuilder.h"
#include "protocol/Simulation.h"
#include <hla/SimpleTimeFactory.h>
#include <hla/SimpleTimeIntervalFactory.h>
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    std::auto_ptr< Federate_ABC > CreateFederate( xml::xisubstream xis, hla::RtiAmbassador_ABC& ambassador, const FederateAmbassadorFactory_ABC& factory, const std::string& pluginDirectory )
    {
        std::auto_ptr< Federate_ABC > federate = factory.Create( ambassador, xis.attribute< std::string >( "name", "SWORD" ), xis.attribute< int >( "lookahead", -1 ) );
        if( !federate->Connect() )
            throw std::runtime_error( "Could not connect to '" + xis.attribute< std::string >( "host", "localhost" ) + ":" + xis.attribute< std::string >( "port", "8989" ) + "'" );
        const std::string name = xis.attribute< std::string >( "federation", "Federation" );
        const bool joined = federate->Join( name, xis.attribute< bool >( "time-constrained", true ), xis.attribute< bool >( "time-regulating", true ) );
        if( !joined )
        {
            if( xis.attribute< bool >( "creation", false ) )
            {
                const std::string fom = xis.attribute< std::string >( "fom", "ASI_FOM_v2.0.8_2010.xml" );
                if( !federate->Create( name, pluginDirectory + "/" + fom ) )
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
    FederationDestructor( Federate_ABC& federate, const std::string& federation )
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
    Federate_ABC& federate_;
    const std::string federation_;
};

// -----------------------------------------------------------------------------
// Name: FederateFacade constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::FederateFacade( xml::xisubstream xis, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                AgentSubject_ABC& subject, const RtiAmbassadorFactory_ABC& rtiFactory,
                                const FederateAmbassadorFactory_ABC& federateFactory, const std::string& pluginDirectory )
    : timeFactory_          ( new ::hla::SimpleTimeFactory() )
    , intervalFactory_      ( new ::hla::SimpleTimeIntervalFactory() )
    , ambassador_           ( rtiFactory.CreateAmbassador( *timeFactory_, *intervalFactory_, ::hla::RtiAmbassador_ABC::TimeStampOrder, xis.attribute< std::string >( "host", "localhost" ), xis.attribute< std::string >( "port", "8989" ) ) )
    , federate_             ( CreateFederate( xis, *ambassador_, federateFactory, pluginDirectory ) )
    , destructor_           ( xis.attribute< bool >( "destruction", false ) ? new FederateFacade::FederationDestructor( *federate_, xis.attribute< std::string >( "federation", "Federation" ) ) : 0 )
    , pAggregateFactory_    ( new AggregateFactory() )
    , pNetnAggregateFactory_( new NetnAggregateFactory( *pAggregateFactory_ ) )
    , pClassBuilder_        ( new ClassBuilder() )
    , pNetnClassBuilder_    ( new NetnClassBuilder( *pClassBuilder_ ) )
    , agentClass_           ( new AggregateEntityClass( *federate_, subject,
                                                        xis.attribute< bool >( "netn", true ) ? *pNetnAggregateFactory_ : *pAggregateFactory_,
                                                        xis.attribute< bool >( "netn", true ) ? *pNetnClassBuilder_ : *pClassBuilder_ ) )
{
    CONNECT( controller, *this, control_end_tick );
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
// Name: FederateFacade::Notify
// Created: SLI 2011-06-24
// -----------------------------------------------------------------------------
void FederateFacade::Notify( const sword::ControlEndTick& /*message*/ )
{
    federate_->Step();
}
