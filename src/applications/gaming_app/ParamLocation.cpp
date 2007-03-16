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

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamLocation constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamLocation::ParamLocation( const QString& name, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : Param_ABC  ( name )
    , converter_ ( converter )
    , layer_     ( layer )
    , controller_( 0 )
    , pLabel_    ( 0 )
    , location_  ( 0 )
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
    pLabel_ = new RichLabel( GetName(), false, parent );
    pShapeLabel_ = new QLabel( "---", parent );
    pShapeLabel_->setMinimumWidth( 100 );
    pShapeLabel_->setAlignment( Qt::AlignCenter );
    pShapeLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    creator_ = new LocationCreator( parent, GetName(), layer_, *this );
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
    if( ! IsOptional() && !location_ )
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
// Name: ParamLocation::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Draw( const geometry::Point2f& , const Viewport_ABC& extent, const GlTools_ABC& tools ) const
{
    if( location_ )
        location_->Draw( tools );
}
