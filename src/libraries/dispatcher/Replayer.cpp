// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Replayer.h"
#include "Config.h"
#include "Model.h"
#include "ClientsNetworker.h"
#include "SimulationDispatcher.h"
#include "LoaderFacade.h"
#include "ProfileManager.h"
#include "Loader.h"

#include "xeumeuleu/xml.h"

using namespace dispatcher;

namespace 
{
    boost::shared_ptr< Model > CreateModel( CompositeMessageHandler& handler, const Config& config )
    {
        boost::shared_ptr< Model > result( new Model( config ) );
        handler.Add( result );
        return result;
    }
    boost::shared_ptr< SimulationDispatcher > CreateSimulation( ClientPublisher_ABC& publisher, Model& model, CompositeMessageHandler& handler )
    {
        boost::shared_ptr< SimulationDispatcher > result( new SimulationDispatcher( publisher, model ) );
        handler.Add( result );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: Replayer constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Replayer::Replayer( const Config& config, const std::string& records )
    : model_           ( CreateModel( handler_, config ) )
    , clientsNetworker_( new ClientsNetworker( *this, config ) )
    , simulation_      ( CreateSimulation( *clientsNetworker_, *model_, handler_ ) )
    , loader_          ( new Loader( *simulation_, handler_, config, records ) )
    , facade_          ( new LoaderFacade( *clientsNetworker_, *loader_ ) )
    , profiles_        ( new ProfileManager( *model_, *clientsNetworker_, config ) )
{
    profiles_->Reset();
    clientsNetworker_->AllowConnections();
}
 
// -----------------------------------------------------------------------------
// Name: Replayer destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Replayer::~Replayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Replayer::Update
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Replayer::Update()
{
    clientsNetworker_->Update();
    facade_->Update();
}

// -----------------------------------------------------------------------------
// Name: Replayer::GetModel
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Model& Replayer::GetModel() const
{
    return *model_;
}

// -----------------------------------------------------------------------------
// Name: Replayer::GetProfiles
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
ProfileManager& Replayer::GetProfiles() const
{
    return *profiles_;
}

// -----------------------------------------------------------------------------
// Name: Replayer::GetLoader
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
LoaderFacade& Replayer::GetLoader() const
{
    return *facade_;
}
