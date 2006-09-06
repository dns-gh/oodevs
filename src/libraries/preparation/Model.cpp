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
#include "Serializable_ABC.h"
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
    , teamFactory_( *new TeamFactory( controllers, *this ) )
    , teams_( *new TeamsModel( controllers, teamFactory_ ) )
    , knowledgeGroups_( *new KnowledgeGroupsModel( teams_ ) )
    , agentFactory_( *new AgentFactory( controllers, *this, staticModel ) )
    , agents_( *new AgentsModel( controllers, agentFactory_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::~Model()
{
    delete &agents_;
    delete &agentFactory_;
    delete &knowledgeGroups_;
    delete &teams_;
    delete &teamFactory_;
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
}
    
// -----------------------------------------------------------------------------
// Name: Model::Serialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Model::Serialize( xml::xostream& xos ) const
{
    xos << start( "ODB" );
    teams_ .Serialize( xos );
    agents_.Serialize( xos );
//    objects_.Serialize( xos );
    xos << end();
}
