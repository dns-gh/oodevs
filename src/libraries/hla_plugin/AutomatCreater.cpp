// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AutomatCreater.h"
#include "ContextHandler_ABC.h"
#include "AutomatTypeResolver_ABC.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/KnowledgeGroup_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "tools/Resolver_ABC.h"
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    unsigned long ResolveAutomatType( xml::xistream& xis, const AutomatTypeResolver_ABC& automatTypeResolver )
    {
        std::string name;
        xis >> xml::start( "types" )
                >> xml::content( "automat", name );
        unsigned long type = automatTypeResolver.Resolve( name );
        if( type == 0 )
            throw std::runtime_error( "Automat type name '" + name + "' not found, please check your physical model." );
        return type;
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatCreater constructor
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
AutomatCreater::AutomatCreater( xml::xisubstream xis,
                                ContextHandler_ABC< sword::FormationCreation >& formationCreation,
                                ContextHandler_ABC< sword::AutomatCreation >& automatCreation,
                                const AutomatTypeResolver_ABC& automatTypeResolver,
                                const tools::Resolver_ABC< dispatcher::KnowledgeGroup_ABC, unsigned long >& knowledgeGroups )
    : automatType_      ( ResolveAutomatType( xis, automatTypeResolver ) )
    , formationCreation_( formationCreation )
    , automatCreation_  ( automatCreation )
{
    formationCreation_.Register( *this );
    for( tools::Iterator< const dispatcher::KnowledgeGroup_ABC& > it = knowledgeGroups.CreateIterator(); it.HasMoreElements(); )
    {
        const dispatcher::KnowledgeGroup_ABC& group = it.NextElement();
        identifiers_[ group.GetTeam().GetId() ] = group.GetId();
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatCreater destructor
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
AutomatCreater::~AutomatCreater()
{
    formationCreation_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AutomatCreater::Notify
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void AutomatCreater::Notify( const sword::FormationCreation& message, const std::string& /*identifier*/ )
{
    simulation::UnitMagicAction automatCreationMessage;
    automatCreationMessage().mutable_tasker()->mutable_formation()->set_id( message.formation().id() );            // parent
    automatCreationMessage().set_type( sword::UnitMagicAction::automat_creation );
    automatCreationMessage().mutable_parameters()->add_elem()->add_value()->set_identifier( automatType_ );        // type
    automatCreationMessage().mutable_parameters()->add_elem()->add_value()->set_identifier( identifiers_[ message.party().id() ] );      // knowledge group
    automatCreationMessage().mutable_parameters()->add_elem()->add_value()->set_acharstr( "HLA distant automat" ); // name
    automatCreation_.Send( automatCreationMessage, boost::lexical_cast< std::string >( message.formation().id() ) );
}
