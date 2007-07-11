// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamLocation.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_gui/LocationCreator.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterLocation.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamLocation constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamLocation::ParamLocation( const OrderParameter& parameter, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : Param_ABC  ( parameter.GetName() )
    , parameter_ ( parameter )
    , converter_ ( converter )
    , layer_     ( layer )
    , creator_   ( 0 )
    , controller_( 0 )
    , pLabel_    ( 0 )
    , location_  ( 0 )
    , filter_    ( true, true, true, true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLocation destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamLocation::~ParamLocation()
{
    delete location_;
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamLocation::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new RichLabel( GetName(), false, box );
    pShapeLabel_ = new QLabel( "---", box );
    pShapeLabel_->setMinimumWidth( 100 );
    pShapeLabel_->setAlignment( Qt::AlignCenter );
    pShapeLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    creator_ = new LocationCreator( box, GetName(), layer_, *this );
    SetShapeFilter( filter_.point_, filter_.line_, filter_.polygon_, filter_.circle_ );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::RemoveFromController
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void ParamLocation::RemoveFromController()
{
    if( controller_ )
        controller_->Unregister( *creator_ );
    Param_ABC::RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::RegisterIn
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void ParamLocation::RegisterIn( ActionController& controller )
{
    controller_ = &controller;
    controller.Register( *creator_ );
    Param_ABC::RegisterIn( controller );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::CheckValidity
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
bool ParamLocation::CheckValidity()
{
    if( !parameter_.IsOptional() && !location_ )
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::CommitTo
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void ParamLocation::CommitTo( ActionParameterContainer_ABC& action ) const
{
    action.AddParameter( *new ActionParameterLocation( parameter_, converter_, *location_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::Handle
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Handle( Location_ABC& location )
{
    // $$$$ AGE 2007-07-11: auto_ptr
    delete location_;
    location_ = &location;
    if( location.IsValid() )
        pShapeLabel_->setText( location.GetName() );
    else
        pShapeLabel_->setText( "---" );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::SetShapeFilter
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
void ParamLocation::SetShapeFilter( bool point, bool line, bool polygon, bool circle )
{
    filter_ = ShapeFilter( point, line, polygon, circle );
    if( creator_ )
        creator_->Allow( point, line, polygon, circle );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Draw( const geometry::Point2f& , const Viewport_ABC& , const GlTools_ABC& tools ) const
{
    if( location_ )
        location_->Draw( tools );
}
