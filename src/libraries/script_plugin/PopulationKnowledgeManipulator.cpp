// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "PopulationKnowledgeManipulator.h"
#include "dispatcher/PopulationKnowledge.h"
#include "dispatcher/KnowledgeGroup.h"
#include "dispatcher/Model.h"
#include "dispatcher/Side.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <directia/brain/Brain.h>

using namespace plugins::script;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeManipulator constructor
// Created: DSO 2010-09-16
// -----------------------------------------------------------------------------
PopulationKnowledgeManipulator::PopulationKnowledgeManipulator( const kernel::CoordinateConverter_ABC& converter, const dispatcher::PopulationKnowledge& knowledge )
    : converter_( converter )
    , knowledge_( knowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeManipulator destructor
// Created: DSO 2010-09-16
// -----------------------------------------------------------------------------
PopulationKnowledgeManipulator::~PopulationKnowledgeManipulator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeManipulator::Registrar::RegisterIn
// Created: DSO 2010-09-16
// -----------------------------------------------------------------------------
void PopulationKnowledgeManipulator::Registrar::RegisterIn( directia::brain::Brain& brain )
{
    brain.Register( "GetIdentifier", &PopulationKnowledgeManipulator::GetIdentifier );
//    brain.RegisterFunction( "GetPosition",   &PopulationKnowledgeManipulator::GetPosition   );
    brain.Register( "GetOwnerTeam",  &PopulationKnowledgeManipulator::GetOwnerTeam  );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeManipulator::GetIdentifier
// Created: DSO 2010-09-16
// -----------------------------------------------------------------------------
unsigned int PopulationKnowledgeManipulator::GetIdentifier() const
{
    return knowledge_.GetId();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeManipulator::GetPosition
// Created: DSO 2010-09-16
// -----------------------------------------------------------------------------
Position PopulationKnowledgeManipulator::GetPosition() const
{
    return Position();
//    return ToPosition( converter_.ConvertToXY( knowledge_.position_ ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeManipulator::GetOwnerTeam
// Created: DSO 2010-09-16
// -----------------------------------------------------------------------------
std::string PopulationKnowledgeManipulator::GetOwnerTeam() const
{
    return std::string( knowledge_.GetOwner().GetName().toStdString() );
    // return knowledge_.model_.knowledgeGroups().Get( knowledge_.knowledgeGroup_.GetId() ).team_.name_;
}


