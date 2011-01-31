// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UrbanKnowledgeFactory.h"
#include "UrbanKnowledge.h"
#include "Model.h"
#include "AgentsModel.h"
#include "UrbanPerceptions.h"
#include "UrbanModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeFactory constructor
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
UrbanKnowledgeFactory::UrbanKnowledgeFactory( Controller& controller, Model& model )
    : controller_( controller )
    , model_     ( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeFactory destructor
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
UrbanKnowledgeFactory::~UrbanKnowledgeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeFactory::Create
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
UrbanKnowledge_ABC* UrbanKnowledgeFactory::Create( const Team_ABC& owner, const sword::UrbanKnowledgeCreation& message )
{
    // $$$ FDS 2009-01-11: remove public blocks, use knowledgeConverter if possible
    UrbanKnowledge* knowledge = new UrbanKnowledge( owner, message, controller_, model_.urbanObjects_ );
    knowledge->Attach( *new UrbanPerceptions( controller_, model_.agents_ ) );
    knowledge->Polish();
    return knowledge;
}
