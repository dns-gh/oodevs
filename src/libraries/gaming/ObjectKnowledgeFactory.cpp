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

#include "ConstructionAttribute.h"
#include "BypassAttribute.h"
#include "ObstacleAttribute.h"
#include "MineAttribute.h"
#include "SupplyRouteAttribute.h"
#include "CrossingSiteAttribute.h"
#include "LogisticAttribute.h"
#include "ActivityTimeAttribute.h"
#include "NBCAttribute.h"
#include "FireAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "ObjectKnowledgePositions.h"
#include "ObjectPerceptions.h"
#include "StaticModel.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeFactory::ObjectKnowledgeFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
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
kernel::ObjectKnowledge_ABC* ObjectKnowledgeFactory::Create( const kernel::Entity_ABC& owner, const MsgsSimToClient::MsgObjectKnowledgeCreation& message )
{
    ObjectKnowledge* knowledge = new ObjectKnowledge( owner, message, controllers_.controller_, static_.coordinateConverter_, model_.objects_, static_.objectTypes_ );
    knowledge->Attach< kernel::Positions >( *new ObjectKnowledgePositions( static_.coordinateConverter_, *knowledge ) );
    knowledge->Attach( *new ObjectPerceptions( controllers_.controller_, model_.agents_ ) );
    Register( *knowledge, message.attributes() );
    knowledge->Polish();
    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory::Register
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObjectKnowledgeFactory::Register( kernel::ObjectKnowledge_ABC& knowledge, const Common::MsgObjectAttributes& attributes ) const
{
    if( attributes.has_logistic() && knowledge.Retrieve< kernel::LogisticAttribute_ABC >() == 0 )
        knowledge.Attach< kernel::LogisticAttribute_ABC >( *new LogisticAttribute( controllers_.controller_, model_.agents_ ) );

    if( attributes.has_construction() && knowledge.Retrieve< kernel::ConstructionAttribute_ABC >() == 0 )
        knowledge.Attach< kernel::ConstructionAttribute_ABC >( *new ConstructionAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_mine() && knowledge.Retrieve< kernel::MineAttribute_ABC >() == 0 )
        knowledge.Attach< kernel::MineAttribute_ABC >( *new MineAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_bypass() && knowledge.Retrieve< kernel::BypassAttribute_ABC >() == 0 )
        knowledge.Attach< kernel::BypassAttribute_ABC >( *new BypassAttribute( controllers_.controller_ ) );

    if( attributes.has_obstacle() && knowledge.Retrieve< kernel::ObstacleAttribute_ABC >() == 0 )
        knowledge.Attach< kernel::ObstacleAttribute_ABC >( *new ObstacleAttribute( controllers_.controller_ ) );

    if( attributes.has_activity_time() && knowledge.Retrieve< kernel::ActivityTimeAttribute_ABC >() == 0 )
        knowledge.Attach< kernel::ActivityTimeAttribute_ABC >( *new ActivityTimeAttribute( controllers_.controller_ ) );

    if( attributes.has_crossing_site() && knowledge.Retrieve< kernel::CrossingSiteAttribute_ABC >() == 0 )
        knowledge.Attach< kernel::CrossingSiteAttribute_ABC >( *new CrossingSiteAttribute( controllers_.controller_ ) );

    if( attributes.has_supply_route() && knowledge.Retrieve< kernel::SupplyRouteAttribute_ABC >() == 0 )
        knowledge.Attach< kernel::SupplyRouteAttribute_ABC >( *new SupplyRouteAttribute( controllers_.controller_ ) );

    if( attributes.has_nbc() && knowledge.Retrieve< kernel::NBCAttribute_ABC >() == 0 )
        knowledge.Attach< kernel::NBCAttribute_ABC >( *new NBCAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_fire() && knowledge.Retrieve< kernel::FireAttribute_ABC >() == 0 )
        knowledge.Attach< kernel::FireAttribute_ABC >( *new FireAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_medical_treatment() && knowledge.Retrieve< kernel::MedicalTreatmentAttribute_ABC >() == 0 )
        knowledge.Attach< kernel::MedicalTreatmentAttribute_ABC >( *new MedicalTreatmentAttribute( controllers_.controller_, static_.objectTypes_ ) );
}
