// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationKnowledges.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/Controller.h"
#include "PopulationKnowledgeFactory_ABC.h"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
PopulationKnowledges::PopulationKnowledges( Controller& controller, const KnowledgeGroup_ABC& owner, PopulationKnowledgeFactory_ABC& factory )
    : Creatable< PopulationKnowledges >( controller, this )
    , owner_( owner )
    , controller_( controller )
    , factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
PopulationKnowledges::~PopulationKnowledges()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::UpdatePopulation
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void PopulationKnowledges::UpdatePopulation( const T& message )
{
    if( PopulationKnowledge_ABC* k = Find( message.population().id() ) )
    {
        k->Update( message );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgPopulationKnowledgeCreation& message )
{
    if( ! Find( message.id().id() ) )
    {
        PopulationKnowledge_ABC* knowledge = factory_.CreatePopulationKnowledge( owner_, message );
        Register( message.id().id(), *knowledge );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgPopulationKnowledgeUpdate& message )
{
    if( PopulationKnowledge_ABC* k = Find( message.id().id() ) )
    {
        k->Update( message );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgPopulationKnowledgeDestruction& message )
{
    delete Find( message.id().id() );
    Remove( message.id().id() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeCreation& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeUpdate& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeDestruction& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgPopulationFlowKnowledgeCreation& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgPopulationFlowKnowledgeUpdate& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgPopulationFlowKnowledgeDestruction& message )
{
    UpdatePopulation( message );
}
