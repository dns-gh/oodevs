// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamLocation.h"
#include "astec_gui/ParametersLayer.h"
#include "astec_gui/RichLabel.h"
#include "astec_gui/Tools.h"
#include "astec_kernel/GlTools_ABC.h"
#include "astec_gui/LocationCreator.h"
#include "astec_kernel/ActionController.h"
#include "astec_kernel/CoordinateConverter_ABC.h"
#include "astec_kernel/Location_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamLocation constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamLocation::ParamLocation( QWidget* pParent, ASN1T_Localisation& asn, const std::string& label, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : QHBox      ( pParent )
    , converter_ ( converter )
    , serializer_( converter, asn )
    , controller_( 0 )
    , location_( 0 )
{
    setSpacing( 5 );
    pLabel_ = new RichLabel( label.c_str(), false, this, "" );

    pShapeLabel_ = new QLabel( "---", this );
    pShapeLabel_->setMinimumWidth( 100 );
    pShapeLabel_->setAlignment( Qt::AlignCenter );
    pShapeLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    creator_ = new LocationCreator( this, label, layer, *this );

    asn.vecteur_point.elem = 0;
    asn.vecteur_point.n = 0;
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
        pShapeLabel_->setText( location.GetName().c_str() );
    else
        pShapeLabel_->setText( tr( "---" ) );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Draw( const geometry::Point2f& , const geometry::Rectangle2f& extent, const GlTools_ABC& tools ) const
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
