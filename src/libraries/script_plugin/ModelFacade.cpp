// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ModelFacade.h"
#include "AgentManipulator.h"
#include "directia/brain/Brain.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Model_ABC.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ModelFacade constructor
// Created: SBO 2010-09-17
// -----------------------------------------------------------------------------
ModelFacade::ModelFacade( const dispatcher::Model_ABC& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelFacade destructor
// Created: SBO 2010-09-17
// -----------------------------------------------------------------------------
ModelFacade::~ModelFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelFacade::RegisterIn
// Created: SBO 2010-09-17
// -----------------------------------------------------------------------------
void ModelFacade::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "model.orbat" ] = this ;
    brain.Register( "FindAgent", &ModelFacade::FindAgent );
}

namespace directia
{
    void UsedByDIA    ( AgentManipulator* ) { }
    void ReleasedByDIA( AgentManipulator* ) { }
    void UsedByDIA    ( ModelFacade* ) { }
    void ReleasedByDIA( ModelFacade* ) { }
}

// -----------------------------------------------------------------------------
// Name: ModelFacade::FindAgent
// Created: SBO 2010-09-17
// -----------------------------------------------------------------------------
const AgentManipulator* ModelFacade::FindAgent( unsigned long id )
{
    if( const dispatcher::Agent_ABC* agent = model_.Agents().Find( id ) )
        if( const AgentManipulator* manipulator = agent->Retrieve< AgentManipulator >() )
            return manipulator;
    return 0;
}
