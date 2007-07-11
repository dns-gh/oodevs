// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamLimits.h"
#include "LimitParameter.h"
#include "clients_kernel/OrderParameter.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterLimits.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamLimits constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
ParamLimits::ParamLimits( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter )
    : Param_ABC( parameter.GetName() )
    , parameter_( parameter )
    , limit1_( new LimitParameter( parent, tools::translate( "ParamLimits", "Limit 1" ), converter, parameter.IsOptional() ) )
    , limit2_( new LimitParameter( parent, tools::translate( "ParamLimits", "Limit 2" ), converter, parameter.IsOptional() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLimits destructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
ParamLimits::~ParamLimits()
{
    delete limit1_;
    delete limit2_;
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::RemoveFromController
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLimits::RemoveFromController()
{
    Param_ABC::RemoveFromController();
    limit1_->RemoveFromController();
    limit2_->RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLimits::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const GlTools_ABC& tools ) const
{
    limit1_->Draw( point, extent, tools );
    limit2_->Draw( point, extent, tools );
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::RegisterIn
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLimits::RegisterIn( ActionController& controller )
{
    Param_ABC::RegisterIn( controller );
    limit1_->RegisterIn( controller );
    limit2_->RegisterIn( controller );
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::CheckValidity
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
bool ParamLimits::CheckValidity()
{
    bool bDummy = limit1_->CheckValidity(), bDenis = limit2_->CheckValidity();
    return bDummy && bDenis;
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamLimits::BuildInterface( QWidget* parent )
{
    limit1_->BuildInterface( parent );
    limit2_->BuildInterface( parent );
}

// -----------------------------------------------------------------------------
// Name: ParamLimits::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamLimits::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterLimits( parameter_ ) );
    limit1_->CommitTo( *param );
    limit2_->CommitTo( *param );
    action.AddParameter( *param.release() );
}
