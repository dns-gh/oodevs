// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "UrbanKnowledgeConverter.h"
#include "UrbanKnowledge_ABC.h"
#include "Model_ABC.h"
#include "Agent_ABC.h"
#include "Automat_ABC.h"
#include "Team_ABC.h"
#include "tools/Resolver.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeConverter constructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
UrbanKnowledgeConverter::UrbanKnowledgeConverter( const dispatcher::Model_ABC& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeConverter destructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
UrbanKnowledgeConverter::~UrbanKnowledgeConverter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::UrbanKnowledge_ABC* UrbanKnowledgeConverter::Find( unsigned long id, const kernel::Team_ABC& /*owner*/ ) const
{
    return model_.UrbanKnowledges().Find( id );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::UrbanKnowledge_ABC* UrbanKnowledgeConverter::Find( unsigned long id, const kernel::Entity_ABC& /*owner*/ ) const
{
    return model_.UrbanKnowledges().Find( id );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
const kernel::UrbanKnowledge_ABC* UrbanKnowledgeConverter::Find( const kernel::Entity_ABC& /*base*/, const kernel::Team_ABC& /*owner*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}