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
#include "moc_ParamLocation.cpp"
#include "CoordinateConverter.h"
#include "ParametersLayer.h"
#include "MT/MT_Qt/MT_ParameterLabel.h"
#include "Tools.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamLocation constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamLocation::ParamLocation( QWidget* pParent, ASN1T_Localisation& asn, const std::string label, const std::string menu, ParametersLayer& layer, const CoordinateConverter& converter )
    : QHBox        ( pParent )
    , layer_       ( layer )
    , asn_         ( asn )
    , converter_   ( converter )
    , menu_        ( menu )
    , pUMTCoords_  ( 0 )
    , nType_       ( EnumTypeLocalisation::point )
{
    setSpacing( 5 );
    pLabel_ = new MT_ParameterLabel( label.c_str(), false, this, "" );

    pShapeLabel_ = new QLabel( "---", this );
    pShapeLabel_->setMinimumWidth( 100 );
    pShapeLabel_->setAlignment( Qt::AlignCenter );
    pShapeLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    pPopupMenu_ = new QPopupMenu( this );
    int n;
    n = pPopupMenu_->insertItem( tr( "point" ), this, SLOT( Start( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::point );
    n = pPopupMenu_->insertItem( tr( "ligne" ), this, SLOT( Start( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::line );
    n = pPopupMenu_->insertItem( tr( "polygone" ), this, SLOT( Start( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::polygon );
    n = pPopupMenu_->insertItem( tr( "cercle" ), this, SLOT( Start( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::circle );

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
// Name: ParamLocation::CheckValidity
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
bool ParamLocation::CheckValidity()
{
    if( ! IsOptional() && points_.empty() )
    {
        pLabel_->TurnRed( 3000 );
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
// Name: ParamLocation::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::NotifyContextMenu( const geometry::Point2f& point, QPopupMenu& menu )
{
    popupPoint_ = point;
    menu.insertItem( menu_.c_str(), pPopupMenu_ );
}
    
// -----------------------------------------------------------------------------
// Name: ParamLocation::Handle
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Handle( const T_PointVector& points )
{
    points_ = points;
    if(    (nType_ == EnumTypeLocalisation::point   && points_.size() == 1 )
        || (nType_ == EnumTypeLocalisation::line    && points_.size() >= 2 )
        || (nType_ == EnumTypeLocalisation::circle  && points_.size() == 2 )
        || (nType_ == EnumTypeLocalisation::polygon && points_.size() > 2 ) )
        pShapeLabel_->setText( Tools::ToString( nType_ ) );
    else
        pShapeLabel_->setText( tr( "---" ) );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::Start
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Start( int type )
{
    points_.clear();
    nType_ = (ASN1T_EnumTypeLocalisation)type;

    // Special case for point parameter.
    if( nType_ == EnumTypeLocalisation::point )
    {
        points_.push_back( popupPoint_ );
        pShapeLabel_->setText( Tools::ToString( nType_ ) );
    }
    else
    {
        pShapeLabel_->setText( tr( "En cours..." ) );
        if( nType_ == EnumTypeLocalisation::line )
            layer_.StartLine( *this );
        else if( nType_ == EnumTypeLocalisation::circle )
            layer_.StartCircle( *this );
        else if( nType_ == EnumTypeLocalisation::polygon )
            layer_.StartPolygon( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamLocation::Draw( const geometry::Point2f& point, const GlTools_ABC& tools ) const
{
    if( points_.empty() )
        return;
    
    if( nType_ == EnumTypeLocalisation::point )
        for( T_PointVector::const_iterator it = points_.begin(); it != points_.end(); ++it )
            tools.DrawCross( *it );
    else if( nType_ == EnumTypeLocalisation::line )
        tools.DrawLines( points_ );
    else if( nType_ == EnumTypeLocalisation::polygon && ! points_.empty() )
    {
        tools.DrawLines( points_ );
        tools.DrawLine( points_.back(), points_.front() );
    }
    else if( nType_ == EnumTypeLocalisation::circle && points_.size() >= 2 )
        tools.DrawCircle( points_.front(), points_.front().Distance( points_.back() ) );
}
