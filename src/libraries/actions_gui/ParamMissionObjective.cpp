// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamMissionObjective.h"
#include "ParamLocation.h"
#include "ParamDateTime.h"
#include "actions/Action_ABC.h"
#include "actions/Objective.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ParamMissionObjective::ParamMissionObjective( const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const QDateTime& current, const kernel::CoordinateConverter_ABC& converter )
    : Param_ABC( parameter.GetName().c_str() )
    , parameter_( parameter )
    , layer_( layer )
    , converter_( converter )
    , currentDate_( current )
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
    RemoveFromController();
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
    Param_ABC::RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective::RegisterIn
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamMissionObjective::RegisterIn( kernel::ActionController& controller )
{
    location_->RegisterIn( controller );
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective::Draw
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamMissionObjective::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    location_->Draw( point, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective::BuildInterface
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
QWidget* ParamMissionObjective::BuildInterface( QWidget* parent )
{
    Q3GroupBox* group = new Q3GroupBox( 1, Qt::Horizontal, GetName(), parent );
    location_ = new ParamLocation( kernel::OrderParameter( tools::translate( "ParamMissionObjective", "Location" ).ascii(), "location", false ), layer_, converter_ );
    location_->BuildInterface( group );
    schedule_ = new ParamDateTime( group, tools::translate( "ParamMissionObjective", "Schedule" ).ascii(), currentDate_, true ); // $$$$ SBO 2007-05-14: optional?
    schedule_->BuildInterface( group );
    return group;
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
void ParamMissionObjective::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::Objective( parameter_ ) );
    location_->CommitTo( *param );
    schedule_->CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjective::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamMissionObjective::IsOptional() const
{
    return parameter_.IsOptional();
}
