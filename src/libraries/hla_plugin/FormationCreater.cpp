// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "FormationCreater.h"
#include "ContextHandler_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "tools/Resolver_ABC.h"
#include "protocol/SimulationSenders.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: FormationCreater constructor
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
FormationCreater::FormationCreater( tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                                    const tools::Resolver_ABC< dispatcher::Team_ABC >& sides,
                                    ContextHandler_ABC< sword::FormationCreation >& contextHandler )
    : messageController_( messageController )
    , contextHandler_   ( contextHandler )
{
    CONNECT( messageController, *this, control_end_tick );
    for( tools::Iterator< const dispatcher::Team_ABC& > it = sides.CreateIterator(); it.HasMoreElements(); )
        sides_.push_back( it.NextElement().GetId() );
}

// -----------------------------------------------------------------------------
// Name: FormationCreater destructor
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
FormationCreater::~FormationCreater()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationCreater::Notify
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
void FormationCreater::Notify( const sword::ControlEndTick& /*message*/, int /*context*/ )
{
    DISCONNECT( messageController_, *this, control_end_tick );
    BOOST_FOREACH( unsigned long side, sides_ )
    {
        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_party()->set_id( side );
        message().set_type( sword::UnitMagicAction::formation_creation );
        message().mutable_parameters()->add_elem()->add_value()->set_areal( 6 );                          // hierarchy level
        message().mutable_parameters()->add_elem()->add_value()->set_acharstr( "HLA distant formation" ); // name
        message().mutable_parameters()->add_elem()->set_null_value( true );                               // logistic level
        contextHandler_.Send( message, boost::lexical_cast< std::string >( side ) );
    }
}
