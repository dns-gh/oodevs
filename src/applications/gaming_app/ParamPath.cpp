// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPath.h"
#include "moc_ParamPath.cpp"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/RichLabel.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/OrderParameter.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterPath.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamPath constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPath::ParamPath( QObject* parent, const OrderParameter& parameter, ParametersLayer& layer, const CoordinateConverter_ABC& converter, const Entity_ABC& entity )
    : QObject( parent )
    , Param_ABC( parameter.GetName() )
    , parameter_( parameter )
    , converter_( converter )
    , layer_( layer )
    , entity_( entity )
    , pLabel_( 0 )
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPath::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamPath::BuildInterface( QWidget* parent )
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
// Name: ParamPath::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Draw( const geometry::Point2f& , const kernel::Viewport_ABC& , const GlTools_ABC& tools ) const
{
    if( location_.get() )
        location_->Draw( tools );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::CheckValidity
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
bool ParamPath::CheckValidity()
{
    if( !parameter_.IsOptional() && ( !location_.get() || !location_->IsValid() ) )
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamPath::CommitTo
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void ParamPath::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param;
    if( location_.get() )
    {
        param.reset( new ActionParameterPath( parameter_, converter_, *location_ ) );
        param->Set( location_->IsValid() );
    }
    else
    {
        kernel::Point stub;
        param.reset( new ActionParameterPath( parameter_, converter_, stub ) );
        param->Set( false );
    }
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::NotifyContextMenu( const kernel::Nothing&, ContextMenu& menu )
{   
    menu.InsertItem( "Parameter", GetName(), this, SLOT( StartPath() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::StartPath
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::StartPath()
{
    layer_.StartPath( *this, entity_.Get< Positions >() );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Handle
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Handle( Location_ABC& location )
{
    if( location.IsValid() )
    {
        location_.reset( &location );
        pPosLabel_->setText( location.GetName() );
    }
}
