// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
#include <tools/Resolver.h>

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeFactory::ObjectKnowledgeFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel,
                                                const kernel::Time_ABC& simulation )
    : attributesFactory_( controllers, model, staticModel, simulation )
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
    const kernel::ObjectType& type = static_.objectTypes_.tools::StringResolver< kernel::ObjectType >::Get( message.type().id() );
    ObjectKnowledge* knowledge = new ObjectKnowledge( owner, message, controllers_.controller_, model_.GetObjectResolver(),
                                                      model_.GetTeamResolver(), type );
    knowledge->Attach< kernel::Positions >( *new ObjectKnowledgePositions( static_.coordinateConverter_, *knowledge, type ) );
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
