// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ObjectKnowledgeConverter.h"
#include "Model.h"
#include "ObjectKnowledge.h"
#include "tools/Resolver.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter constructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
ObjectKnowledgeConverter::ObjectKnowledgeConverter( const dispatcher::Model& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter destructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
ObjectKnowledgeConverter::~ObjectKnowledgeConverter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const kernel::Object_ABC& base,  const kernel::Team_ABC& owner ) const
{
    tools::Iterator< const dispatcher::ObjectKnowledge& > it = model_.objectKnowledges_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const dispatcher::ObjectKnowledge& k = it.NextElement();
        if( & k.team_ == &owner && k.pObject_ == &base )
            return &k;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( unsigned long /*id*/, const kernel::Team_ABC& /*owner*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const kernel::ObjectKnowledge_ABC& /*base*/, const kernel::Team_ABC& /*owner*/ ) const
{
    return 0;
}
