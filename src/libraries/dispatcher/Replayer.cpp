// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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

#include "xeumeuleu/xml.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Replayer constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Replayer::Replayer( const Config& config, const std::string& records )
    : config_( config )
{
    pModel_            = new Model               ();
    pClientsNetworker_ = new ClientsNetworker    ( *this, config_ );
    simulation_        = new SimulationDispatcher( *pClientsNetworker_, *pModel_ );
    loader_            = new LoaderFacade        ( *pClientsNetworker_, *simulation_, config_, records );
    profiles_          = new ProfileManager      ( *pModel_, *pClientsNetworker_, config_ );

    profiles_->Reset();
    pClientsNetworker_->AllowConnections();
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
    pClientsNetworker_->Update();
    loader_->Update();
}

// -----------------------------------------------------------------------------
// Name: Replayer::GetModel
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Model& Replayer::GetModel() const
{
    return *pModel_;
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
    return *loader_;
}
