// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "RcEntityResolver.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "dispatcher/Model.h"
#include "dispatcher/Agent.h"
#include "dispatcher/AgentKnowledge.h"
#include "dispatcher/ObjectKnowledge.h"
#include "dispatcher/PopulationKnowledge.h"

using namespace plugins::logger;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: RcEntityResolver constructor
// Created: LDC 2010-03-18
// -----------------------------------------------------------------------------
RcEntityResolver::RcEntityResolver( const dispatcher::Model& model )
: model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver destructor
// Created: LDC 2010-03-18
// -----------------------------------------------------------------------------
RcEntityResolver::~RcEntityResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RcEntityResolver::CreateLink
// Created: LDC 2010-03-18
// -----------------------------------------------------------------------------
QString RcEntityResolver::CreateLink( const QString& type, unsigned long id ) const
{
    Entity_ABC* entity = 0;
    if( type == Agent_ABC::typeName_ )
        entity = model_.Agents().Find( id );
    else if( type == ObjectKnowledge_ABC::typeName_ )
        entity = model_.AgentKnowledges().Find( id );
    else if( type == AgentKnowledge_ABC::typeName_ )
        entity = model_.ObjectKnowledges().Find( id );
    else if( type == PopulationKnowledge_ABC::typeName_ )
        entity = model_.PopulationKnowledges().Find( id );
    else
        return QString::number( id );
    if( !entity )
        return "Unknown entity";
    std::stringstream stream;
    stream << entity->GetName() << " [" << entity->GetId() << "] ";
    return stream.str().c_str();
}
