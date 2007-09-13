// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamMissionObjective.h"
#include "ParamLocation.h"
#include "ParamDateTime.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameter.h"
#include "gaming/ActionParameterObjective.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ParamMissionObjective::ParamMissionObjective( const OrderParameter& parameter, gui::ParametersLayer& layer, const Simulation& simulation, const CoordinateConverter_ABC& converter )
    : Param_ABC( parameter.GetName() )
    , parameter_( parameter )
    , layer_( layer )
    , converter_( converter )
    , simulation_( simulation )
    , location_( 0 )
    , schedule_( 0 )
    , optional_( parameter.IsOptional() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective destructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ParamMissionObjective::~ParamMissionObjective()
{
    delete schedule_;
    delete location_;
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective::RemoveFromController
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamMissionObjective::RemoveFromController()
{
    location_->RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective::RegisterIn
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamMissionObjective::RegisterIn( ActionController& controller )
{
    location_->RegisterIn( controller );
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective::Draw
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamMissionObjective::Draw( const geometry::Point2f& point, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    location_->Draw( point, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective::BuildInterface
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamMissionObjective::BuildInterface( QWidget* parent )
{
    QGroupBox* group = new QGroupBox( 1, Qt::Horizontal, GetName(), parent );
    location_ = new ParamLocation( OrderParameter( tools::translate( "ParamMissionObjective", "Location" ), "location", false ), layer_, converter_ );
    location_->BuildInterface( group );
    schedule_ = new ParamDateTime( group, tools::translate( "ParamMissionObjective", "Schedule" ), simulation_, true ); // $$$$ SBO 2007-05-14: optional?
    schedule_->BuildInterface( group );
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective::CheckValidity
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
bool ParamMissionObjective::CheckValidity()
{
    return optional_ || ( location_ && schedule_ && location_->CheckValidity() && schedule_->CheckValidity() );
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective::CommitTo
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamMissionObjective::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterObjective( parameter_ ) );
    location_->CommitTo( *param );
    schedule_->CommitTo( *param );
    action.AddParameter( *param.release() );
}

