// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "KnowledgeGroupsModel.h"
#include "KnowledgeGroup.h"
#include "clients_kernel/Controllers.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroupsModel::KnowledgeGroupsModel( kernel::Controllers& controllers, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory ) // LTO
    : controllers_( controllers )
    , knowledgeGroupFactory_( knowledgeGroupFactory ) // LTO
{
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroupsModel::~KnowledgeGroupsModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::Purge()
{
    // NOTHING 
}

// LTO begin
// -----------------------------------------------------------------------------
// Name: tools::Iterator< const kernel::KnowledgeGroup_ABC& > KnowledgeGroupsModel::CreateIterator
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
tools::Iterator< const kernel::KnowledgeGroup_ABC& > KnowledgeGroupsModel::CreateIterator() const
{
    throw std::runtime_error( "not implemented" );
}

// -----------------------------------------------------------------------------
// Name: void KnowledgeGroupsModel::Create
// Created:  FHD 2009-11-19: 
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::Create( kernel::Team_ABC& parent )
{
    KnowledgeGroup_ABC* knowledgegroup = knowledgeGroupFactory_.Create( parent );
    Register( knowledgegroup->GetId(), *knowledgegroup );
}

// -----------------------------------------------------------------------------
// Name: void KnowledgeGroupsModel::Create
// Created:  FHD 2009-11-19: 
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::Create( xml::xistream& xis, kernel::Team_ABC& parent, Model& model )
{
    KnowledgeGroup_ABC* knowledgegroup = knowledgeGroupFactory_.Create( xis, parent );
    Register( knowledgegroup->GetId(), *knowledgegroup );
    xis >> xml::list( "knowledge-group", *this, &KnowledgeGroupsModel::CreateSubKnowledgeGroup, *knowledgegroup, model );
}

// -----------------------------------------------------------------------------
// Name: void KnowledgeGroupsModel::CreateSubKnowledgeGroup
// Created:  FHD 2009-11-19: 
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::CreateSubKnowledgeGroup( kernel::KnowledgeGroup_ABC& parent )
{
    KnowledgeGroup_ABC* knowledgegroup = knowledgeGroupFactory_.Create( parent );
    Register( knowledgegroup->GetId(), *knowledgegroup );
}

// -----------------------------------------------------------------------------
// Name: void KnowledgeGroupsModel::CreateSubKnowledgeGroup
// Created:  FHD 2009-11-19: 
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::CreateSubKnowledgeGroup( xml::xistream& xis, kernel::KnowledgeGroup_ABC& parent, Model& model )
{
    KnowledgeGroup_ABC* knowledgegroup = knowledgeGroupFactory_.Create( xis, parent );
    xis >> xml::list( "knowledge-group", *this, &KnowledgeGroupsModel::CreateSubKnowledgeGroup, *knowledgegroup, model );
    Register( knowledgegroup->GetId(), *knowledgegroup );
}
// LTO end
