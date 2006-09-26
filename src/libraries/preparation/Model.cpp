// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Model.h"
#include "StaticModel.h"
#include "TeamsModel.h"
#include "TeamFactory.h"
#include "KnowledgeGroupsModel.h"
#include "AgentFactory.h"
#include "AgentsModel.h"
#include "ObjectFactory.h"
#include "ObjectsModel.h"
#include "FormationFactory.h"
#include "FormationModel.h"
#include "IdManager.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::Model( Controllers& controllers, const StaticModel& staticModel )
    : controllers_( controllers )
    , idManager_( *new IdManager() )
    , teamFactory_( *new TeamFactory( controllers, *this, idManager_ ) )
    , agentFactory_( *new AgentFactory( controllers, *this, staticModel, idManager_ ) )
    , objectFactory_( *new ObjectFactory( controllers, *this, staticModel, idManager_ ) )
    , formationFactory_( *new FormationFactory( controllers, idManager_ ) )
    , teams_( *new TeamsModel( controllers, teamFactory_ ) )
    , knowledgeGroups_( *new KnowledgeGroupsModel( teams_ ) )
    , agents_( *new AgentsModel( controllers, agentFactory_ ) )
    , objects_( *new ObjectsModel( objectFactory_ ) )
    , formations_( *new FormationModel( controllers, formationFactory_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::~Model()
{
    delete &formations_;
    delete &formationFactory_;
    delete &objects_;
    delete &objectFactory_;
    delete &agents_;
    delete &agentFactory_;
    delete &knowledgeGroups_;
    delete &teams_;
    delete &teamFactory_;
    delete &idManager_;
}

// ----------------------------------------------------------------------------- 
// Name: Model::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void Model::Purge()
{
    agents_.Purge();
    knowledgeGroups_.Purge();
    teams_.Purge();
    idManager_.Reset();
}
    
// -----------------------------------------------------------------------------
// Name: Model::Serialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Model::Serialize( xml::xostream& xos ) const
{
    xos << start( "orbat" );
    teams_.Serialize( xos );
    objects_.Serialize( xos );
    xos << end();
}
