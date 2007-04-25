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
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Point.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterLocation.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamPoint constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPoint::ParamPoint( QObject* parent, const OrderParameter& parameter, const CoordinateConverter_ABC& converter )
    : QObject   ( parent )
    , Param_ABC ( parameter.GetName() )
    , parameter_( parameter )
    , converter_( converter )
    , pLabel_   ( 0 )
{
    // NOTHING
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
    if( !parameter_.IsOptional() && pPosLabel_->text() == "---" )
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
// Name: ParamPoint::CommitTo
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPoint::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_point;
    ASN1T_Point*& point = asn.value.u.point = new ASN1T_Point();
    if( !pPosLabel_ )
        InterfaceNotInitialized();
    asn.null_value = pPosLabel_->text() == "---" ? 1 : 0;
    if( asn.null_value )
        return;
    point->type               = EnumTypeLocalisation::point;
    point->vecteur_point.n    = 1;
    point->vecteur_point.elem = new ASN1T_CoordUTM[1];
    point->vecteur_point.elem[0] = converter_.ConvertToMgrs( paramPoint_ ).c_str();
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::Clean
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamPoint::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.point )
        delete[] asn.value.u.point->vecteur_point.elem;
    delete asn.value.u.point;
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::CommitTo
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void ParamPoint::CommitTo( Action_ABC& action ) const
{
    kernel::Point point;
    point.AddPoint( paramPoint_ );
    std::auto_ptr< ActionParameterLocation > param( new ActionParameterLocation( parameter_, converter_, point ) );
//    param->SetValue( converter_.ConvertToMgrs( paramPoint_ ).c_str() );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPoint::NotifyContextMenu( const geometry::Point2f& point, ContextMenu& menu )
{
    popupPoint_ = point;
    menu.InsertItem( "Parameter", GetName(), this, SLOT( AcceptPopupMenuPoint() ) );
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
