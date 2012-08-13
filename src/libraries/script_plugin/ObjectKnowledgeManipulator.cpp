// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ObjectKnowledgeManipulator.h"
#include "dispatcher/ObjectKnowledge.h"
#include "dispatcher/KnowledgeGroup.h"
#include "dispatcher/Model.h"
#include "dispatcher/Side.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <directia/brain/Brain.h>

using namespace plugins::script;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeManipulator constructor
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
ObjectKnowledgeManipulator::ObjectKnowledgeManipulator( const kernel::CoordinateConverter_ABC& converter, const dispatcher::ObjectKnowledge& knowledge )
    : converter_( converter )
    , knowledge_( knowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeManipulator destructor
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
ObjectKnowledgeManipulator::~ObjectKnowledgeManipulator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeManipulator::Registrar::RegisterIn
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
void ObjectKnowledgeManipulator::Registrar::RegisterIn( directia::brain::Brain& brain )
{
    brain.Register( "GetIdentifier", &ObjectKnowledgeManipulator::GetIdentifier );
//    brain.Register( "GetPosition",   &ObjectKnowledgeManipulator::GetPosition   );
    brain.Register( "GetOwnerTeam",  &ObjectKnowledgeManipulator::GetOwnerTeam  );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeManipulator::GetIdentifier
// Created: AGE 2008-06-18
// -----------------------------------------------------------------------------
unsigned int ObjectKnowledgeManipulator::GetIdentifier() const
{
    return knowledge_.GetId();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeManipulator::GetPosition
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
Position ObjectKnowledgeManipulator::GetPosition() const
{
    return Position();
//    return ToPosition( converter_.ConvertToXY( knowledge_.position_ ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeManipulator::GetOwnerTeam
// Created: AGE 2008-08-01
// -----------------------------------------------------------------------------
std::string ObjectKnowledgeManipulator::GetOwnerTeam() const
{
    return std::string( knowledge_.GetOwner().GetName().toStdString() );
    // return knowledge_.model_.sides().Get( knowledge_.team_.GetId() ).name_;
}


