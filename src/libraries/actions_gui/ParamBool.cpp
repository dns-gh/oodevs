// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamBool.h"
#include "moc_ParamBool.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/Bool.h"
#include "clients_kernel/OrderParameter.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamBool constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamBool::ParamBool( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , value_   ( false )
    , checkBox_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamBool destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamBool::~ParamBool()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamBool::SetValue
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
void ParamBool::SetValue( bool value )
{
    value_ = value;
    if( checkBox_ != 0 )
        checkBox_->setChecked( value_ );
}

// -----------------------------------------------------------------------------
// Name: ParamBool::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamBool::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QHBoxLayout* layout = new QHBoxLayout( group_ );
    layout->setAlignment( Qt::AlignCenter );
    checkBox_ = new QCheckBox( parent );
    checkBox_->setChecked( value_ );
    layout->addWidget( checkBox_, Qt::AlignCenter );
    connect( checkBox_, SIGNAL( clicked() ), SLOT( OnClicked() ) );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamBool::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamBool::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() )
        action.AddParameter( *new actions::parameters::Bool( parameter_, value_ ) );
    else
        action.AddParameter( *new actions::parameters::Bool( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamBool::OnClicked
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamBool::OnClicked()
{
    value_ = !value_;
}
