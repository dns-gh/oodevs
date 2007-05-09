// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ESRI_ConnectorFacade.h"
#include "ESRI_Connector.h"
#include "ESRI_Config.h"
#include "ESRI_World.h"
#include "AgentTypes.h"
#include "dispatcher/Model.h"
#include "dispatcher/Visitors.h"

using namespace dispatcher;
using namespace esri;

// -----------------------------------------------------------------------------
// Name: ConnectorFacade constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ConnectorFacade::ConnectorFacade( const Model& model, const esri::Config& config )
    : model_     ( model )
    , types_     ( new kernel::AgentTypes() )
    , world_     ( new esri::World( config ) )
    , connector_ ( new Connector( config.GetGeodatabaseFile(), *types_, *world_ ) ) // $$$$ JCR 2007-04-30 : Config file
{
    types_->Load( config );
}
    
// -----------------------------------------------------------------------------
// Name: ConnectorFacade destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ConnectorFacade::~ConnectorFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::Update
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void ConnectorFacade::Update()
{
    {
        CreationVisitor visitor( *connector_ );
        model_.Accept( visitor );
    }
    {
        FullUpdateVisitor visitor( *connector_ );
        model_.Accept( visitor );
    }    
}
