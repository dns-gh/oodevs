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
#include "moc_ParamUrbanBlock.cpp"

#include "actions/Action_ABC.h"
#include "actions/UrbanBlock.h"

#include "clients_kernel/ActionController.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/TerrainObjectProxy.h"

#include "urban/TerrainObject_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock constructor
// Created: MGD 2009-11-03
// -----------------------------------------------------------------------------
ParamUrbanBlock::ParamUrbanBlock( QObject* parent, const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer )
: QObject( parent )
, Param_ABC  ( parameter.GetName().c_str() )
, parameter_ ( parameter )
, layer_     ( layer )
, pLabel_    ( 0 )
, selected_  ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock destructor
// Created: AGE MGD 2009-11-03
// -----------------------------------------------------------------------------
ParamUrbanBlock::~ParamUrbanBlock()
{
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
    pBlockLabel_ = new QLabel( "---", box );
    pBlockLabel_->setMinimumWidth( 100 );
    pBlockLabel_->setAlignment( Qt::AlignCenter );
    pBlockLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
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
    if( selected_ )
    {
        std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::UrbanBlock( parameter_, *(selected_->object_) ) );
        param->Set( true );
        action.AddParameter( *param.release() );
    }   
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

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::NotifyContextMenu
// Created: MGD 2009-11-04
// -----------------------------------------------------------------------------
void ParamUrbanBlock::NotifyContextMenu( const ::gui::TerrainObjectProxy& object, kernel::ContextMenu& menu )
{
    potential_ = &object;
    if( pBlockLabel_->isShown() )
        menu.InsertItem( "Parameter", GetName(), this, SLOT( MenuItemValidated() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::MenuItemValidated
// Created: MGD 2009-11-05
// -----------------------------------------------------------------------------
void ParamUrbanBlock::MenuItemValidated()
{
    selected_ = potential_;
    if( selected_ )
        pBlockLabel_->setText(  selected_->object_->GetName().c_str() );
    else
        pBlockLabel_->setText(  "---" );
}
