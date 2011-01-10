// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectKnowledgeFactory.h"
#include "ObjectKnowledge.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "TeamsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectTypes.h"
#include "ObjectKnowledgePositions.h"
#include "ObjectPerceptions.h"
#include "StaticModel.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeFactory::ObjectKnowledgeFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel )
    : attributesFactory_( controllers, model, staticModel )
    , controllers_      ( controllers )
    , model_            ( model )
    , static_           ( staticModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeFactory::~ObjectKnowledgeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory::Create
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
kernel::ObjectKnowledge_ABC* ObjectKnowledgeFactory::Create( const kernel::Entity_ABC& owner, const sword::ObjectKnowledgeCreation& message )
{
    ObjectKnowledge* knowledge = new ObjectKnowledge( owner, message, controllers_.controller_, static_.coordinateConverter_, model_.objects_, model_.urbanObjects_, static_.objectTypes_ );
    knowledge->Attach< kernel::Positions >( *new ObjectKnowledgePositions( static_.coordinateConverter_, *knowledge ) );
    knowledge->Attach( *new ObjectPerceptions( controllers_.controller_, model_.agents_ ) );
    knowledge->Polish();
    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory::RegisterAttributes
// Created: NLD 2010-11-02
// -----------------------------------------------------------------------------
void ObjectKnowledgeFactory::RegisterAttributes( kernel::ObjectKnowledge_ABC& result, const sword::ObjectAttributes& attributes ) const
{
    attributesFactory_.Register( result, attributes );
}
