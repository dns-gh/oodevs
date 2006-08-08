// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ParamLocation.h"
#include "ParametersLayer.h"
#include "RichLabel.h"
#include "Tools.h"
#include "astec_kernel/GlTools_ABC.h"
#include "LocationCreator.h"
#include "astec_kernel/ActionController.h"
#include "astec_kernel/CoordinateConverter_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamLocation constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamLocation::ParamLocation( QWidget* pParent, ASN1T_Localisation& asn, const std::string& label, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : QHBox      ( pParent )
    , asn_       ( asn )
    , converter_ ( converter )
    , pUMTCoords_( 0 )
    , controller_( 0 )
{
    setSpacing( 5 );
    pLabel_ = new RichLabel( label.c_str(), false, this, "" );

    pShapeLabel_ = new QLabel( "---", this );
    pShapeLabel_->setMinimumWidth( 100 );
    pShapeLabel_->setAlignment( Qt::AlignCenter );
    pShapeLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    creator_ = new LocationCreator( this, label, layer, *this );
    creator_->AddLocationType( tr( "point" ), EnumTypeLocalisation::point );
    creator_->AddLocationType( tr( "ligne" ), EnumTypeLocalisation::line );
    creator_->AddLocationType( tr( "polygone" ), EnumTypeLocalisation::polygon );
    creator_->AddLocationType( tr( "cercle" ), EnumTypeLocalisation::circle );

    asn_.vecteur_point.elem = 0;
    asn_.vecteur_point.n = 0;
}

// -----------------------------------------------------------------------------
// Name: ParamLocation destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamLocation::~ParamLocation()
{
    delete[] pUMTCoords_;
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
    if( ! IsOptional() && points_.empty() )
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
    asn_.type = nType_;
    unsigned nNbrPoints = points_.size();

    asn_.vecteur_point.n = nNbrPoints;
    if( ! nNbrPoints )
        return;

    delete[] pUMTCoords_;
    pUMTCoords_ = new ASN1T_CoordUTM[ nNbrPoints ];
    asn_.vecteur_point.elem = pUMTCoords_;

    for( uint i = 0; i < nNbrPoints; ++i )
    {
        const std::string coord = converter_.ConvertToMgrs( points_[i] );
        asn_.vecteur_point.elem[i] = coord.c_str();
    }
}
    
// -----------------------------------------------------------------------------
// Name: ParamLocation::Handle
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Handle( const T_PointVector& points )
{
    nType_ = creator_->GetCurrentType();
    points_ = points;
    if(    (nType_ == EnumTypeLocalisation::point   && points_.size() == 1 )
        || (nType_ == EnumTypeLocalisation::line    && points_.size() >= 2 )
        || (nType_ == EnumTypeLocalisation::circle  && points_.size() == 2 )
        || (nType_ == EnumTypeLocalisation::polygon && points_.size() > 2 ) )
        pShapeLabel_->setText( tools::ToString( nType_ ) );
    else
        pShapeLabel_->setText( tr( "---" ) );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Draw( const geometry::Point2f& , const geometry::Rectangle2f& extent, const GlTools_ABC& tools ) const
{
    creator_->Draw( points_, nType_, tools );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::CommitTo
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamLocation::CommitTo( ASN1T_Localisation& destination )
{
    destination.type = asn_.type;
    destination.vecteur_point.n    = asn_.vecteur_point.n;
    destination.vecteur_point.elem = new ASN1T_CoordUTM[ asn_.vecteur_point.n ];
    for( unsigned int i = 0; i < asn_.vecteur_point.n; ++i )
        destination.vecteur_point.elem[i] = asn_.vecteur_point.elem[i];
}
