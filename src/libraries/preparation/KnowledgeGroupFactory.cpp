// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "EntityCommunications.h"
#include "KnowledgeGroup.h"
#include "KnowledgeGroupCommunications.h"
#include "KnowledgeGroupFactory.h"
#include "KnowledgeGroupsModel.h"
#include "StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/KnowledgeGroupType.h"
#include "clients_kernel/Team_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: FormationFactory constructor
// Created: FHD 2009-11-19
// -----------------------------------------------------------------------------
KnowledgeGroupFactory::KnowledgeGroupFactory( kernel::Controllers& controllers, const StaticModel& staticModel, IdManager& idManager )
    : controllers_( controllers )
    , idManager_( idManager )
    , staticModel_( staticModel )
{
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::~KnowledgeGroupFactory
// Created: FHD 2009-11-19
// -----------------------------------------------------------------------------
KnowledgeGroupFactory::~KnowledgeGroupFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::CreateKnowledgeGroup
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* KnowledgeGroupFactory::Create( kernel::Team_ABC& team )
{
    KnowledgeGroup* result = new KnowledgeGroup( controllers_.controller_, idManager_, staticModel_.types_ );
    result->Attach< kernel::CommunicationHierarchies >( *new KnowledgeGroupCommunications( controllers_.controller_, *result, &team ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::CreateKnowledgeGroup
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* KnowledgeGroupFactory::Create( xml::xistream& xis, kernel::Team_ABC& team )
{
    KnowledgeGroup* result = new KnowledgeGroup( xis, controllers_.controller_, idManager_, staticModel_.types_ );
    result->Attach< kernel::CommunicationHierarchies >( *new KnowledgeGroupCommunications( controllers_.controller_, *result, &team ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::CreateKnowledgeGroup
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* KnowledgeGroupFactory::Create( kernel::KnowledgeGroup_ABC& kgparent )
{
    KnowledgeGroup* result = new KnowledgeGroup( controllers_.controller_, idManager_, staticModel_.types_ );
    result->Attach< kernel::CommunicationHierarchies >( *new KnowledgeGroupCommunications( controllers_.controller_, *result, &kgparent ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::CreateKnowledgeGroup
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* KnowledgeGroupFactory::Create( xml::xistream& xis, kernel::KnowledgeGroup_ABC& kgparent )
{
    KnowledgeGroup* result = new KnowledgeGroup( xis, controllers_.controller_, idManager_, staticModel_.types_ );
    result->Attach< kernel::CommunicationHierarchies >( *new KnowledgeGroupCommunications( controllers_.controller_, *result, &kgparent ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::FindKnowledgeGroup
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
//kernel::KnowledgeGroup_ABC* KnowledgeGroupFactory::FindKnowledgeGroup( const unsigned long& id ) const
//{
//    for( tools::Resolver< Team_ABC >::CIT_Elements it = tools::Resolver< Team_ABC >::elements_.begin(); it != tools::Resolver< Team_ABC >::elements_.end(); ++it )
//    {
//        Team_ABC& team = *it->second;
//        const kernel::CommunicationHierarchies& hierarchies = team.Get< kernel::CommunicationHierarchies >();
//        tools::Iterator< const kernel::Entity_ABC& > subIt = hierarchies.CreateSubordinateIterator();
//        while( subIt.HasMoreElements() )
//        {
//            const kernel::Entity_ABC& kg = subIt.NextElement();
//            if( kg.GetId() == id )
//                // $$$$ AGE 2006-10-09: 
//                return const_cast< KnowledgeGroup_ABC* >( static_cast< const KnowledgeGroup_ABC* >( &kg ) );
//        };
//    }
//    return 0;
//}
