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
#include "clients_kernel/Viewport_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamPoint constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPoint::ParamPoint( QObject* parent, ASN1T_Point*& asn, const QString& name, const QString& menu, const CoordinateConverter_ABC& converter )
    : QObject   ( parent )
    , Param_ABC ( name )
    , asn_      ( new ASN1T_Point() )
    , converter_( converter )
    , menu_     ( menu )
    , pLabel_   ( 0 )
{
    asn = asn_;
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
// Name: ParamPoint::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamPoint::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new RichLabel( GetName(), false, box );
    pPosLabel_ = new QLabel( "---", box );
    pPosLabel_->setMinimumWidth( 100 );
    pPosLabel_->setAlignment( Qt::AlignCenter );
    pPosLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
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
void ParamPoint::Draw( const geometry::Point2f& /*point*/, const kernel::Viewport_ABC& extent, const GlTools_ABC& tools ) const
{
    if( pPosLabel_->text() != "---" && extent.IsVisible( paramPoint_ ) )
        tools.DrawCross( paramPoint_ );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::Commit
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPoint::Commit()
{
    if( !pPosLabel_ )
        InterfaceNotInitialized();
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
