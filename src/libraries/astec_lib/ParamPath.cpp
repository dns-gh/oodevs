// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamPath.h"
#include "moc_ParamPath.cpp"
#include "ParametersLayer.h"
#include "RichLabel.h"
#include "CoordinateConverter_ABC.h"
#include "GlTools_ABC.h"
#include "Entity_ABC.h"
#include "Positions.h"

// -----------------------------------------------------------------------------
// Name: ParamPath constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPath::ParamPath( QWidget* pParent, ASN1T_Itineraire& asn, const std::string label, const std::string menu, ParametersLayer& layer, const CoordinateConverter_ABC& converter, const Entity_ABC& agent )
    : QHBox( pParent )
    , asn_( asn )
    , menu_( menu )
    , layer_( layer )
    , converter_( converter )
    , agentPos_( agent.Get< Positions >().GetPosition() )
    , pUMTCoords_( 0 )
{
    setSpacing( 5 );
    pLabel_ = new RichLabel( label.c_str(), false, this, "" );

    pPosLabel_ = new QLabel( "---", this );
    pPosLabel_->setMinimumWidth( 100 );
    pPosLabel_->setAlignment( Qt::AlignCenter );
    pPosLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
}

// -----------------------------------------------------------------------------
// Name: ParamPath destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPath::~ParamPath()
{
    delete pUMTCoords_;
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Draw( const geometry::Point2f& point, const geometry::Rectangle2f& , const GlTools_ABC& tools ) const
{
    if( ! points_.empty() )
    {
        tools.DrawLine( point, points_.front() );
        tools.DrawLines( points_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamPath::CheckValidity
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
bool ParamPath::CheckValidity()
{
    if( points_.size() <= 1 )
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Commit
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Commit()
{
    unsigned nNbrPoints  = points_.size() - 1;
    asn_.type            = EnumTypeLocalisation::line;
    asn_.vecteur_point.n = nNbrPoints;

    delete[] pUMTCoords_;
    pUMTCoords_ = new ASN1T_CoordUTM[ nNbrPoints ];
    asn_.vecteur_point.elem = pUMTCoords_;

    for( unsigned i = 0; i < nNbrPoints; ++i )
    {
        const std::string coord = converter_.ConvertToMgrs( points_[i+1] );
        asn_.vecteur_point.elem[i] = coord.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ParamPath::CommitTo
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPath::CommitTo( ASN1T_Itineraire& destination )
{
    destination.type = EnumTypeLocalisation::line;
    const unsigned int points = points_.size() - 1;
    destination.vecteur_point.n = points;
    destination.vecteur_point.elem = new ASN1T_CoordUTM[ points ];
    for( unsigned int i = 0; i < points; ++i )
    {
        const std::string coord = converter_.ConvertToMgrs( points_[ i + 1 ] );
        destination.vecteur_point.elem[i] = coord.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ParamPath::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::NotifyContextMenu( const geometry::Point2f&, QPopupMenu& menu )
{   
    menu.insertItem( menu_.c_str(), this, SLOT( StartPath() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::StartPath
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::StartPath()
{
    points_.clear();
    layer_.StartLine( *this );
    layer_.AddPoint( agentPos_ );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Handle
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Handle( const T_PointVector& points )
{
    if( points.empty() )
        return;

    points_ = points;
    pPosLabel_->setText( converter_.ConvertToMgrs( points_.back() ).c_str() );
}
