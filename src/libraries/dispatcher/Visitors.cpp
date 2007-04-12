// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Visitors.h"
#include "Side.h"
#include "KnowledgeGroup.h"
#include "Formation.h"
#include "Automat.h"
#include "Agent.h"
#include "Object.h"
#include "Population.h"
#include "PopulationConcentration.h"
#include "PopulationFlow.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FullUpdateVisitor::FullUpdateVisitor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
FullUpdateVisitor::FullUpdateVisitor( Publisher_ABC& publisher )
    : publisher_( &publisher )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: FullUpdateVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void FullUpdateVisitor::Visit( Side& entity )
{
    entity.SendFullUpdate( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: FullUpdateVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void FullUpdateVisitor::Visit( KnowledgeGroup& entity )
{
    entity.SendFullUpdate( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: FullUpdateVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void FullUpdateVisitor::Visit( Formation& entity )
{
    entity.SendFullUpdate( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: FullUpdateVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void FullUpdateVisitor::Visit( Automat& entity )
{
    entity.SendFullUpdate( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: FullUpdateVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void FullUpdateVisitor::Visit( Agent& entity )
{
    entity.SendFullUpdate( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: FullUpdateVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void FullUpdateVisitor::Visit( Object& entity )
{
    entity.SendFullUpdate( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: FullUpdateVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void FullUpdateVisitor::Visit( Population& entity )
{
    entity.SendFullUpdate( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: FullUpdateVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void FullUpdateVisitor::Visit( PopulationFlow& entity )
{
    entity.SendFullUpdate( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: FullUpdateVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void FullUpdateVisitor::Visit( PopulationConcentration& entity )
{
    entity.SendFullUpdate( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: CreationVisitor::CreationVisitor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
CreationVisitor::CreationVisitor( Publisher_ABC& publisher )
    : publisher_( &publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreationVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void CreationVisitor::Visit( Side& entity )
{
    entity.SendCreation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: CreationVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void CreationVisitor::Visit( KnowledgeGroup& entity )
{
    entity.SendCreation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: CreationVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void CreationVisitor::Visit( Formation& entity )
{
    entity.SendCreation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: CreationVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void CreationVisitor::Visit( Automat& entity )
{
    entity.SendCreation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: CreationVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void CreationVisitor::Visit( Agent& entity )
{
    entity.SendCreation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: CreationVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void CreationVisitor::Visit( Object& entity )
{
    entity.SendCreation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: CreationVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void CreationVisitor::Visit( Population& entity )
{
    entity.SendCreation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: CreationVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void CreationVisitor::Visit( PopulationFlow& entity )
{
    entity.SendCreation( *publisher_ );
}
    
// -----------------------------------------------------------------------------
// Name: CreationVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void CreationVisitor::Visit( PopulationConcentration& entity )
{
    entity.SendCreation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: StartSynchVisitor::StartSynchVisitor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
StartSynchVisitor::StartSynchVisitor( Publisher_ABC& publisher )
    : publisher_( &publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StartSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void StartSynchVisitor::Visit( Side& entity )
{
    entity.StartSynchronisation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: StartSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void StartSynchVisitor::Visit( KnowledgeGroup& entity )
{
    entity.StartSynchronisation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: StartSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void StartSynchVisitor::Visit( Formation& entity )
{
    entity.StartSynchronisation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: StartSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void StartSynchVisitor::Visit( Automat& entity )
{
    entity.StartSynchronisation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: StartSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void StartSynchVisitor::Visit( Agent& entity )
{
    entity.StartSynchronisation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: StartSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void StartSynchVisitor::Visit( Object& entity )
{
    entity.StartSynchronisation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: StartSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void StartSynchVisitor::Visit( Population& entity )
{
    entity.StartSynchronisation( *publisher_ );
}


// -----------------------------------------------------------------------------
// Name: StartSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void StartSynchVisitor::Visit( PopulationConcentration& entity )
{
    entity.StartSynchronisation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: StartSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void StartSynchVisitor::Visit( PopulationFlow& entity )
{
    entity.StartSynchronisation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: EndSynchVisitor::EndSynchVisitor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
EndSynchVisitor::EndSynchVisitor( Model& model )
    : model_( &model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EndSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void EndSynchVisitor::Visit( Side& entity )
{
    entity.EndSynchronisation( *model_ );
}

// -----------------------------------------------------------------------------
// Name: EndSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void EndSynchVisitor::Visit( KnowledgeGroup& entity )
{
    entity.EndSynchronisation( *model_ );
}

// -----------------------------------------------------------------------------
// Name: EndSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void EndSynchVisitor::Visit( Formation& entity )
{
    entity.EndSynchronisation( *model_ );
}

// -----------------------------------------------------------------------------
// Name: EndSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void EndSynchVisitor::Visit( Automat& entity )
{
    entity.EndSynchronisation( *model_ );
}

// -----------------------------------------------------------------------------
// Name: EndSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void EndSynchVisitor::Visit( Agent& entity )
{
    entity.EndSynchronisation( *model_ );
}

// -----------------------------------------------------------------------------
// Name: EndSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void EndSynchVisitor::Visit( Object& entity )
{
    entity.EndSynchronisation( *model_ );
}

// -----------------------------------------------------------------------------
// Name: EndSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void EndSynchVisitor::Visit( Population& entity )
{
    entity.EndSynchronisation( *model_ );
}

// -----------------------------------------------------------------------------
// Name: EndSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void EndSynchVisitor::Visit( PopulationConcentration& entity )
{
    entity.EndSynchronisation( *model_ );
}

// -----------------------------------------------------------------------------
// Name: EndSynchVisitor::Visit
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void EndSynchVisitor::Visit( PopulationFlow& entity )
{
    entity.EndSynchronisation( *model_ );
}

