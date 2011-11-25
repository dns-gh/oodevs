// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamPoint.h"
#include "moc_ParamPoint.cpp"
#include "actions/Action_ABC.h"
#include "actions/Point.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_gui/RichLabel.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamPoint constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPoint::ParamPoint( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter )
    : QObject   ( parent )
    , Param_ABC ( parameter.GetName().c_str(), parameter.IsOptional() )
    , parameter_( parameter )
    , converter_( converter )
    , isSet_    ( false )
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
QWidget* ParamPoint::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    pPosLabel_ = new QLabel( "---", parent );
    pPosLabel_->setMinimumWidth( 100 );
    pPosLabel_->setAlignment( Qt::AlignCenter );
    layout->addWidget( pPosLabel_ );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::InternalCheckValidity
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
bool ParamPoint::InternalCheckValidity() const
{
    return isSet_;
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPoint::Draw( const geometry::Point2f& /*point*/, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const
{
    if( isSet_ && extent.IsVisible( paramPoint_ ) )
        tools.DrawCross( paramPoint_ );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::CommitTo
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void ParamPoint::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    kernel::Point point;
    if( isSet_ )
        point.AddPoint( paramPoint_ );
    if( IsChecked() && isSet_ )
    {
        kernel::Point point;
        point.AddPoint( paramPoint_ );
        action.AddParameter( *new actions::parameters::Point( parameter_, converter_, point ) );
    }
    else
        action.AddParameter( *new actions::parameters::Point( parameter_, converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamPoint::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPoint::NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu )
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
    isSet_ = true;
    NotifyChange();
}
