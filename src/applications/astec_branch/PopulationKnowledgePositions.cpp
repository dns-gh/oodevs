// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationKnowledgePositions.h"
#include "PopulationKnowledge.h"
#include "Population.h"

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
PopulationKnowledgePositions::PopulationKnowledgePositions( const PopulationKnowledge& k )
    : knowledge_( k )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions destructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
PopulationKnowledgePositions::~PopulationKnowledgePositions()
{
    // NOTHING
}

// $$$$ AGE 2006-05-18: pas terribule

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions::GetPosition
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
geometry::Point2f PopulationKnowledgePositions::GetPosition() const
{
    return knowledge_.GetRealPopulation().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions::GetHeight
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
float PopulationKnowledgePositions::GetHeight() const
{
    return knowledge_.GetRealPopulation().GetHeight();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions::IsAt
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
bool PopulationKnowledgePositions::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
{
    return knowledge_.GetRealPopulation().IsAt( pos, precision );
}   

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions::IsIn
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
bool PopulationKnowledgePositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return knowledge_.GetRealPopulation().IsIn( rectangle );
}
