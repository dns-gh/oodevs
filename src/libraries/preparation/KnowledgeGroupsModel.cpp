// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "KnowledgeGroupsModel.h"
#include "KnowledgeGroup.h"
#include "Model.h"
#include "clients_kernel/Controllers.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroupsModel::KnowledgeGroupsModel( kernel::Controllers& controllers, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory ) // LTO
    : controllers_( controllers )
    , knowledgeGroupFactory_( knowledgeGroupFactory ) // LTO
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroupsModel::~KnowledgeGroupsModel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::Purge()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: void KnowledgeGroupsModel::Create
// Created:  FHD 2009-11-19:
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::Create( kernel::Team_ABC& parent, bool isCrowd )
{
    KnowledgeGroup_ABC* knowledgegroup = knowledgeGroupFactory_.Create( parent, isCrowd );
    Register( knowledgegroup->GetId(), *knowledgegroup );
}

// LTO begin
// -----------------------------------------------------------------------------
// Name: void KnowledgeGroupsModel::Create
// Created:  FHD 2009-11-19:
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::Create( xml::xistream& xis, kernel::Team_ABC& parent, Model& model )
{
    try
    {
        KnowledgeGroup_ABC* knowledgegroup = knowledgeGroupFactory_.Create( xis, parent );
        if( knowledgegroup )
        {
            Register( knowledgegroup->GetId(), *knowledgegroup );
            xis >> xml::list( "knowledge-group", *this, &KnowledgeGroupsModel::CreateSubKnowledgeGroup, *knowledgegroup, model );
        }
    }
    catch( std::runtime_error& e )
    {
        model.AppendLoadingError( eOthers, std::string( e.what() ) );
    }
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
    try
    {
        KnowledgeGroup_ABC* knowledgegroup = knowledgeGroupFactory_.Create( xis, parent );
        if( knowledgegroup )
        {
            Register( knowledgegroup->GetId(), *knowledgegroup );
            xis >> xml::list( "knowledge-group", *this, &KnowledgeGroupsModel::CreateSubKnowledgeGroup, *knowledgegroup, model );
        }
    }
    catch( std::runtime_error& e )
    {
        model.AppendLoadingError( eOthers, std::string( e.what() ) );
    }
}
// LTO end

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel::NotifyDeleted
// Created: LGY 2012-03-26
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::NotifyDeleted( const kernel::KnowledgeGroup_ABC& group )
{
    tools::Resolver< KnowledgeGroup_ABC >::Remove( group.GetId() );
}
