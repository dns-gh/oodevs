// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamLocation.h"
#include "actions/Action_ABC.h"
#include "actions/Location.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Point.h"
#include "clients_gui/LocationCreator.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamLocation constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamLocation::ParamLocation( const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter )
    : Param_ABC  ( parameter.GetName().c_str() )
    , parameter_ ( parameter )
    , converter_ ( converter )
    , layer_     ( layer )
    , creator_   ( 0 )
    , controller_( 0 )
    , pLabel_    ( 0 )
    , location_  ()
    , filter_    ( true, true, true, true, true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLocation destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamLocation::~ParamLocation()
{
    RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamLocation::BuildInterface( QWidget* parent )
{
    Q3HBox* box = new Q3HBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new ::gui::RichLabel( GetName(), false, box );
    pShapeLabel_ = new QLabel( "---", box );
    pShapeLabel_->setMinimumWidth( 100 );
    pShapeLabel_->setAlignment( Qt::AlignCenter );
    pShapeLabel_->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
    creator_ = new ::gui::LocationCreator( box, GetName(), layer_, *this );
    SetShapeFilter( filter_.point_, filter_.line_, filter_.polygon_, filter_.circle_, filter_.rectangle_ );
    box->setStretchFactor( pShapeLabel_, 1 );
    return box;
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
void ParamLocation::RegisterIn( kernel::ActionController& controller )
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
    if( ! parameter_.IsOptional() && ! ( location_.get() && location_->IsValid() ) )
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
void ParamLocation::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    Commit< actions::parameters::Location, kernel::Point >( action );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::Handle
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Handle( kernel::Location_ABC& location )
{
    location_.reset( &location );
    if( location.IsValid() )
    {
        pShapeLabel_->setText( location.GetName() );
        NotifyChange();
    }
    else
        pShapeLabel_->setText( "---" );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::SetShapeFilter
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
void ParamLocation::SetShapeFilter( bool point, bool line, bool polygon, bool circle, bool rectangle )
{
    filter_ = ShapeFilter( point, line, polygon, circle, rectangle );
    if( creator_ )
    {
        creator_->Allow( point, line, polygon, circle, rectangle );
        if( location_.get() && location_->IsValid() && !creator_->Allows( *location_ ) )
            location_.reset();
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Draw( const geometry::Point2f& , const kernel::Viewport_ABC& , const kernel::GlTools_ABC& tools ) const
{
    if( location_.get() )
        ShapeHandler_ABC::Draw( *location_, geometry::Rectangle2f(), tools ); // $$$$ SBO 2008-06-03:
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamLocation::IsOptional() const
{
    return parameter_.IsOptional();
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::SetName
// Created: ABR 2011-01-21
// -----------------------------------------------------------------------------
void ParamLocation::SetName( const QString& name )
{
    this->name_ = name;
    if( pLabel_ )
        pLabel_->setText( name );
    if( creator_ )
        creator_->setName( name );
}
