// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamUrbanBlock.h"
#include "actions/Action_ABC.h"
#include "actions/Location.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Point.h"
#include "clients_gui/LocationCreator.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock constructor
// Created: MGD 2009-11-03
// -----------------------------------------------------------------------------
ParamUrbanBlock::ParamUrbanBlock( const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer )
: Param_ABC  ( parameter.GetName().c_str() )
, parameter_ ( parameter )
, layer_     ( layer )
, controller_( 0 )
, pLabel_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock destructor
// Created: AGE MGD 2009-11-03
// -----------------------------------------------------------------------------
ParamUrbanBlock::~ParamUrbanBlock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::BuildInterface
// Created: MGD 2009-11-03
// -----------------------------------------------------------------------------
void ParamUrbanBlock::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new ::gui::RichLabel( GetName(), false, box );
    pShapeLabel_ = new QLabel( "---", box );
    pShapeLabel_->setMinimumWidth( 100 );
    pShapeLabel_->setAlignment( Qt::AlignCenter );
    pShapeLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::RemoveFromController
// Created: MGD 2009-11-03
// -----------------------------------------------------------------------------
void ParamUrbanBlock::RemoveFromController()
{
    Param_ABC::RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::RegisterIn
// Created: MGD 2009-11-03
// -----------------------------------------------------------------------------
void ParamUrbanBlock::RegisterIn( kernel::ActionController& controller )
{
    controller_ = &controller;
    Param_ABC::RegisterIn( controller );
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::CheckValidity
// Created: MGD 2009-11-03
// -----------------------------------------------------------------------------
bool ParamUrbanBlock::CheckValidity()
{
    if( ! parameter_.IsOptional() && false )//@TODO Add urban check
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::CommitTo
// Created: MGD 2009-11-03
// -----------------------------------------------------------------------------
void ParamUrbanBlock::CommitTo( actions::ParameterContainer_ABC& action ) const
{
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::Draw
// Created: MGD 2009-11-03
// -----------------------------------------------------------------------------
void ParamUrbanBlock::Draw( const geometry::Point2f& , const kernel::Viewport_ABC& , const kernel::GlTools_ABC& tools ) const
{

}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::IsOptional
// Created: MGD 2009-11-03
// -----------------------------------------------------------------------------
bool ParamUrbanBlock::IsOptional() const
{
    return parameter_.IsOptional();
}
