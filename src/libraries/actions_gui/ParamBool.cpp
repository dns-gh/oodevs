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
#include "actions/Action_ABC.h"
#include "actions/Bool.h"
#include "clients_kernel/OrderParameter.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamBool constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamBool::ParamBool( QObject* parent, const kernel::OrderParameter& parameter, bool defaultValue /* = false*/ )
    : QObject( parent )
    , Param_ABC( parameter.GetName().c_str(), parameter.IsOptional() )
    , parameter_( parameter )
    , value_( defaultValue )
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
// Name: ParamBool::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamBool::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QHBoxLayout* layout = new QHBoxLayout( group_ );
    layout->setAlignment( Qt::AlignCenter );
    QCheckBox* checkBox = new QCheckBox( parent );
    checkBox->setChecked( value_ );
    layout->addWidget( checkBox, Qt::AlignCenter );
    connect( checkBox, SIGNAL( clicked() ), SLOT( OnClicked() ) );
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
