// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ParamPoint.h"
#include "moc_ParamPoint.cpp"
#include "astec_kernel/CoordinateConverter_ABC.h"
#include "RichLabel.h"
#include "astec_kernel/GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamPoint constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPoint::ParamPoint( QWidget* pParent, ASN1T_Point& asn, const std::string label, const std::string menu, const CoordinateConverter_ABC& converter )
    : QHBox     ( pParent )
    , asn_      ( asn )
    , converter_( converter )
    , menu_     ( menu )
{
    setSpacing( 5 );
    pLabel_ = new RichLabel( label.c_str(), false, this, "" );

    pPosLabel_ = new QLabel( "---", this );
    pPosLabel_->setMinimumWidth( 100 );
    pPosLabel_->setAlignment( Qt::AlignCenter );
    pPosLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPoint::~ParamPoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::CheckValidity
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
bool ParamPoint::CheckValidity()
{
    if( pPosLabel_->text() == "---" )
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPoint::Draw( const geometry::Point2f& /*point*/, const geometry::Rectangle2f& extent, const GlTools_ABC& tools ) const
{
    if( pPosLabel_->text() != "---" && extent.IsInside( popupPoint_ ) )
        tools.DrawCross( popupPoint_, GL_CROSSSIZE );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::Commit
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPoint::Commit()
{
    asn_.type                  = EnumTypeLocalisation::point;
    asn_.vecteur_point.n       = 1;
    asn_.vecteur_point.elem    = &asnPoint_;
    const std::string coord = converter_.ConvertToMgrs( popupPoint_ );
    asnPoint_ = coord.c_str();
}


// -----------------------------------------------------------------------------
// Name: ParamPoint::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPoint::NotifyContextMenu( const geometry::Point2f& point, ContextMenu& menu )
{
    popupPoint_ = point;
    menu.InsertItem( "Parametre", menu_.c_str(), this, SLOT( AcceptPopupMenuPoint() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::AcceptPopupMenuPoint
// Created: APE 2004-03-23
// -----------------------------------------------------------------------------
void ParamPoint::AcceptPopupMenuPoint()
{
    pPosLabel_->setText( converter_.ConvertToMgrs( popupPoint_ ).c_str() );
}
