// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "KnowledgeManipulator.h"
#include "directia/Brain.h"
#include "dispatcher/AgentKnowledge.h"
#include "dispatcher/KnowledgeGroup.h"
#include "dispatcher/Side.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

using namespace script;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: KnowledgeManipulator constructor
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
KnowledgeManipulator::KnowledgeManipulator( const kernel::CoordinateConverter_ABC& converter, const dispatcher::AgentKnowledge& knowledge )
    : converter_( converter )
    , knowledge_( knowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeManipulator destructor
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
KnowledgeManipulator::~KnowledgeManipulator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeManipulator::Registrar::RegisterIn
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
void KnowledgeManipulator::Registrar::RegisterIn( directia::Brain& brain )
{
    brain.RegisterFunction( "GetIdentifier", &KnowledgeManipulator::GetIdentifier );
    brain.RegisterFunction( "GetPosition",   &KnowledgeManipulator::GetPosition   );
    brain.RegisterFunction( "GetOwnerTeam",  &KnowledgeManipulator::GetOwnerTeam  );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeManipulator::GetIdentifier
// Created: AGE 2008-06-18
// -----------------------------------------------------------------------------
unsigned int KnowledgeManipulator::GetIdentifier() const
{
    return knowledge_.GetId();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeManipulator::GetPosition
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
Position KnowledgeManipulator::GetPosition() const
{
    return ToPosition( converter_.ConvertToXY( knowledge_.position_ ) );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeManipulator::GetOwnerTeam
// Created: AGE 2008-08-01
// -----------------------------------------------------------------------------
std::string KnowledgeManipulator::GetOwnerTeam() const
{
    return knowledge_.knowledgeGroup_.team_.name_;
}


