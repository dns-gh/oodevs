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
#include "PopulationKnowledgeFactory_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"

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
// Name: PopulationKnowledges::UpdatePopulationPart
// Created: LDC 2010-11-18
// -----------------------------------------------------------------------------
template< typename T >
void PopulationKnowledges::UpdatePopulationPart( const T& message )
{
    if( PopulationKnowledge_ABC* k = Find( message.crowd().id() ) )
    {
        k->Update( message );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const sword::CrowdKnowledgeCreation& message )
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
void PopulationKnowledges::DoUpdate( const sword::CrowdKnowledgeUpdate& message )
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
void PopulationKnowledges::DoUpdate( const sword::CrowdKnowledgeDestruction& message )
{
    delete Find( message.knowledge().id() );
    Remove( message.knowledge().id() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const sword::CrowdConcentrationKnowledgeCreation& message )
{
    UpdatePopulationPart( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const sword::CrowdConcentrationKnowledgeUpdate& message )
{
    UpdatePopulationPart( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const sword::CrowdConcentrationKnowledgeDestruction& message )
{
    UpdatePopulationPart( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const sword::CrowdFlowKnowledgeCreation& message )
{
    UpdatePopulationPart( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const sword::CrowdFlowKnowledgeUpdate& message )
{
    UpdatePopulationPart( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void PopulationKnowledges::DoUpdate( const sword::CrowdFlowKnowledgeDestruction& message )
{
    UpdatePopulationPart( message );
}
