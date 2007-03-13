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
ParamLocation::ParamLocation( QWidget* parent, ASN1T_Localisation*& asn, const QString& label, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : converter_ ( converter )
    , asn_       ( new ASN1T_Localisation() )
    , serializer_( converter, *asn_ )
    , controller_( 0 )
    , location_( 0 )
{
    asn = asn_;

    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new RichLabel( label, false, box, "" );

    pShapeLabel_ = new QLabel( "---", box );
    pShapeLabel_->setMinimumWidth( 100 );
    pShapeLabel_->setAlignment( Qt::AlignCenter );
    pShapeLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    creator_ = new LocationCreator( box, label, layer, *this );

    asn_->vecteur_point.elem = 0;
    asn_->vecteur_point.n = 0;
}

// -----------------------------------------------------------------------------
// Name: ParamLocation destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamLocation::~ParamLocation()
{
    delete asn_;
    delete location_;
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
// Name: ParamLocation::Commit
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Commit()
{
    if( location_ )
        serializer_.Serialize( *location_ );
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

// -----------------------------------------------------------------------------
// Name: ParamLocation::CommitTo
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamLocation::CommitTo( ASN1T_Localisation& destination )
{
    if( location_ )
    {
        LocationSerializer serializer( converter_ );
        serializer.Serialize( *location_, destination );
    }
}
