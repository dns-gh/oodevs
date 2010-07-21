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
const kernel::ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const kernel::Object_ABC& base, const kernel::Team_ABC& owner ) const
{
    tools::Iterator< const dispatcher::ObjectKnowledge_ABC& > it = model_.ObjectKnowledges().CreateIterator();
    while( it.HasMoreElements() )
    {
        const dispatcher::ObjectKnowledge_ABC& k = it.NextElement();
        if( & k.GetOwner() == &owner && k.GetEntity() == &base )
            return &k;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const kernel::Object_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( unsigned long id, const kernel::Team_ABC& /*owner*/ ) const
{
    return model_.ObjectKnowledges().Find( id );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( unsigned long id, const kernel::Entity_ABC& /*owner*/ ) const
{
    return model_.ObjectKnowledges().Find( id );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const kernel::ObjectKnowledge_ABC& /*base*/, const kernel::Team_ABC& /*owner*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}
