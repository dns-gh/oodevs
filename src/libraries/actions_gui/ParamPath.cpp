// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamPath.h"
#include "moc_ParamPath.cpp"
#include "actions/Action_ABC.h"
#include "actions/Path.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Point.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/RichLabel.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamPath constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPath::ParamPath( QObject* parent, const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, const kernel::Entity_ABC& entity )
    : QObject( parent )
    , Param_ABC( parameter.GetName().c_str(), parameter.IsOptional() )
    , parameter_( parameter )
    , converter_( converter )
    , layer_( layer )
    , entity_( entity )
    , location_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPath destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPath::~ParamPath()
{
    layer_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ParamPath::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamPath::BuildInterface( QWidget* parent )
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
// Name: ParamPath::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Draw( const geometry::Point2f& , const kernel::Viewport_ABC& , const kernel::GlTools_ABC& tools ) const
{
    if( location_.get() )
        ShapeHandler_ABC::Draw( *location_, geometry::Rectangle2f(), tools ); // $$$$ SBO 2008-06-03:
}

// -----------------------------------------------------------------------------
// Name: ParamPath::InternalCheckValidity
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
bool ParamPath::InternalCheckValidity() const
{
    return location_.get() && location_->IsValid();
}

// -----------------------------------------------------------------------------
// Name: ParamPath::CommitTo
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void ParamPath::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() && location_.get() && location_->IsValid() )
        action.AddParameter( *new actions::parameters::Path( parameter_, converter_, *location_ ) );
    else
        action.AddParameter( *new actions::parameters::Path( parameter_, converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::NotifyContextMenu( const kernel::Nothing&, kernel::ContextMenu& menu )
{
    menu.InsertItem( "Parameter", GetName(), this, SLOT( StartPath() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::StartPath
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::StartPath()
{
    layer_.StartPath( *this, entity_.Get< kernel::Positions >() );
    if( group_ && IsOptional() )
        group_->setChecked( true );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Handle
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Handle( kernel::Location_ABC& location )
{
    if( location.IsValid() )
    {
        location_.reset( &location );
        pPosLabel_->setText( location.GetName() );
        NotifyChange();
    }
}
