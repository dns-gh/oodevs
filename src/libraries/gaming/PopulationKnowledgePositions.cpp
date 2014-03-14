// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationKnowledgePositions.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/Population_ABC.h"

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
PopulationKnowledgePositions::PopulationKnowledgePositions( const kernel::PopulationKnowledge_ABC& k )
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
// Name: PopulationKnowledgePositions::GetRealPosition
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
const kernel::Positions& PopulationKnowledgePositions::GetRealPosition() const
{
    // $$$$ AGE 2006-08-07: pas terribule atole
    return knowledge_.GetEntity()->Get< Positions >();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions::GetPosition
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
geometry::Point2f PopulationKnowledgePositions::GetPosition( bool aggregated ) const
{
    return GetRealPosition().GetPosition( aggregated );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions::GetHeight
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
float PopulationKnowledgePositions::GetHeight( bool aggregated ) const
{
    return GetRealPosition().GetHeight( aggregated );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions::IsIn
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
bool PopulationKnowledgePositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return GetRealPosition().IsIn( rectangle );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f PopulationKnowledgePositions::GetBoundingBox() const
{
    return GetRealPosition().GetBoundingBox();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void PopulationKnowledgePositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    GetRealPosition().Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool PopulationKnowledgePositions::CanAggregate() const
{
    return false;
}
