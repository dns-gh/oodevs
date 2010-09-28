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
    if( PopulationKnowledge_ABC* k = Find( message.knowledge().id() ) )
    {
        k->Update( message );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgCrowdKnowledgeCreation& message )
{
    if( ! Find( message.knowledge().id() ) )
    {
        PopulationKnowledge_ABC* knowledge = factory_.CreatePopulationKnowledge( owner_, message );
        Register( message.knowledge().id(), *knowledge );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgCrowdKnowledgeUpdate& message )
{
    if( PopulationKnowledge_ABC* k = Find( message.knowledge().id() ) )
    {
        k->Update( message );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgCrowdKnowledgeDestruction& message )
{
    delete Find( message.knowledge().id() );
    Remove( message.knowledge().id() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeCreation& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeUpdate& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeDestruction& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgCrowdFlowKnowledgeCreation& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgCrowdFlowKnowledgeUpdate& message )
{
    UpdatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const MsgsSimToClient::MsgCrowdFlowKnowledgeDestruction& message )
{
    UpdatePopulation( message );
}
