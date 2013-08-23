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
#include "Model_ABC.h"
#include "ObjectKnowledge.h"
#include "Agent_ABC.h"
#include "Automat_ABC.h"
#include "Population_ABC.h"
#include "Team_ABC.h"
#include "tools/Resolver.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter constructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
ObjectKnowledgeConverter::ObjectKnowledgeConverter( const dispatcher::Model_ABC& model )
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
const kernel::ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const kernel::Object_ABC& base, const kernel::Entity_ABC& owner ) const
{
    const kernel::Entity_ABC* team = 0;
    if( const dispatcher::Agent_ABC* agent = dynamic_cast< const dispatcher::Agent_ABC* >( &owner ) )
        team = &agent->GetSuperior().GetTeam();
    else if( const dispatcher::Automat_ABC* automat = dynamic_cast< const dispatcher::Automat_ABC* >( &owner ) )
        team = &automat->GetTeam();
    else if( const dispatcher::Population_ABC* population = dynamic_cast< const dispatcher::Population_ABC* >( &owner ) )
        team = &population->GetTeam();
    else
        team = dynamic_cast< const kernel::Team_ABC* >( &owner );
    if( team )
    {
        tools::Iterator< const dispatcher::ObjectKnowledge_ABC& > it = model_.ObjectKnowledges().CreateIterator();
        while( it.HasMoreElements() )
        {
            const dispatcher::ObjectKnowledge_ABC& k = it.NextElement();
            if( & k.GetOwner() == team && k.GetEntity() == &base )
                return &k;
        }
    }
    return 0;
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
const kernel::ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const kernel::ObjectKnowledge_ABC& /*base*/, const kernel::Entity_ABC& /*owner*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}
