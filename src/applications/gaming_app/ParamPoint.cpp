// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPoint.h"
#include "moc_ParamPoint.cpp"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_gui/RichLabel.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamPoint constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPoint::ParamPoint( QWidget* pParent, ASN1T_Point*& asn, const QString& label, const QString& menu, const CoordinateConverter_ABC& converter )
    : QHBox     ( pParent )
    , asn_      ( new ASN1T_Point() )
    , converter_( converter )
    , menu_     ( menu )
{
    asn = asn_;
    setSpacing( 5 );
    pLabel_ = new RichLabel( label, false, this, "" );

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
    delete asn_;
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::CheckValidity
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
bool ParamPoint::CheckValidity()
{
    if( !IsOptional() && pPosLabel_->text() == "---" )
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
    if( pPosLabel_->text() != "---" && extent.IsInside( paramPoint_ ) )
        tools.DrawCross( paramPoint_ );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::Commit
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPoint::Commit()
{
    if( pPosLabel_->text() == "---" )
        return;
    SetOptionalPresent();
    asn_->type                  = EnumTypeLocalisation::point;
    asn_->vecteur_point.n       = 1;
    asn_->vecteur_point.elem    = &asnPoint_;
    const std::string coord = converter_.ConvertToMgrs( paramPoint_ );
    asnPoint_ = coord.c_str();
}


// -----------------------------------------------------------------------------
// Name: ParamPoint::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPoint::NotifyContextMenu( const geometry::Point2f& point, ContextMenu& menu )
{
    popupPoint_ = point;
    menu.InsertItem( "Parameter", menu_, this, SLOT( AcceptPopupMenuPoint() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::AcceptPopupMenuPoint
// Created: APE 2004-03-23
// -----------------------------------------------------------------------------
void ParamPoint::AcceptPopupMenuPoint()
{
    paramPoint_ = popupPoint_;
    pPosLabel_->setText( converter_.ConvertToMgrs( paramPoint_ ).c_str() );
}
