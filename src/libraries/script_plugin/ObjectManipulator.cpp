// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ObjectManipulator.h"
#include "clients_kernel/ObjectType.h"
#include "dispatcher/Object_ABC.h"
#include <directia/brain/Brain.h>

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ObjectManipulator constructor
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
ObjectManipulator::ObjectManipulator( const dispatcher::Object_ABC& entity )
    : entity_( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectManipulator destructor
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
ObjectManipulator::~ObjectManipulator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectManipulator::Registrar::RegisterIn
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
void ObjectManipulator::Registrar::RegisterIn( directia::brain::Brain& brain )
{
    brain.Register( "GetIdentifier", &ObjectManipulator::GetIdentifier );
    brain.Register( "GetName", &ObjectManipulator::GetName );
    brain.Register( "GetType", &ObjectManipulator::GetType );
}

// -----------------------------------------------------------------------------
// Name: ObjectManipulator::GetIdentifier
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
unsigned int ObjectManipulator::GetIdentifier() const
{
    return entity_.GetId();
}

// -----------------------------------------------------------------------------
// Name: ObjectManipulator::GetName
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
std::string ObjectManipulator::GetName() const
{
    return entity_.GetName().toStdString();
}

// -----------------------------------------------------------------------------
// Name: ObjectManipulator::GetType
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
std::string ObjectManipulator::GetType() const
{
    return entity_.GetType().GetName();
}
