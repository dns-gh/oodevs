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
#include "clients_kernel/Controllers.h"
#include "AgentsModel.h"
#include "UrbanPerceptions.h"
#include "UrbanModel.h"
#include "StaticModel.h"
#include "urban/TerrainObject_ABC.h"
#include "urban/Block.h"
#include "urban/Model.h"
#include "urban/BlockModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeFactory constructor
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
UrbanKnowledgeFactory::UrbanKnowledgeFactory( Controllers& controllers, Model& model, const StaticModel& staticModel )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
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
UrbanKnowledge_ABC* UrbanKnowledgeFactory::Create( const Team_ABC& owner, const ASN1T_MsgUrbanKnowledgeCreation& message )
{
    UrbanKnowledge* knowledge = new UrbanKnowledge( owner, message, controllers_.controller_, model_.urbanObjects_.model_.blocks_ );
    knowledge->Attach( *new UrbanPerceptions( controllers_.controller_, model_.agents_ ) );
    knowledge->Polish();
    return knowledge;
}
