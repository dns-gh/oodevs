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
#include "FormationFactory.h"
#include "FormationModel.h"
#include "IdManager.h"
#include "LimitsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PathTools.h"
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
    , teamFactory_( *new TeamFactory( controllers, *this, staticModel, idManager_ ) )
    , agentFactory_( *new AgentFactory( controllers, *this, staticModel, idManager_ ) )
    , formationFactory_( *new FormationFactory( controllers, idManager_ ) )
    , teams_( *new TeamsModel( controllers, teamFactory_ ) )
    , knowledgeGroups_( *new KnowledgeGroupsModel( teams_ ) )
    , agents_( *new AgentsModel( controllers, agentFactory_ ) )
    , formations_( *new FormationModel( controllers, formationFactory_ ) )
    , limits_( *new LimitsModel( controllers, staticModel.coordinateConverter_, idManager_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::~Model()
{
    delete &limits_;
    delete &formations_;
    delete &formationFactory_;
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
    limits_.Purge();
    formations_.Purge();
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
    xos << end();
}

// -----------------------------------------------------------------------------
// Name: Model::Load
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void Model::Load( const std::string& filename )
{
    xml::xifstream xis( filename );
    std::string orbat;
    xis >> start( "Scipio" )
            >> start( "Donnees" )
            >> content( "ODB", orbat );
    teams_.Load( path_tools::BuildChildPath( filename, orbat ), *this );
}
