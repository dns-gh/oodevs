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
#include "clients_kernel/OrderParameter.h"
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
    , parameter_ ( &parameter )
    , converter_ ( converter )
    , layer_     ( layer )
    , creator_   ( 0 )
    , controller_( 0 )
    , pLabel_    ( 0 )
    , location_  ( 0 )
    , optional_  ( parameter.IsOptional() )
    , filter_    ( true, true, true, true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLocation constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ParamLocation::ParamLocation( const QString& name, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, bool optional )
    : Param_ABC  ( name )
    , parameter_ ( 0 )
    , converter_ ( converter )
    , layer_     ( layer )
    , creator_   ( 0 )
    , controller_( 0 )
    , pLabel_    ( 0 )
    , location_  ( 0 )
    , optional_  ( optional )
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
        controller_->Remove( *creator_ );
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
    if( !optional_ && !location_ )
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamLocation::CommitTo( ASN1T_MissionParameter& asn ) const
{
    if( ! pLabel_ )
        InterfaceNotInitialized();
    asn.value.t = T_MissionParameter_value_localisation;
    asn.value.u.localisation = new ASN1T_Localisation();
    CommitTo( *asn.value.u.localisation );
    asn.null_value = asn.value.u.localisation->vecteur_point.n ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamLocation::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.localisation )
        delete[] asn.value.u.localisation->vecteur_point.elem;
    delete asn.value.u.localisation;
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamLocation::CommitTo( ASN1T_Localisation& asn ) const
{
    if( location_ )
    {
        LocationSerializer serializer( converter_ );
        serializer.Serialize( *location_, asn );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::CommitTo
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void ParamLocation::CommitTo( Action_ABC& action ) const
{
    if( !parameter_ )
        throw std::runtime_error( "OrderParameter not defined" );
    action.AddParameter( *new ActionParameterLocation( *parameter_, converter_, *location_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::CommitTo
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ParamLocation::CommitTo( ActionParameter_ABC& parameter ) const
{
    parameter.AddParameter( *new ActionParameterLocation( GetName(), converter_, *location_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::Handle
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Handle( Location_ABC& location )
{
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
void ParamLocation::Draw( const geometry::Point2f& , const Viewport_ABC& extent, const GlTools_ABC& tools ) const
{
    if( location_ )
        location_->Draw( tools );
}
